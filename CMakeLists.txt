cmake_minimum_required(VERSION 3.22)

project(board_support C)

add_library(board_support INTERFACE)

set(BSP_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

option(USE_FREERTOS "Compile FreeRTOS sources" OFF)
option(ZEPHYR_BUILD "Build app with Zephyr" OFF)
option(USE_SIMUL_ADC "Compile simul ADC sources" OFF)

# Print variables for debugging
include(CMakePrintHelpers)

# Select board and add subdirectory
if(EVB MATCHES "app_mcu_h5")
    add_subdirectory(${BSP_ROOT_DIR}/stm/app_mcu_h5 ${CMAKE_BINARY_DIR}/app_mcu_h5)
    target_link_libraries(board_support INTERFACE app_mcu_h5)
elseif(EVB MATCHES "app_mcu_h7")
    add_subdirectory(${BSP_ROOT_DIR}/stm/app_mcu_h7 ${CMAKE_BINARY_DIR}/app_mcu_h7)
    target_link_libraries(board_support INTERFACE app_mcu_h7)
elseif(EVB MATCHES "nucleo_h563zi")
    add_subdirectory(${BSP_ROOT_DIR}/stm/nucleo_h563zi_master ${CMAKE_BINARY_DIR}/nucleo_h563zi)
    target_link_libraries(board_support INTERFACE nucleo_h563zi)
elseif(EVB MATCHES "nucleo_h7s3l8")
    # Add bootloader and application for nucleo_h7s3l8
    add_subdirectory(${BSP_ROOT_DIR}/stm/nucleo_h7s3l8/boot ${CMAKE_BINARY_DIR}/boot)
    add_subdirectory(${BSP_ROOT_DIR}/stm/nucleo_h7s3l8/app ${CMAKE_BINARY_DIR}/nucleo_h7s3l8)
    target_link_libraries(board_support INTERFACE nucleo_h7s3l8)

    # Create a custom target that ensures boot is built (generic name)
    add_custom_target(bsp_boot ALL DEPENDS boot)
    # Make board_support depend on the boot target so any target linking to board_support will build boot
    add_dependencies(board_support bsp_boot)
elseif(EVB STREQUAL "eval_ade9178")
    add_subdirectory(${BSP_ROOT_DIR}/max/eval_ade9178 ${CMAKE_BINARY_DIR}/eval_ade9178)
    target_link_libraries(board_support INTERFACE eval_ade9178)
elseif(EVB STREQUAL "eval_ade9178_slave")
    add_subdirectory(${BSP_ROOT_DIR}/max/eval_ade9178_slave ${CMAKE_BINARY_DIR}/eval_ade9178_slave)
    target_link_libraries(board_support INTERFACE eval_ade9178_slave)
elseif(EVB MATCHES "dummy_board")
    add_subdirectory(${BSP_ROOT_DIR}/dummy_board ${CMAKE_BINARY_DIR}/dummy_board)
    target_link_libraries(board_support INTERFACE dummy_board)
elseif(EVB MATCHES "x86")
    add_subdirectory(${BSP_ROOT_DIR}/simul_x86 ${CMAKE_BINARY_DIR}/simul_x86)
    target_link_libraries(board_support INTERFACE x86)
else()
    add_subdirectory(${BSP_ROOT_DIR}/stm/nucleo_h563zi_master ${CMAKE_BINARY_DIR}/nucleo_h563zi)
    target_link_libraries(board_support INTERFACE nucleo_h563zi)
endif()

# RTOS and generic sources
if(USE_FREERTOS)
    add_subdirectory(${BSP_ROOT_DIR}/rtos)
    target_link_libraries(board_support INTERFACE rtos)
    set(GENERIC_SRC ${BSP_ROOT_DIR}/rtos/source/evb_freertos.c)
    set(RTOS_INCLUDE ${BSP_ROOT_DIR}/rtos/include)
elseif(ZEPHYR_BUILD)
    set(GENERIC_SRC ${BSP_ROOT_DIR}/stm/nucleo_h563zi_master/source/zephyr_delay.c)
    set(RTOS_INCLUDE "")
else()
    set(RTOS_INCLUDE "")
    if(EVB MATCHES "nucleo_h563zi" OR EVB MATCHES "app_mcu_h5" OR EVB MATCHES "app_mcu_h7" OR EVB MATCHES "nucleo_h7s3l8")
        set(GENERIC_SRC ${BSP_ROOT_DIR}/stm/source/stm_delay.c)
    else()
        set(GENERIC_SRC "")
    endif()

endif()
if(USE_SIMUL_ADC)
    add_subdirectory(${BSP_ROOT_DIR}/adc_model)
    target_link_libraries(board_support INTERFACE adc_model)
endif()

target_include_directories(board_support INTERFACE
    ${BSP_ROOT_DIR}/generic/include
    ${RTOS_INCLUDE}
)

target_sources(board_support INTERFACE
    ${GENERIC_SRC}
    ${BSP_ROOT_DIR}/generic/source/message.c
)

# Board-specific post-processing handled in individual board CMakeLists.txt files

# Generic function to apply board-specific linker scripts to executables
function(apply_board_linker_script target_name)
    if(EVB MATCHES "nucleo_h7s3l8")
        # Apply H7S3L8 app linker script to the target
        apply_h7s3l8_app_linker_script(${target_name})
    endif()
endfunction()
