if (NOT SOC_MODEL)
  message(FATAL_ERROR "Missing SOC_MODEL setting in deri_bsp.cmake")
endif ()

set(ARCH riscv)
set(CPU_CORE bumblebee)
set(CPU rv32)
set(CPU_C_FLAGS -march=rv32imac -mabi=ilp32)
set(CODE_MODEL_C_FLAGS -mcmodel=medlow)
target_compile_options(deri_soc_flags INTERFACE ${CODE_MODEL_C_FLAGS} ${CPU_C_FLAGS})
target_link_options(deri_soc_flags INTERFACE ${CPU_C_FLAGS})

set(RISCV_RAM_LENGTH 32K)
set(RISCV_RAM_ORIGIN 0x20000000)
set(RISCV_ROM_ORIGIN 0x08000000)
if (SOC_MODEL MATCHES "^gd32vf103(.)(.)(.)(.)$")
  set(gd32vf1_mem ${CMAKE_MATCH_2})
  if (gd32vf1_mem STREQUAL "b")
    set(RISCV_ROM_LENGTH 128K)
  elseif (gd32vf1_mem STREQUAL "8")
    set(RISCV_ROM_LENGTH 64K)
  elseif (gd32vf1_mem STREQUAL "6")
    set(RISCV_ROM_LENGTH 32K)
  elseif (gd32vf1_mem STREQUAL "4")
    set(RISCV_ROM_LENGTH 16K)
  else ()
    message(FATAL_ERROR "GD32VF1 unknown memory size designation \"${gd32vf1_mem}\"")
  endif ()
  target_link_libraries(deri_soc_flags INTERFACE "-T gd32vf103_mmio.ld")
else ()
  message(FATAL_ERROR "Unknown GD32VF1 model ${SOC_MODEL}")
endif ()
