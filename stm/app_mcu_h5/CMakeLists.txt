cmake_minimum_required(VERSION 3.22)

project(app_mcu_h5 C)

option(SUPPRESS_SDK_WARNINGS "Suppress warnings for Driver sources" ON)
option(USE_HW_CRC "HW Crc Support" OFF)

if(NOT DEFINED BOARD_SUPPORT_DIR )
    set(BOARD_SUPPORT_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)
endif()


# Board source files
set(BOARD_SRC
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/source/app_mcu_h5.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/source/app_mcu_h5_control.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/source/app_mcu_h5_gpio.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/source/app_mcu_h5_spi.c
    ${BOARD_SUPPORT_DIR}/stm/source/stm_spi.c
    ${BOARD_SUPPORT_DIR}/stm/source/stm_uart.c
    ${BOARD_SUPPORT_DIR}/stm/source/stm_i2c.c
    ${BOARD_SUPPORT_DIR}/stm/source/stm_timer.c
    ${BOARD_SUPPORT_DIR}/stm/source/stm32h563x_timer_config.c
    ${BOARD_SUPPORT_DIR}/stm/source/stm32h563x_uart_config.c
    ${BOARD_SUPPORT_DIR}/stm/source/stm32h563x_i2c_config.c
    ${BOARD_SUPPORT_DIR}/stm/source/stm32h563xx_spi_config.c
)

# CRC source file
if(USE_CRC AND USE_HW_CRC)
    set(CRC_SRC ${BOARD_SUPPORT_DIR}/stm/source/stm_crc.c)
else()
    set(CRC_SRC "")
endif()

# Generated code sources
if(TOOLCHAIN MATCHES "armclang")
    set(GEN_CODE_SRC
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/stm_gen_code/stm32h5xx_hal_msp.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/stm_gen_code/stm32h5xx_it.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/armclang_gen_code/system_stm32h5xx.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/armclang_gen_code/startup_stm32h573xx.s
    )
elseif(TOOLCHAIN MATCHES "gcc-arm-none-eabi")
    set(GEN_CODE_SRC
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/stm_gen_code/stm32h5xx_hal_msp.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/stm_gen_code/stm32h5xx_it.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/stm_gen_code/system_stm32h5xx.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/stm_gen_code/startup_stm32h573xx.s
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/stm_gen_code/sysmem.c
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/stm_gen_code/syscalls.c
)
endif()

# HAL driver sources
set(STM32_HAL_FILES
    ${SDK}/Src/stm32h5xx_hal_cortex.c
    ${SDK}/Src/stm32h5xx_hal_crc.c
    ${SDK}/Src/stm32h5xx_hal_crc_ex.c
    ${SDK}/Src/stm32h5xx_hal_icache.c
    ${SDK}/Src/stm32h5xx_hal_rcc.c
    ${SDK}/Src/stm32h5xx_hal_rcc_ex.c
    ${SDK}/Src/stm32h5xx_hal_flash.c
    ${SDK}/Src/stm32h5xx_hal_flash_ex.c
    ${SDK}/Src/stm32h5xx_hal_gpio.c
    ${SDK}/Src/stm32h5xx_hal_irda.c
    ${SDK}/Src/stm32h5xx_hal_dma.c
    ${SDK}/Src/stm32h5xx_hal_dma_ex.c
    ${SDK}/Src/stm32h5xx_hal_pwr.c
    ${SDK}/Src/stm32h5xx_hal_pwr_ex.c
    ${SDK}/Src/stm32h5xx_hal.c
    ${SDK}/Src/stm32h5xx_hal_exti.c
    ${SDK}/Src/stm32h5xx_hal_spi.c
    ${SDK}/Src/stm32h5xx_hal_spi_ex.c
    ${SDK}/Src/stm32h5xx_hal_tim.c
    ${SDK}/Src/stm32h5xx_hal_tim_ex.c
    ${SDK}/Src/stm32h5xx_hal_uart.c
    ${SDK}/Src/stm32h5xx_hal_uart_ex.c
    ${SDK}/Src/stm32h5xx_hal_i2c.c
    ${SDK}/Src/stm32h5xx_hal_i2c_ex.c
    ${SDK}/Src/stm32h5xx_hal_lptim.c
)

# Create HAL drivers library and suppress warnings if requested
add_library(hal_drivers INTERFACE)
target_sources(hal_drivers INTERFACE ${STM32_HAL_FILES})
if(SUPPRESS_SDK_WARNINGS)
    target_compile_options(hal_drivers INTERFACE -w)
endif()

# Suppress cast alignment warnings for CRC source file
set_source_files_properties(${CRC_SRC} PROPERTIES COMPILE_FLAGS "-Wno-cast-align")

# Create board source library
add_library(board_src INTERFACE)
target_sources(board_src INTERFACE ${BOARD_SRC} ${GEN_CODE_SRC} ${CRC_SRC})

if(TOOLCHAIN MATCHES "armclang")
    target_compile_definitions(board_src INTERFACE
        # CMake applies target_compile_definitions to all source files (.c and .s) as -DEVB etc.
        # But for armasm.exe, this is not a recognized option.
        # So add compile definitions only for .c and .cpp files.
        $<$<COMPILE_LANGUAGE:C>:STM32H573xx;STM32H573ZI;USE_HAL_DRIVER;EVB>
        $<$<COMPILE_LANGUAGE:CXX>:STM32H573xx;STM32H573ZI;USE_HAL_DRIVER;EVB>
        $<$<CONFIG:Debug>:$<$<COMPILE_LANGUAGE:C>:DEBUG>>
        $<$<CONFIG:Debug>:$<$<COMPILE_LANGUAGE:CXX>:DEBUG>>
    )
elseif(TOOLCHAIN MATCHES "gcc-arm-none-eabi")
    target_compile_definitions(board_src INTERFACE
        USE_HAL_DRIVER
        STM32H573xx
        $<$<CONFIG:Debug>:DEBUG>
        STM32H573ZI
        _USE_STD_INT_TYPES
        EVB
    )
endif()

# Main target
add_library(app_mcu_h5 INTERFACE)
target_include_directories(app_mcu_h5 INTERFACE
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/include
    ${BOARD_SUPPORT_DIR}/stm/include
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/stm_gen_code
    ${BOARD_SUPPORT_DIR}/stm/app_mcu_h5/include/config
    ${SDK}/Inc
    ${SDK}/Inc/Legacy
    ${SDK}/../CMSIS/Device/ST/STM32H5xx/Include
    ${SDK}/../CMSIS/Include
)
target_link_directories(app_mcu_h5 INTERFACE)
target_link_libraries(app_mcu_h5 INTERFACE hal_drivers board_src)
