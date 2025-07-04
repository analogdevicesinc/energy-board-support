cmake_minimum_required(VERSION 3.22)

project(board_support C)

add_library(board_support INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

set(BSP_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

option(USE_FREERTOS "CompileFreeRTOS sources" OFF)
option(ZEPHYR_BUILD "Build app with Zephyr" OFF)
option(USE_IIO "Compile IIO board_support sources" OFF)

## Add project symbols (macros)
if(BUILD_TARGET MATCHES "stm32h573zi")
	target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
		STM32H573xx
		$<$<CONFIG:Debug>:DEBUG>
	)
elseif(BUILD_TARGET MATCHES "stm32h735ig")
	target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
		STM32H735xx
		$<$<CONFIG:Debug>:DEBUG>
	)
endif()

#Needed to print variables in cmake
include(CMakePrintHelpers)

if(BUILD_TARGET MATCHES "stm32h573zi")
	add_subdirectory(${BSP_ROOT_DIR}/stm/app_mcu_h5
		 			 ${CMAKE_BINARY_DIR}/stm32h573zi)
	target_link_libraries(board_support INTERFACE stm32h573zi)
elseif(BUILD_TARGET MATCHES "stm32h735ig")
	add_subdirectory(${BSP_ROOT_DIR}/stm/app_mcu_h7
		 			 ${CMAKE_BINARY_DIR}/stm32h735ig)
	target_link_libraries(board_support INTERFACE stm32h735ig)
elseif(BUILD_TARGET MATCHES "stm32h563zi")
	add_subdirectory(${BSP_ROOT_DIR}/stm/nucleo_h563zi_master
		 			 ${CMAKE_BINARY_DIR}/stm32h563zi)
	target_link_libraries(board_support INTERFACE stm32h563zi)
elseif(BUILD_TARGET MATCHES "max32670")
	add_subdirectory(${BSP_ROOT_DIR}/max/eval_ade9178
		 			 ${CMAKE_BINARY_DIR}/max32670)
	target_link_libraries(board_support INTERFACE max32670)
else()
	add_subdirectory(${BSP_ROOT_DIR}/stm/nucleo_h563zi_master
		 			 ${CMAKE_BINARY_DIR}/stm32h563zi)
	target_link_libraries(board_support INTERFACE stm32h563zi)
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
	if(BUILD_TARGET MATCHES "stm32h563zi" OR BUILD_TARGET MATCHES "stm32h573zi" OR BUILD_TARGET MATCHES "stm32h735ig")
		set(GENERIC_SRC ${BSP_ROOT_DIR}/stm/source/stm_delay.c)
	endif()
endif()

target_include_directories(board_support INTERFACE
			${BSP_ROOT_DIR}/generic/include
			${RTOS_INCLUDE}
)

message("Just a bunch of debug statements:")
cmake_print_variables(USE_IIO)
cmake_print_variables(BUILD_TARGET)
cmake_print_variables(GENERIC_SRC)
target_sources(board_support INTERFACE
	${GENERIC_SRC}
	${BSP_ROOT_DIR}/generic/source/message.c
)
