# EVAL-ADE9178

## Hardware Connections

The EVAL-ADE9178 evaluation kit uses a MAX32670 Host MCU, which communicates with your PC via UART. To set up the hardware:

1. **Connect Power and UART:**
   - Use a micro-USB cable to connect your PC to the board's Micro-USB port (labelled VDD_USB).
   - Ensure that jumpers P3 and P4 are populated on the board.

2. **Install USB-UART Driver:**
   - Download and install the CP2102 USB-UART driver from the [official Silicon Labs website](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers).
   - Follow the installation instructions provided on the website.

3. **Verify Connection:**
   - After connecting, your PC should detect a new COM port (check Device Manager on Windows).


## Building the Firmware

1. **Install Required Tools:**
   - **CMake:** Download and install CMake from the [official website](https://cmake.org/download/).

   - Clone or download the [MAX3267x SDK (v2024_10)](https://github.com/analogdevicesinc/msdk/tree/v2024_10) from GitHub.
   - Follow the SDK's installation instructions and set up the required environment 

3. **Configure the Build:**
   - Open a terminal and navigate to the firmware project directory (e.g., [ade9178 example]()).

     ```sh
     cmake -S . -B build\Release -G "Ninja" -DSDK=<Path to MAX32670 SDK>
     #For ex, -DSDK=C:\MAximSDK\

     ```

4. **Build the Firmware:**
   - Compile the project:
     ```sh
     cmake --build build\Release

     ```
   - The output `.hex` file will be generated in the build directory.


## Programming the Firmware onto the Board

You can flash the generated `.hex` file to the MAX32670 board using a MAX32625PICO debug adapter and OpenOCD.

1. **Connect the Debugger:**
   - Attach the MAX32625PICO ribbon cable to the SWD header on the EVAL-ADE9178 board.
   - Connect the MAX32625PICO to your PC via USB.

2. **Get the Debug Adapter Unique ID:**
   - After connecting, a DAPLINK drive should appear in File Explorer.
   - Open `DETAILS.TXT` on the DAPLINK drive and copy the Unique ID.

3. **Flash the Hex File:**
   - Open a terminal or command prompt.
   - Run the following command, replacing `<YOUR_UNIQUE_ID>` and the path to your `.hex` file:
     ```sh
     "C:\MaximSDK\Tools\OpenOCD\openocd.exe" -s "C:\MaximSDK\Tools\OpenOCD\scripts" -f interface\cmsis-dap.cfg -f target\max32670.cfg -c "cmsis_dap_serial <YOUR_UNIQUE_ID>; program \"C:/path/to/your/firmware.hex\" reset exit"
     ```

---

### Debugging with VS Code Workspace

The example projects may come with  a pre-configured VS Code workspace for easy building and debugging.

1. The default CMake preset is loaded automatically when you open the workspace.
2. If not, open the command palette, search for `CMake: Select Configure Preset`, and choose the appropriate preset.
3. Use the command palette to run `Tasks: Run Task` and select the desired build, clean, or flash task.
4. Open the "Run and Debug" sidebar in VS Code.
5. Click "Start Debugging" to launch the debugger and step through your code or reset the device as needed.


## References
- [ADE9178 Product Page](https://www.analog.com/en/products/ade9178.html)
- [MAX32670 Product Page](https://www.maximintegrated.com/en/products/microcontrollers/MAX32670.html)
- [MAX3267x SDK GitHub](https://github.com/analogdevicesinc/msdk/tree/v2024_10)
- [MAX32625PICO](https://www.analog.com/en/resources/evaluation-hardware-and-software/evaluation-boards-kits/max32625pico.html#eb-documentation)