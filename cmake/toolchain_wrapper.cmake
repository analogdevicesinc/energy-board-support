 # Copyright (c) 2025  Analog Devices Inc
 
set(STM32CLT_PATH $ENV{STM32CLT_PATH})
# Set default toolchain path, target and ldf file. User can override on command line

if(EVB MATCHES "app_mcu_h5")
  include(${CMAKE_CURRENT_LIST_DIR}/st_tools.cmake)
  set(TARGET_FLAGS "-mcpu=cortex-m33 -mfpu=fpv4-sp-d16 -mfloat-abi=hard " CACHE STRING "Target flags")
  set(LD_FILE "${CMAKE_CURRENT_LIST_DIR}/../stm/app_mcu_h5/stm_gen_code/STM32H573xx_FLASH.ld" CACHE FILEPATH "Linker script")
elseif(EVB MATCHES "nucleo_h563zi")
  include(${CMAKE_CURRENT_LIST_DIR}/st_tools.cmake)
  set(TARGET_FLAGS "-mcpu=cortex-m33 -mfpu=fpv4-sp-d16 -mfloat-abi=hard " CACHE STRING "Target flags")
  set(LD_FILE "${CMAKE_CURRENT_LIST_DIR}/../stm/nucleo_h563zi_master/stm_gen_code/STM32H563ZITX_FLASH.ld" CACHE FILEPATH "Linker script")
elseif(EVB MATCHES "eval_ade9178")
  include(${CMAKE_CURRENT_LIST_DIR}/max_tools.cmake)
  set(LD_FILE "${CMAKE_CURRENT_LIST_DIR}/../max/eval_ade9178/max32670.ld" CACHE FILEPATH "Linker script")
  set(TARGET_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16" CACHE STRING "Target flags")
elseif(EVB MATCHES "app_mcu_h7")
  include(${CMAKE_CURRENT_LIST_DIR}/st_tools.cmake)
  set(LD_FILE "${BOARD_SUPPORT_DIR}/stm/app_mcu_h7/stm_gen_code/stm32h735igkx_flash.ld" CACHE FILEPATH "Linker script")
  set(TARGET_FLAGS "-mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard " CACHE STRING "Target flags")
elseif(EVB MATCHES "dummy_board")
  include(${CMAKE_CURRENT_LIST_DIR}/gcc_tools.cmake)
  set(TOOLCHAIN "gcc")
endif()


if(TOOLCHAIN MATCHES "armclang")
  include(${CMAKE_CURRENT_LIST_DIR}/armclang.cmake)
elseif(TOOLCHAIN MATCHES "iar")
  include(${CMAKE_CURRENT_LIST_DIR}/iar.cmake)
elseif (TOOLCHAIN STREQUAL "gcc")
  message("use gcc toolchain")
else()
  set(TOOLCHAIN "gcc-arm-none-eabi")
  #Default to gcc-arm-none-eabi if no specific toolchain is set
  include(${CMAKE_CURRENT_LIST_DIR}/gcc-arm-none-eabi.cmake)
endif()
# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# Enable CMake support for ASM and C languages
enable_language(C ASM)


message(STATUS "EVB: ${EVB}")
message(STATUS "SDK: ${SDK}")
message(STATUS "Toolchain: ${TOOLCHAIN_PATH},${TOOLCHAIN}")
message(STATUS "Binary output path: ${CMAKE_CURRENT_BINARY_DIR}")

