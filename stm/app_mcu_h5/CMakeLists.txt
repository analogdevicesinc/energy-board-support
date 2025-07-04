cmake_minimum_required(VERSION 3.22)
project(stm32h573zi C)

option(USE_IIO "Compile IIO board_support sources" OFF)
option(USE_HW_CRC "Compile IIO board_support sources" OFF)
add_library(stm32h573zi INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

set(STM32H573_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Base directory for HAL Drivers
set(STM32H5_HAL_DRIVERS C:/STM32Cube/Repository/STM32Cube_FW_H5_V1.3.0/Drivers/STM32H5xx_HAL_Driver CACHE
    PATH "Path to STM32CubeH5 directory")

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    # Add user defined symbols
    STM32H573ZI
)

# Needed to print variables in cmake
include(CMakePrintHelpers)

set(BOARD_SRC ${STM32H573_ROOT_DIR}/source/app_mcu_h5.c
				${STM32H573_ROOT_DIR}/source/app_mcu_h5_control.c
				${STM32H573_ROOT_DIR}/source/app_mcu_h5_gpio.c
                ${STM32H573_ROOT_DIR}/../source/stm_uart.c
                ${STM32H573_ROOT_DIR}/../source/stm_i2c.c
                ${STM32H573_ROOT_DIR}/../source/stm_timer.c
                ${STM32H573_ROOT_DIR}/../source/stm32h563x_timer_config.c
                ${STM32H573_ROOT_DIR}/../source/stm32h563x_uart_config.c
                ${STM32H573_ROOT_DIR}/../source/stm32h563x_i2c_config.c

)
if(USE_CRC AND USE_HW_CRC)
set(CRC_SRC ${STM32H573_ROOT_DIR}/../source/stm_crc.c)
else()
set(CRC_SRC "")
endif()

set(IIO_SRC ${STM32H573_ROOT_DIR}/source/app_mcu_h5_spi.c
            ${STM32H573_ROOT_DIR}/../source/stm_spi.c
            ${STM32H573_ROOT_DIR}/../source/stm32h563xx_spi_config.c)

set(STM_GEN_CODE_SRC ${STM32H573_ROOT_DIR}/stm_gen_code/stm32h5xx_hal_msp.c
                     ${STM32H573_ROOT_DIR}/stm_gen_code/stm32h5xx_it.c
                     ${STM32H573_ROOT_DIR}/stm_gen_code/system_stm32h5xx.c
                     ${STM32H573_ROOT_DIR}/stm_gen_code/startup_stm32h573xx.s
                     ${STM32H573_ROOT_DIR}/stm_gen_code/sysmem.c
                     ${STM32H573_ROOT_DIR}/stm_gen_code/syscalls.c
)

set(STM32_HAL_FILES    # HAL driver sources
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_cortex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_crc.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_crc_ex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_icache.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_rcc.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_rcc_ex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_flash.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_flash_ex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_gpio.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_irda.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_dma.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_dma_ex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_pwr.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_pwr_ex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_exti.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_spi.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_spi_ex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_tim.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_tim_ex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_uart.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_uart_ex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_i2c.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_i2c_ex.c
    ${STM32H5_HAL_DRIVERS}/Src/stm32h5xx_hal_lptim.c
)

target_sources(stm32h573zi INTERFACE
    ${STM_GEN_CODE_SRC}
    ${STM32_HAL_FILES}
    ${BOARD_SRC}
    ${IIO_SRC}
    ${CRC_SRC}
)

target_include_directories(stm32h573zi INTERFACE
    ${STM32H573_ROOT_DIR}/include
    ${STM32H573_ROOT_DIR}/../include
    ${STM32H573_ROOT_DIR}/stm_gen_code
    ${STM32H573_ROOT_DIR}/include/config
    ${STM32H5_HAL_DRIVERS}/Inc
    ${STM32H5_HAL_DRIVERS}/Inc/Legacy
    ${STM32H5_HAL_DRIVERS}/../CMSIS/Device/ST/STM32H5xx/Include
    ${STM32H5_HAL_DRIVERS}/../CMSIS/Include
)

target_link_directories(stm32h573zi INTERFACE
)

target_link_libraries(stm32h573zi INTERFACE
)
