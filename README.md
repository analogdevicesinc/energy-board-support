# Energy Board Support

This directory provides an abstraction layer for board-specific details and MCU drivers used in the application. It includes functions to initialize the board and configure required peripherals such as SPI. Function prototypes can be found in the [include](generic/include/) folder.

The implementation relies on hardware abstraction layers (HALs) and MCU drivers supplied by the MCU vendor. You must ensure that the appropriate drivers are included in your application. This board support package is designed to make it easy to develop examples that can run on multiple boards. For applications targeting a single MCU, it may be more efficient to use the vendor's APIs directly.

The supported boards are 

 - ADI EVAL-ADEMA127 Board - [Readme](stm/app_mcu_h5/readme.md)
 - ADI EVAL-ADE9178 Board  - [Readme](max/eval_ade9178/readme.md)

## Repository Structure

```
<mcu_family>/include         - Common header files for boards using <mcu_family>
<mcu_family>/source          - Common source files for boards using <mcu_family>
<mcu_family>/<board_name>    - Board-specific headers and sources
<dummy_board>                - Dummy implementation with no hardware dependencies
```