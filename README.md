# Energy Board Support

This repository contains files to abstract the board details and MCU drivers used for the application. This includes functions to initialize the board and configure required peripherals such as SPI. Refer to the header files in the [include](generic/include/) folder for prototypes of functions implemented by this module.

Note that these codes use HALs and MCU drivers provided by MCU vendors beneath the API layer. This expects appropriate drivers to be included as part of the application. The intention is to facilitate easy development of examples targeting multiple boards. For normal application development targeted only to one MCU, it would be more efficient to directly use the APIs provided by the MCU vendor.

Repository structure:

```
<mcu_family>/include         - Common include files for boards with <mcu_family>
<mcu_family>/source          - Common source files for boards with <mcu_family>
<mcu_family>/<board_name>    - Board-specific includes and sources
<dummy_board>                - Dummy implementation without hardware dependencies
```