# SD Card Demonstration

A Nios V processor-based design example demonstrating how to read from and write to an SD card on the Atum A3 Nano.

## Description

This demonstration showcases file I/O operations on a FAT32-formatted SD card. The hardware design is created in Platform Designer and features a Nios V soft processor that runs the file system logic. A custom `terasic_sdio` component handles the low-level communication with the SD card.

The Nios V software uses the open-source FatFs library to manage the file system. When executed, the demo first attempts to find and read a file named `readme.txt` from the card, displaying its contents in the Nios V terminal. Afterwards, it creates a new file, `create.txt`, and writes the text "Write Test!" into it.

## Project Details

- **Family**: Agilex 3
- **Quartus Version**: 25.1
- **Development Kit**: Atum A3 Nano
- **Device Part**: A3CZ135BB18AE7S
- **Category**: Storage, SD Card, Nios V, Demonstration
- **URL**: https://github.com/terasic/Atum-A3-Nano/tree/rel/25.1/sdcard_filesystem
- **Design Package**: Atum_A3_Nano_sdcard_filesystem.zip
