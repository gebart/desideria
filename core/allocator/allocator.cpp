/*
 * Copyright (c) 2020 Joakim Nohlgård
 */
#include "deri/allocator.h"
#include "deri/assert.h"
#include <cstdint>
extern "C" {
#include <stdio.h>
}

namespace deri {

namespace {
template <typename T, typename U> T *aligned_ptr(U *possibly_unaligned_ptr) {
  return reinterpret_cast<T *>(
      static_cast<uint8_t *>(nullptr) +
      ((static_cast<uint8_t *>(possibly_unaligned_ptr) + alignof(T) - 1 -
        static_cast<uint8_t *>(nullptr)) &
       ~(alignof(T) - 1)));
}

static const Allocator::bitmap_type order_masks[Allocator::MAX_ORDER + 1] = {
    0x0000ffff, // ~0 << 16 >> 16, 16 = 1 << MAX_ORDER - order, 16 = sizeof(bitmap_type) * 8 - 16
    0x00ff0000, // ~0 << 24 >> 8,   8 = 1 << MAX_ORDER - order, 24 = sizeof(bitmap_type) * 8 - 8
    0x0f000000, // ~0 << 28 >> 4,   4 = 1 << MAX_ORDER - order, 28 = sizeof(bitmap_type) * 8 - 4
    0x30000000, // ~0 << 30 >> 2,   2 = 1 << MAX_ORDER - order, 30 = sizeof(bitmap_type) * 8 - 2
    0x40000000, // ~0 << 31 >> 1,   1 = 1 << MAX_ORDER - order, 31 = sizeof(bitmap_type) * 8 - 1
};
}

struct Allocator::FreeBlock {
  FreeBlock *next;
};

Allocator::Allocator(void *area_base, size_t area_size, size_t block_size)
    : aligned_base{nullptr}, free_map{nullptr}, split_map{nullptr},
      free_blocks{0}, block_size{block_size}, block_size_log2{0}, map_count{0},
      map_stride_log2{0} {
  deri::assert((block_size > 0) && (block_size & (block_size - 1u)) == 0,
               "block_size must be a power of 2");
  size_t max_block_size = (block_size << MAX_ORDER);
  printf("block_size     = %5lu (0x%04lx)\n",
         static_cast<unsigned long>(block_size),
         static_cast<unsigned long>(block_size));
  printf("max_block_size = %5lu (0x%04lx)\n",
         static_cast<unsigned long>(max_block_size),
         static_cast<unsigned long>(max_block_size));
  deri::assert(max_block_size >= block_size, "max_block_size overflow");
  for (size_t k = block_size >> 1; k > 0; k >>= 1) {
    ++block_size_log2;
  }
  map_stride_log2 = block_size_log2 + MAX_ORDER;
  printf("block_size_log2 = %u\n", block_size_log2);
  printf("map_stride_log2 = %u\n", map_stride_log2);
  // aligned_base, aligned_size may point outside of the designated area, but we
  // will mark as used all blocks which are out of bounds.
  aligned_base =
      static_cast<uint8_t *>(nullptr) +
      ((static_cast<uint8_t *>(area_base) - static_cast<uint8_t *>(nullptr)) &
       ~(max_block_size - 1u));
  offset_type aligned_size = (static_cast<uint8_t *>(area_base) + area_size -
                              aligned_base + max_block_size - 1u) &
                             ~(max_block_size - 1u);
  printf("area_base: %p\n", static_cast<void *>(area_base));
  printf("area_size: %lu\n", static_cast<unsigned long>(area_size));
  printf("aligned_base: %p\n", static_cast<void *>(aligned_base));
  printf("aligned_size: %6lu (0x%04lx)\n",
         static_cast<unsigned long>(aligned_size),
         static_cast<unsigned long>(aligned_size));
  map_count = aligned_size >> map_stride_log2;

  // NB: ensure alignment before casting byte pointer into wider type pointer!
  free_map = reinterpret_cast<bitmap_type *>(
      static_cast<uint8_t *>(nullptr) +
      ((static_cast<uint8_t *>(area_base) + alignof(bitmap_type) - 1 -
        static_cast<uint8_t *>(nullptr)) &
       ~(alignof(bitmap_type) - 1)));
  split_map = free_map + map_count;
  printf("free_map:  %p (%u elements)\n", static_cast<void *>(free_map),
         map_count);
  printf("split_map: %p (%u elements)\n", static_cast<void *>(split_map),
         map_count);
  printf("map_stride = %u (2**%u)\n",
         static_cast<unsigned int>(1u << map_stride_log2),
         static_cast<unsigned int>(map_stride_log2));
  // Round up to find the first completely free block
  offset_type first_free_block =
      (((reinterpret_cast<uint8_t *>(split_map + map_count) - aligned_base) +
        block_size - 1) &
       ~(block_size - 1)) >>
      block_size_log2;
  // Round down for the past-the-end block
  offset_type end_block =
      ((static_cast<uint8_t *>(area_base) + area_size - aligned_base) &
       ~(block_size - 1)) >>
      block_size_log2;

  init_free_blocks_list(first_free_block, end_block);
}

void *Allocator::allocate(size_t) { return aligned_base; }

void Allocator::init_free_blocks_list(offset_type free_begin,
                                      offset_type free_end) {
  printf("map_count = %u\n", map_count);
  for (unsigned int k = 0; k <= MAX_ORDER; ++k) {
    free_blocks[k] = nullptr;
  }

  // Scan the free space and add as large blocks as possible to the free blocks
  // lists
  offset_type block = free_begin;
  unsigned int order = 0;
  while (block < free_end) {
    FreeBlock *free_block =
        ::new (aligned_base + (block << block_size_log2)) FreeBlock();
    printf("next free: %p\n", static_cast<void *>(free_block));
    for (offset_type k = block >> order; order < MAX_ORDER && (k & 1u) == 0; k >>= 1u) {
      ++order;
    }
    while (order > 0 && (block + (1u << order)) > free_end) {
      --order;
    }
    printf("next free block order: %u (0x%04x)\n", order,
           (block_size << order));
    size_t map_idx = block >> MAX_ORDER;
    printf("map_idx = %u\n", map_idx);
    size_t block_sub =
        (block >> order) & ((1u << (MAX_ORDER + 1 - order)) - 1u);
    printf("block_sub = %u (0x%04x)\n", static_cast<unsigned>(block_sub),
           static_cast<unsigned>(block_sub << (block_size_log2 + order)));
    bitmap_type block_mask =
        block_sub << (sizeof(bitmap_type) * 8 - (1u << (MAX_ORDER - order)));
    printf("block_mask = 0x%08x\n", static_cast<unsigned>(block_mask));
    //    bitmap_type order_mask = block >> map_stride_log2 - order;
    //        free_map[map_idx] =
    free_block->next = free_blocks[order];
    free_blocks[order] = free_block;
    block += 1u << order;
  }

  printf("free blocks table:\n");
  for (unsigned int k = 0; k <= MAX_ORDER; ++k) {
    printf("  [%u] (%5lu)", k, static_cast<unsigned long>(block_size << k));
    FreeBlock *it = free_blocks[k];
    while (it) {
      printf(" -> %p", static_cast<void *>(it));
      it = it->next;
    }
    puts("");
  }
}

Allocator *create_allocator(void *area_base, size_t area_size,
                            size_t block_size) {
  uint8_t *area_end = static_cast<uint8_t *>(area_base) + area_size;
  // Align object placement pointer first, rounding up
  Allocator *object_ptr = aligned_ptr<Allocator>(area_base);
  // trim leading area to allow space for storing the Allocator instance
  area_base = static_cast<void *>(object_ptr + 1);
  deri::assert(area_base < area_end, "area too small");
  // recalculate size after trimming
  area_size = area_end - static_cast<uint8_t *>(area_base);
  return ::new (object_ptr) Allocator(area_base, area_size, block_size);
}
} // namespace deri
