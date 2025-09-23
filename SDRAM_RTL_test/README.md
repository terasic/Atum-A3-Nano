# SDRAM Test in Verilog

This is a Verilog HDL design example that tests the 64MB SDRAM on the Atum A3 Nano board.

## Description

This design verifies the functionality of the onboard SDRAM. The SDRAM controller uses a 50 MHz clock to generate a 125MHz memory clock. A test module writes a sequence to the entire memory, then reads the data back to verify its integrity.

To run the test, you press the **KEY0** push-button. The LEDs will indicate the test status. After pressing and releasing KEY0, LED1 and LED0 will blink. A successful test is indicated when LED0 stops blinking and remains ON after approximately 6 seconds.

***

## Project Details

- **Family**: Agilex 3
- **Quartus Version**: 25.1
- **Development Kit**: Atum A3 Nano
- **Device Part**: A3CZ135BB18AE7S
- **Category**: Memory, SDRAM, Demonstration
- **URL**: https://github.com/terasic/Atum-A3-Nano/tree/rel/25.1/SDRAM_RTL_test
- **Design Package**: Atum_A3_Nano_SDRAM_RTL_test.zip
