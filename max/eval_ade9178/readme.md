# EVAL-ADE9178 :Build & Run Instructions


## Install Required Tools

   - **CMake:** Download and install from the [CMake website](https://cmake.org/download/).
   - **Ninja:** Download from the [Ninja releases page](https://github.com/ninja-build/ninja/releases).
   - **MAX3267x SDK:**
      - Download the [automatic installer](https://analogdevicesinc.github.io/msdk//USERGUIDE/#installation) for the MAX3267x SDK.
      - Follow the installation instructions in the user guide and configure any required environment variables (such as the `SDK` path).

   -  USB-UART driver [CP2102, Universal Windows Driver](https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip)
  

## Building the Project
   - Open a terminal and navigate to the firmware project directory where `CMakeLists.txt` is present.

     ```sh
     cmake -S . -B build/Release -G "Ninja" -DSDK=<Path to MAX3267x SDK>
     # For example: -DSDK=C:/MaximSDK/
     ```
   - Compile the project:
     ```sh
     cmake --build build/Release
     ```
   - The output `.hex` file will be generated in the `build/Release` directory.


## Flashing the Hex File

To flash the generated `.hex` file to the MAX32670 board, use a MAX32625PICO debug adapter and OpenOCD.

1. **Connect the Debugger:**
   - Attach the MAX32625PICO ribbon cable to the SWD header on the EVAL-ADE9178 board.
   - Connect the MAX32625PICO to your PC via USB. A DAPLINK drive should appear in your file explorer.


2. **Flash the Firmware:**
   - Open a command prompt.
   - Run the following command:
     ```sh
     "<MSDK Path>/Tools/OpenOCD/openocd.exe" -s "<MSDK Path>/Tools/OpenOCD/scripts" -f interface/cmsis-dap.cfg -f target/max32670.cfg -c "program \"<path_to_your_firmware.hex\" reset exit"
     ```
   - Replace `<MSDK Path>` and `<path_to_your_firmware.hex>` with the actual paths on your system.

## Communicating with the Eval Board

The  Board features an MAX32670 MCU that communicates with a PC via UART. To establish this UART connection:

   - Connect a micro-USB cable to the board's Micro-USB port VDD_USB.
   - Connect the other end of the micro-USB cable to your PC.
   - Ensure that jumpers P3 and P4 are populated on the board.
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
2. If it does not, open the application's `CMakeLists.txt`, then open the command palette (Ctrl + Shift + P), search for `CMake: Select Configure Preset`, and choose the appropriate preset.
3. Use the command palette to run `Tasks: Run Task` and select the desired build, clean, or flash task.
4. Open the "Run and Debug" sidebar in VS Code.
5. Click "Start Debugging" to launch the debugger and step through your code or reset the device as needed.


## References
- [ADE9178 Product Page](https://www.analog.com/en/products/ade9178.html)
- [MAX32670 Product Page](https://www.maximintegrated.com/en/products/microcontrollers/MAX32670.html)
- [MAX32625PICO](https://www.analog.com/en/resources/evaluation-hardware-and-software/evaluation-boards-kits/max32625pico.html#eb-documentation)