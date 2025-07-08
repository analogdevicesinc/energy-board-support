cmake_minimum_required(VERSION 3.22)

project(app_mcu_h5 C)

option(SUPPRESS_MSDK_WARNINGS "Suppress warnings for Driver sources" ON)
option(USE_HW_CRC "Compile IIO board_support sources" OFF)
add_library(app_mcu_h5 INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

set(STM32H573_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Base directory for HAL Drivers
set(SDK C:/STM32Cube/Repository/STM32Cube_FW_H5_V1.3.0/Drivers/STM32H5xx_HAL_Driver CACHE
    PATH "Path to STM32CubeH5 directory")

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    # Add user defined symbols
    STM32H573xx
    STM32H573ZI
)

# Needed to print variables in cmake
include(CMakePrintHelpers)

set(BOARD_SRC ${STM32H573_ROOT_DIR}/source/app_mcu_h5.c
				${STM32H573_ROOT_DIR}/source/app_mcu_h5_control.c
				${STM32H573_ROOT_DIR}/source/app_mcu_h5_gpio.c
                ${STM32H573_ROOT_DIR}/source/app_mcu_h5_spi.c
                ${STM32H573_ROOT_DIR}/../source/stm_spi.c
                ${STM32H573_ROOT_DIR}/../source/stm_uart.c
                ${STM32H573_ROOT_DIR}/../source/stm_i2c.c
                ${STM32H573_ROOT_DIR}/../source/stm_timer.c
                ${STM32H573_ROOT_DIR}/../source/stm32h563x_timer_config.c
                ${STM32H573_ROOT_DIR}/../source/stm32h563x_uart_config.c
                ${STM32H573_ROOT_DIR}/../source/stm32h563x_i2c_config.c
                ${STM32H573_ROOT_DIR}/../source/stm32h563xx_spi_config.c

)
if(USE_CRC AND USE_HW_CRC)
set(CRC_SRC ${STM32H573_ROOT_DIR}/../source/stm_crc.c)
else()
set(CRC_SRC "")
endif()


set(STM_GEN_CODE_SRC ${STM32H573_ROOT_DIR}/stm_gen_code/stm32h5xx_hal_msp.c
                     ${STM32H573_ROOT_DIR}/stm_gen_code/stm32h5xx_it.c
                     ${STM32H573_ROOT_DIR}/stm_gen_code/system_stm32h5xx.c
                     ${STM32H573_ROOT_DIR}/stm_gen_code/startup_stm32h573xx.s
                     ${STM32H573_ROOT_DIR}/stm_gen_code/sysmem.c
                     ${STM32H573_ROOT_DIR}/stm_gen_code/syscalls.c
)

set(STM32_HAL_FILES    # HAL driver sources
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

# Create a library for STMCubeH5 HAL files and suppress warnings
add_library(hal_drivers INTERFACE)
target_sources(hal_drivers INTERFACE ${STM32_HAL_FILES})

if (SUPPRESS_MSDK_WARNINGS)
    # Suppress warnings for STMCubeH5 HAL files
    target_compile_options(hal_drivers INTERFACE -w)
endif()

# Create a library for your board source files (warnings enabled)
add_library(board_src INTERFACE)
target_sources(board_src INTERFACE ${BOARD_SRC} ${STM_GEN_CODE_SRC} ${CRC_SRC})

target_include_directories(app_mcu_h5 INTERFACE
    ${STM32H573_ROOT_DIR}/include
    ${STM32H573_ROOT_DIR}/../include
    ${STM32H573_ROOT_DIR}/stm_gen_code
    ${STM32H573_ROOT_DIR}/include/config
    ${SDK}/Inc
    ${SDK}/Inc/Legacy
    ${SDK}/../CMSIS/Device/ST/STM32H5xx/Include
    ${SDK}/../CMSIS/Include
)

target_link_directories(app_mcu_h5 INTERFACE
)

# Add project symbols (macros)
target_compile_definitions(board_src INTERFACE
    USE_HAL_DRIVER
    STM32H573xx
    $<$<CONFIG:Debug>:DEBUG>
    # Add user defined symbols
    STM32H573ZI
    _USE_STD_INT_TYPES
	EVB
)

# Main target
target_link_libraries(app_mcu_h5 INTERFACE hal_drivers board_src)
