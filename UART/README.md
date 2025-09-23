# UART Test in Verilog

This is a Verilog HDL design that demonstrates a UART loopback test on the Atum A3 Nano board.

## Description

This design implements a simple UART loopback test. When you type characters into a serial terminal on a PC (e.g., PuTTY) with a baud rate of 115200, the characters are sent to the FPGA. The FPGA then immediately transmits the same characters back to the PC, echoing them in the terminal window. The **KEY0** push-button can be used to interrupt the loopback connection to verify its function.

## Project Details

- **Family**: Agilex 3
- **Quartus Version**: 25.1
- **Development Kit**: Atum A3 Nano
- **Device Part**: A3CZ135BB18AE7S
- **Category**: Communication, UART, Demonstration
- **URL**: https://github.com/terasic/Atum-A3-Nano/tree/rel/25.1/UART
- **Design Package**: Atum_A3_Nano_UART.zip
