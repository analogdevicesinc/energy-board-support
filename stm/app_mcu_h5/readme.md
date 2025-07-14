# App MCU Board: Build & Run Instructions


## Install Required Tools
   - Download and install [STM32CubeCLT](https://www.st.com/en/development-tools/stm32cubeclt.html).
   - Download the STM32H5 HAL drivers from the [STM32H5 HAL Drivers](https://github.com/STMicroelectronics/STM32CubeH5).
   - Make sure the following directories are present in your system's PATH:
      - %STM32CLT_PATH%
      - %STM32CLT_PATH%\CMake\bin
      - %STM32CLT_PATH%\Ninja\bin
      - %STM32CLT_PATH%\GNU-tools-for-STM32\bin
      - %STM32CLT_PATH%\STLink-gdb-server\bin
      - %STM32CLT_PATH%\STM32CubeProgrammer\bin

      where STM32CLT_PATH = <Installation path of STM32CubeCLT> (e.g., C:\ST\STM32CubeCLT_1.16.0)

   - Install the required USB-UART driver [CP2102, Universal Windows Driver](https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip)
  
## Building the Project
   - Open a command prompt and navigate to the project directory.
   - Run the following commands (replace `<Path to STM32H5xx_HAL_Driver folder>` with the actual path):
     ```sh
      cmake -S . -B build/Release -G "Ninja" -DSDK=<Path to STM32H5xx_HAL_Driver folder> -DTOOLCHAIN_PATH=<Path to STM32CubeCLT>
     # For example: -DSDK=C:/STM32Cube/Repository/STM32Cube_FW_H5_V1.3.0/Drivers/STM32H5xx_HAL_Driver -DTOOLCHAIN_PATH=C:/ST/STM32CubeCLT_1.16.0/GNU-tools-for-STM32/bin

     cmake --build build/Release
     ```
   - The output `.hex` file will be generated in the `build/Release` directory.

## Flashing the Hex File

You can flash the generated `.hex` file to the APP MCU board using a hardware debugger such as [STLINK-V3SET](https://www.st.com/en/development-tools/stlink-v3set.html). The PC should detect it as an STLINK port.

1. **Connect the ST-Link Debugger:**
   - Connect the debugger’s SWD interface to the corresponding pins on the APP MCU Board.
   - Plug the debugger into your PC.

3. **Flash the Hex File:**
   - Use the STM32CubeProgrammer GUI to flash the `.hex` file.
   - Alternatively, use the command line to load firmware using STM32CubeProgrammer:
     ```sh
     STM32_Programmer_CLI.exe -c port=SWD freq=4000 -d <path to .hex file> -g
     ```

## Communicating with the Board

The APP MCU Board features an STM32H573ZIT6 MCU that communicates with a PC via UART. To establish this UART connection:

   - Connect a micro-USB cable to the App MCU board's Micro-USB port.
   - Connect the other end of the micro-USB cable to your PC.
   - After connecting, your PC should detect a new COM port (check Device Manager on Windows).
   - Use a terminal application such as PuTTY or Tera Term.
   - Configure the connection with the following parameters:
     ```
     Baud rate    : 115200
     Data bits    : 8
     Parity       : None
     Stop bits    : 1
     Flow control : None
     ```


## Debugging with VS Code Workspace

The example projects may come with a pre-configured VS Code workspace for easy building and debugging.

1. The default CMake preset is loaded automatically when you open the workspace.
2. If it does not, open the application's CMakeLists.txt, then open the command palette (Ctrl + Shift + P), search for `CMake: Select Configure Preset`, and choose the appropriate preset.
3. Use the command palette to run `Tasks: Run Task` and select the desired build, clean, or flash task.
4. Open the "Run and Debug" sidebar in VS Code.
5. Click "Start Debugging" to launch the debugger and step through your code or reset the device as needed.


## References
- [STM32H563/573 Product Page](https://www.st.com/en/microcontrollers-microprocessors/stm32h563-573.html)
- [STM32CubeProgrammer software](https://www.st.com/en/development-tools/stm32cubeprog.html)
