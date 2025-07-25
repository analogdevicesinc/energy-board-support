# Copyright (c) 2025  Analog Devices Inc

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_ID ARMClang)
set(CMAKE_CXX_COMPILER_ID ARMClang)

# Set compilers
set(TOOLCHAIN_PREFIX "${TOOLCHAIN_PATH}/")
set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}armclang.exe")
#armasm.exe is deprecated. It is recommended to use armclang as compiler and assembler.
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}armasm.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}armclang.exe")
set(CMAKE_LINKER "${TOOLCHAIN_PREFIX}armlink.exe")
set(CMAKE_OBJCOPY "${TOOLCHAIN_PREFIX}fromelf.exe")
set(CMAKE_SIZE "${TOOLCHAIN_PREFIX}fromelf.exe")

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".axf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".axf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".axf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --target=arm-arm-none-eabi -mthumb -masm=auto -Wa,armasm,--diag_suppress=A1950W -Wall -Wextra -Wpedantic -ffunction-sections -fdata-sections -c")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

set(CMAKE_C_FLAGS_RELEASE  "-O3 -DNDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-rtti -fno-exceptions")

set(CMAKE_ASM_FLAGS "${CMAKE_LINK_FLAGS} ${CMAKE_ASM_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --scatter ${LD_FILE}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --map --info=totals --info=sizes")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --libpath=C:/Keil_v5/ARM/ARMCLANG/lib --userlibpath=path/to/userlibs")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --start-group -lstdc++ -lsupc++ --end-group")

macro(add_hex_generation target)
    add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Generated HEX file: ${CMAKE_CURRENT_BINARY_DIR}/${target}.hex"
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${target}.axf ${target}.hex
    )
endmacro()

macro(add_hex_generation target)
    add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Generated HEX file: ${CMAKE_CURRENT_BINARY_DIR}/${target}.hex"
        COMMAND ${CMAKE_OBJCOPY} --i32 --output=${CMAKE_CURRENT_BINARY_DIR}/${target}.hex ${CMAKE_CURRENT_BINARY_DIR}/${target}.axf
    )
endmacro()