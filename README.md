# Customized Bootloader for STM32F446RE

This repository contains a customized bootloader for STM32F446RE microcontrollers. The bootloader provides a reliable and efficient method for updating and enhancing firmware on the microcontroller using the USART communication protocol.

## Features

- USART communication protocol: The bootloader leverages the USART protocol for seamless communication between the microcontroller and the host device, enabling firmware updates and enhancements.
- Customization: The bootloader has been tailored specifically for the STM32F446RE microcontroller, ensuring optimal performance and compatibility.
- Firmware Updates: The bootloader enables users to update the firmware of the microcontroller, allowing for bug fixes, new features, and improved functionality.


## Getting Started

To get started with the bootloader, follow these steps:

1. **Hardware Setup**: Ensure that the STM32F446RE microcontroller is properly connected to the host device via USART.
2. **Flash the Bootloader**: Use a suitable tool or IDE to flash the bootloader onto the microcontroller.
3. **Host Device Configuration**: Configure the host device to establish a USART connection with the microcontroller.


## Usage

The bootloader supports the following commands:

1. Get Bootloader's version: `BL_GET_VER`
2. Get help: `BL_GET_HLP`
3. Get Chip ID: `BL_GET_VER`
4. Get RDP status: `BL_GET_RDP_STATUS`
5. Go to a desired address: `BL_GO_TO_ADDR`
6. Flash erase (desired sectors/ mass erase): `BL_FLASH_ERASE`
7. Write in a desired address memory: `BL_MEM_WRITE`
8. Enable RW protection on sectors: `BL_EN_R_W_PROTECT`
9. Disable RW protection on sectors: `BL_DIS_R_W_PROTECT`
10. Get sectors' protection status: `BL_READ_SECTOR_P_STATUS`


## Contributing

Contributions to this project are welcome! If you encounter any bugs, have suggestions for improvements, or would like to contribute new features, please feel free to submit a pull request or open an issue.
