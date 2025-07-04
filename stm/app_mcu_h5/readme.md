# APPLICATION MCU Board

## Communicating with the APP MCU Board

The APP MCU Board features an STM32H573ZIT6 MCU that communicates with a PC via UART. To establish this UART connection:

1. **Connect Power and UART:**
   - Connect a micro-USB cable to the board's Micro-USB port.
   - Install the required USB-UART driver (CP2102, Universal Windows Driver). [Download the driver and follow installation instructions from the official website.](https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip)
   - Connect the other end of the micro-USB cable to your PC.
   - After connecting, your PC should detect a new COM port (check Device Manager on Windows).

## Installation
To build and use the source code in this repository for communicating with the APP MCU Board, ensure you have the following tools installed:

- [CMake](https://cmake.org/download/)
- [Ninja Build System](https://ninja-build.org/)
- [STM32H5 GitHub Repository V1.3.0](https://github.com/STMicroelectronics/STM32CubeH5)
- [STMCubeCLT v1.16.0](https://www.st.com/en/development-tools/stm32cubeclt.html)
- [STMCubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html)

## Building the Project

1. **Install Required Tools:**
   - Download and install CMake and Ninja, and add them to your system PATH.
   - Download and install STM32CubeCLT and ensure the STM32H5 HAL drivers are available.

2. **Generate the Binary:**
   - Open a command prompt and navigate to the project directory.
   - Run the following commands (replace `<Path to STM32H5xx_HAL_Driver folder>` with the actual path):
     ```sh
     cmake -S . -B build\Release -G "Ninja" -DSDK=<Path to STM32H5xx_HAL_Driver folder>
     # For example:
     # -DSDK=C:\STM32Cube\Repository\STM32Cube_FW_H5_V1.3.0\Drivers\STM32H5xx_HAL_Driver
     cmake --build build\Release
     ```
   - The output `.hex` file will be generated in the `build\Release` directory.

## Flashing the Hex File

You can flash the generated `.hex` file to the APP MCU board using a hardware debugger such as [STLINK-V3SET](https://www.st.com/en/development-tools/stlink-v3set.html). The PC should detect it as an STLINK port.

1. **Connect the ST-Link Debugger:**
   - Connect the debugger’s SWD interface to the corresponding pins on the APP MCU Board.
   - Plug the debugger into your PC.

2. **Flash the Hex File:**
   - Use STM32CubeProgrammer GUI to flash the `.hex` file.
   - Alternatively, use the command line to load firmware using STMCubeProg:
     ```sh
     STM32_Programmer_CLI.exe -c port=SWD freq=4000 -d <path to .hex file> -g
     ```

### Debugging with VS Code Workspace

The example projects may come with  a pre-configured VS Code workspace for easy building and debugging.

1. The default CMake preset is loaded automatically when you open the workspace.
2. If not, open the command palette, search for `CMake: Select Configure Preset`, and choose the appropriate preset.
3. Use the command palette to run `Tasks: Run Task` and select the desired build, clean, or flash task.
4. Open the "Run and Debug" sidebar in VS Code.
5. Click "Start Debugging" to launch the debugger and step through your code or reset the device as needed.


## References
- [STM32H563/573 Product Page](https://www.st.com/en/microcontrollers-microprocessors/stm32h563-573.html)
- [STM32CubeProgrammer software](https://www.st.com/en/development-tools/stm32cubeprog.html)
