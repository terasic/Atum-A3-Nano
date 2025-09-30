# Picture Viewer 1080p

A Nios V processor-based design example demonstrating how to sequentially display bitmap image files from a Micro SD card on an HDMI monitor.

## Description

The demo code is built around RTL code and a Nios V processor to implement the SD card controller and file system. The Nios V application scans the SD card, identifies bitmap files that meet display criteria, and generates a display list. From this list, it selects a file, decodes it, and stores the RGB image data into SDRAM before signaling the display subsystem.

The display subsystem leverages the Altera Video and Image Processing Suite. Using the Frame Reader IP, it retrieves the image data from SDRAM, then employs the Clocked Video Output and Full Rate to Clocked Video Converter IPs to prepare the data for the HDMI transmitter (FTP410). The processed image is finally rendered on an HDMI monitor.

Note :  The photos placed in the SD Card must be BMP files with a resolution of 1280x720.

## Project Details

- **Family**: Agilex 3
- **Quartus Version**: 25.1
- **Development Kit**: Atum A3 Nano
- **Device Part**: A3CZ135BB18AE7S
- **Category**: Video, HDMI, Storage, SD Card, Nios V, Demonstration
- **URL**: https://github.com/terasic/Atum-A3-Nano/tree/rel/25.1/pricture_viewer_1080p
- **Design Package**: Atum_A3_Nano_picture_viewer_1080p.zip
