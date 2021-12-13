/*
 * Copyright (c) 2021 Joakim Nohlgård
 */

#include "deri/thread.hpp"

#include "deri/arch/syscall.hpp"
#include "deri/idle.hpp"
#include "deri/span_literals.hpp"

extern "C" {
// defined in linker script:
extern std::byte _main_stack_start[];
extern std::byte _main_stack_end[];

int main();
}

namespace deri {

void Thread::start() { Scheduler::yield(*this); }

void Thread::block(Status blocked_status) {
  status = blocked_status;
  Scheduler::block(*this);
}

void Thread::unblock() {
  status = Status::WAITING;
  Scheduler::yield(*this);
}

void Scheduler::init() {
  using namespace deri::literals;
  IdleThread::init();

  auto &main_thread =
      Thread::create(std::span(_main_stack_start, _main_stack_end),
                     Thread::Priority::NORMAL,
                     "main"_span,
                     main);
  main_thread.start();
  // Switch to the main thread
  arch::syscall(Syscall::SCHEDULER_UPDATE);
  // we will never return here.
  __builtin_unreachable();
}

void Scheduler::yield(Thread &thread) {
  run_queue.remove(thread);
  run_queue.push(thread);
}

void Scheduler::block(Thread &thread) { run_queue.remove(thread); }

void Scheduler::yield() {
  auto &current_thread = run_queue.front();
  run_queue.pop();
  run_queue.push(current_thread);
  update();
}

}  // namespace deri
