#Update this if required. GCC toolchain is normally added to PATH during installation of MinGW
set(TOOLCHAIN_PATH C:/MinGW/bin CACHE PATH "Toolchain path")
macro(add_hex_generation target)
    add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${target}.exe ${target}.hex
    )
endmacro()