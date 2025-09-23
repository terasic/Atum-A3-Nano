# HDMI_out RTL in Verilog

This is a Verilog HDL design example that demonstrates HDMI video output on the Atum A3 Nano board.

## Description

This design generates a 1080p@60 video color bar test pattern and outputs it to a display via the HDMI port. The FPGA uses a PLL to generate the required 148.5MHz pixel clock from a 50MHz input clock. The design also includes an I2C controller to configure the external TFP410 HDMI transmitter chip, enabling it for normal operation. A video pattern generator module creates the color bar image and timing signals, which are then sent to the HDMI chip and displayed on the monitor.

## Project Details

- **Family**: Agilex 3 
- **Quartus Version**: 25.1
- **Development Kit**: Atum A3 Nano 
- **Device Part**: A3CZ135BB18AE7S 
- **Category**: Video, HDMI, Demonstration
- **URL**: https://github.com/terasic/Atum-A3-Nano/tree/rel/25.1/HDMI_out_RTL
- **Design Package**: Atum_A3_Nano_HDMI_out_RTL.zip
