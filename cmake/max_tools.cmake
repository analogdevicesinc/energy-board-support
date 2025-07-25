#Update this based on instalation path
set(ARM_GCC_TOOLCHAIN_PATH "${SDK}/Tools/GNUTools/10.3/bin" CACHE PATH "ARm Gcc Toolchain path")
set(ARM_CLANG_TOOLCHAIN_PATH "C:/Keil_v5/ARM/ARMCLANG/Bin" CACHE PATH "Keil Toolchain path")
if(TOOLCHAIN MATCHES "gcc-arm-none-eabi")
    set(TOOLCHAIN_PATH ${ARM_GCC_TOOLCHAIN_PATH} CACHE PATH "Toolchain Path")
elseif(TOOLCHAIN MATCHES "armclang")
set(TOOLCHAIN_PATH ${ARM_CLANG_TOOLCHAIN_PATH} CACHE PATH "Toolchain Path")
endif()
set(SDK C:/MaximSDK CACHE PATH "Path to MAXIM_DRIVERS directory")