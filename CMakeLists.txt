cmake_minimum_required(VERSION 3.22)

project(board_support C)

add_library(board_support INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

set(BSP_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

option(USE_FREERTOS "CompileFreeRTOS sources" OFF)
option(ZEPHYR_BUILD "Build app with Zephyr" OFF)
option(USE_IIO "Compile IIO board_support sources" OFF)

#Needed to print variables in cmake
include(CMakePrintHelpers)
if(EVB MATCHES "app_mcu_h5")
	add_subdirectory(${BSP_ROOT_DIR}/stm/app_mcu_h5
		 			 ${CMAKE_BINARY_DIR}/app_mcu_h5)
	target_link_libraries(board_support INTERFACE app_mcu_h5)
elseif(EVB MATCHES "app_mcu_h7")
	add_subdirectory(${BSP_ROOT_DIR}/stm/app_mcu_h7
		 			 ${CMAKE_BINARY_DIR}/app_mcu_h7)
	target_link_libraries(board_support INTERFACE app_mcu_h7)
elseif(EVB MATCHES "nucleo_h563zi")
	add_subdirectory(${BSP_ROOT_DIR}/stm/nucleo_h563zi_master
		 			 ${CMAKE_BINARY_DIR}/nucleo_h563zi)
	target_link_libraries(board_support INTERFACE nucleo_h563zi)
elseif(EVB MATCHES "eval_ade9178")
	add_subdirectory(${BSP_ROOT_DIR}/max/eval_ade9178
		 			 ${CMAKE_BINARY_DIR}/eval_ade9178)
	target_link_libraries(board_support INTERFACE eval_ade9178)
else()
	add_subdirectory(${BSP_ROOT_DIR}/stm/nucleo_h563zi_master
		 			 ${CMAKE_BINARY_DIR}/nucleo_h563zi)
	target_link_libraries(board_support INTERFACE nucleo_h563zi)
endif()

if(USE_FREERTOS)
	add_subdirectory(${BSP_ROOT_DIR}/rtos)
	target_link_libraries(board_support INTERFACE rtos)
	set(GENERIC_SRC ${BSP_ROOT_DIR}/rtos/source/evb_freertos.c)
	set(RTOS_INCLUDE ${BSP_ROOT_DIR}/rtos/include)
elseif(ZEPHYR_BUILD)
	set(GENERIC_SRC ${BSP_ROOT_DIR}/stm/nucleo_h563zi_master/source/zephyr_delay.c)
else()
	set(GENERIRTOS_INCLUDEC_INCLUDE "")
	if(EVB MATCHES "nucleo_h563zi" OR EVB MATCHES "app_mcu_h5" OR EVB MATCHES "app_mcu_h7")
		set(GENERIC_SRC ${BSP_ROOT_DIR}/stm/source/stm_delay.c)
	endif()
endif()

target_include_directories(board_support INTERFACE
			${BSP_ROOT_DIR}/generic/include
			${RTOS_INCLUDE}
)

message("Just a bunch of debug statements:")
cmake_print_variables(EVB)
cmake_print_variables(GENERIC_SRC)
target_sources(board_support INTERFACE
	${GENERIC_SRC}
	${BSP_ROOT_DIR}/generic/source/message.c
)
