cmake_minimum_required(VERSION 3.22)
add_definitions(-DTARGET_REV=0x4131)
add_definitions(-DTARGET=MAX32670)

project(eval_ade9178 C)

option(SUPPRESS_MSDK_WARNINGS "Suppress warnings for Maxim SDK sources" ON)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

set(MAX32670_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Define SDK path
set(SDK C:/MaximSDK CACHE PATH "Path to MAXIM_DRIVERS directory")
message(STATUS "Using Maxim SDK path: ${SDK}")

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
file(GLOB_RECURSE DEVICE_SOURCES ${SDK}/Libraries/CMSIS/Device/Maxim/Source/*.c)

set(MAXIM_FILES    # HAL driver sources
        ${SDK}/Libraries/CMSIS/Device/Maxim/MAX32670/Source/GCC/startup_max32670.s
        ${SDK}/Libraries/CMSIS/Device/Maxim/MAX32670/Source/system_max32670.c
        ${SDK}/Libraries/CMSIS/Device/Maxim/MAX32670/Source/heap.c
        ${SDK}/Libraries/PeriphDrivers/Source/SYS/mxc_assert.c
        ${SDK}/Libraries/PeriphDrivers/Source/SYS/mxc_delay.c
        ${SDK}/Libraries/PeriphDrivers/Source/SYS/mxc_lock.c
        ${SDK}/Libraries/PeriphDrivers/Source/SYS/nvic_table.c
        ${SDK}/Libraries/PeriphDrivers/Source/SYS/pins_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/SYS/sys_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/AES/aes_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/AES/aes_revb.c
        ${SDK}/Libraries/PeriphDrivers/Source/DMA/dma_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/DMA/dma_reva.c
        ${SDK}/Libraries/PeriphDrivers/Source/GPIO/gpio_common.c
        ${SDK}/Libraries/PeriphDrivers/Source/GPIO/gpio_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/GPIO/gpio_reva.c
        ${SDK}/Libraries/PeriphDrivers/Source/SPI/spi_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/SPI/spi_reva1.c
        ${SDK}/Libraries/PeriphDrivers/Source/TMR/tmr_common.c
        ${SDK}/Libraries/PeriphDrivers/Source/TMR/tmr_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/UART/uart_common.c
        ${SDK}/Libraries/PeriphDrivers/Source/UART/uart_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/UART/uart_revb.c
        ${SDK}/Libraries/PeriphDrivers/Source/UART/uart_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/UART/uart_revb.c
        ${SDK}/Libraries/PeriphDrivers/Source/FLC/flc_common.c
        ${SDK}/Libraries/PeriphDrivers/Source/FLC/flc_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/FLC/flc_reva.c
        ${SDK}/Libraries/PeriphDrivers/Source/ICC/icc_common.c
        ${SDK}/Libraries/PeriphDrivers/Source/ICC/icc_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/ICC/icc_reva.c
        ${SDK}/Libraries/PeriphDrivers/Source/TMR/tmr_common.c
        ${SDK}/Libraries/PeriphDrivers/Source/TMR/tmr_me15.c
        ${SDK}/Libraries/PeriphDrivers/Source/TMR/tmr_revb.c
        ${DEVICE_SOURCES}
)

file(GLOB HEADER_DIRS LIST_DIRECTORIES true ${SDK}/Libraries/PeriphDrivers/Source/*)

# Create a library for Maxim SDK files and suppress warnings
add_library(maxim_sdk INTERFACE)
target_sources(maxim_sdk INTERFACE ${MAXIM_FILES})

if (SUPPRESS_MSDK_WARNINGS)
    # Suppress warnings for Maxim SDK
    target_compile_options(maxim_sdk INTERFACE -w)
endif()

# Create a library for your board source files (warnings enabled)
add_library(board_src INTERFACE)
target_sources(board_src INTERFACE ${BOARD_SRC})

# Main target
add_library(eval_ade9178 INTERFACE)
target_link_libraries(eval_ade9178 INTERFACE maxim_sdk board_src)

target_include_directories(eval_ade9178 INTERFACE
    ${MAX32670_ROOT_DIR}/include
    ${MAX32670_ROOT_DIR}/../include
    ${MAX32670_ROOT_DIR}/include/config
    ${SDK}/Libraries/PeriphDrivers/Include/MAX32670
    ${SDK}/Libraries/CMSIS/5.9.0/Core/Include
    ${SDK}/Libraries/CMSIS/Device/Maxim/MAX32670/Include
    ${SDK}/Libraries/CMSIS/Include
    ${SDK}/Libraries/Boards/MAX32670
    ${HEADER_DIRS}
)
target_compile_definitions(eval_ade9178 INTERFACE TARGET_REV=0x4131 TARGET=MAX32670)

target_link_directories(eval_ade9178 INTERFACE
)

target_link_libraries(eval_ade9178 INTERFACE
)
