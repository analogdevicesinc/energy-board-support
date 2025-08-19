cmake_minimum_required(VERSION 3.22)

project(dummy_board C)

set(DUMMY_BOARD_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Board source files
set(BOARD_SRC
    ${DUMMY_BOARD_ROOT_DIR}/adi_evb.c
)

# Create board source library
add_library(board_src INTERFACE)
target_sources(board_src INTERFACE ${BOARD_SRC})

# Main target
add_library(dummy_board INTERFACE)
target_include_directories(dummy_board INTERFACE
    ${DUMMY_BOARD_ROOT_DIR}/config
    ${DUMMY_BOARD_ROOT_DIR}/../generic/include
)
target_link_directories(dummy_board INTERFACE)
target_link_libraries(dummy_board INTERFACE board_src)
