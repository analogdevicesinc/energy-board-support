#Wrapper for stm toolchain
if(EVB MATCHES "app_mcu_h5")
  set(TOOLCHAIN_PATH "C:/ST/STM32CubeCLT_1.16.0/GNU-tools-for-STM32/bin")
  set(TOOLCHAIN_PREFIX "${TOOLCHAIN_PATH}/arm-none-eabi-")
  set(TARGET_FLAGS "${TARGET_FLAGS} -mcpu=cortex-m33 -mfpu=fpv4-sp-d16 -mfloat-abi=hard ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
  set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} ${TARGET_FLAGS}")
  set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_CURRENT_LIST_DIR}/../stm/app_mcu_h5/stm_gen_code/STM32H573xx_FLASH.ld\"")

elseif(EVB MATCHES "nucleo_h563zi")
  set(TOOLCHAIN_PATH "C:/ST/STM32CubeCLT_1.16.0/GNU-tools-for-STM32/bin")
  set(TOOLCHAIN_PREFIX "${TOOLCHAIN_PATH}/arm-none-eabi-")
  set(TARGET_FLAGS "${TARGET_FLAGS} -mcpu=cortex-m33 -mfpu=fpv4-sp-d16 -mfloat-abi=hard ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
  set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} ${TARGET_FLAGS}")
  set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_CURRENT_LIST_DIR}/../stm/nucleo_h563zi_master/stm_gen_code/STM32H563ZITX_FLASH.ld\"")

elseif(EVB MATCHES "eval_ade9178")
  set(SDK C:/MaximSDK CACHE PATH "Path to MAXIM_DRIVERS directory")
  set(TOOLCHAIN_PATH "${SDK}/Tools/GNUTools/10.3/bin")
  set(TOOLCHAIN_PREFIX "${TOOLCHAIN_PATH}/arm-none-eabi-")
  set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -u _printf_float -u _scanf_float")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=nosys.specs")
  set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${CMAKE_CURRENT_LIST_DIR}/../max/eval_ade9178/max32670.ld\"")
  set(TARGET_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
  set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} ${TARGET_FLAGS}")
endif()

message(STATUS "Using toolchain path: ${TOOLCHAIN_PATH}")

# Include toolchain file
include(${CMAKE_CURRENT_LIST_DIR}/gcc-arm-none-eabi.cmake)