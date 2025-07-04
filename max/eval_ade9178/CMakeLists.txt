cmake_minimum_required(VERSION 3.22)
project(max32670 C)

option(USE_IIO "Compile IIO board_support sources" OFF)
add_library(max32670 INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

set(MAX32670_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Define MAXIM_DRIVERS path
set(MAXIM_DRIVERS C:/MaximSDK CACHE PATH "Path to MAXIM_DRIVERS directory")

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    # Add user defined symbols
    MAX32670
)

# Needed to print variables in cmake
include(CMakePrintHelpers)

set(BOARD_SRC ${MAX32670_ROOT_DIR}/source/eval_ade9178.c
				${MAX32670_ROOT_DIR}/source/eval_ade9178_control.c
				${MAX32670_ROOT_DIR}/source/eval_ade9178_spi.c
                ${MAX32670_ROOT_DIR}/source/eval_ade9178_gpio.c
                ${MAX32670_ROOT_DIR}/source/eval_ade9178_uart.c
                ${MAX32670_ROOT_DIR}/source/eval_ade9178_flc.c
                ${MAX32670_ROOT_DIR}/../source/max3267x_spi_config.c
                ${MAX32670_ROOT_DIR}/../source/max3267x_uart_config.c
                ${MAX32670_ROOT_DIR}/../source/max3267x_dma_config.c
                ${MAX32670_ROOT_DIR}/../source/max3267x_timer_config.c
                ${MAX32670_ROOT_DIR}/../source/max3267x_flc_config.c
                
)

# Add MaximSDK driver source files
file(GLOB_RECURSE DEVICE_SOURCES ${MAXIM_DRIVERS}/Libraries/CMSIS/Device/Maxim/Source/*.c)

set(MAXIM_FILES    # HAL driver sources
        ${MAXIM_DRIVERS}/Libraries/CMSIS/Device/Maxim/MAX32670/Source/GCC/startup_max32670.s
        ${MAXIM_DRIVERS}/Libraries/CMSIS/Device/Maxim/MAX32670/Source/system_max32670.c
        ${MAXIM_DRIVERS}/Libraries/CMSIS/Device/Maxim/MAX32670/Source/heap.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/SYS/mxc_assert.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/SYS/mxc_delay.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/SYS/mxc_lock.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/SYS/nvic_table.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/SYS/pins_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/SYS/sys_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/AES/aes_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/AES/aes_revb.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/DMA/dma_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/DMA/dma_reva.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/GPIO/gpio_common.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/GPIO/gpio_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/GPIO/gpio_reva.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/SPI/spi_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/SPI/spi_reva1.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/TMR/tmr_common.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/TMR/tmr_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/UART/uart_common.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/UART/uart_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/UART/uart_revb.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/UART/uart_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/UART/uart_revb.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/FLC/flc_common.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/FLC/flc_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/FLC/flc_reva.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/ICC/icc_common.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/ICC/icc_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/ICC/icc_reva.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/TMR/tmr_common.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/TMR/tmr_me15.c
        ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/TMR/tmr_revb.c
        ${DEVICE_SOURCES}
)

file(GLOB HEADER_DIRS LIST_DIRECTORIES true ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Source/*)

target_sources(max32670 INTERFACE
    ${MAXIM_FILES}
    ${BOARD_SRC}
)

target_include_directories(max32670 INTERFACE
    ${MAX32670_ROOT_DIR}/include
    ${MAX32670_ROOT_DIR}/../include
    ${MAX32670_ROOT_DIR}/include/config
    ${MAXIM_DRIVERS}/Libraries/PeriphDrivers/Include/MAX32670
    ${MAXIM_DRIVERS}/Libraries/CMSIS/5.9.0/Core/Include
    ${MAXIM_DRIVERS}/Libraries/CMSIS/Device/Maxim/MAX32670/Include
    ${MAXIM_DRIVERS}/Libraries/CMSIS/Include
    ${MAXIM_DRIVERS}/Libraries/Boards/MAX32670
    ${HEADER_DIRS}
)

target_link_directories(max32670 INTERFACE
)

target_link_libraries(max32670 INTERFACE
)
