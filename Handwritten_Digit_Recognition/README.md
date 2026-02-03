# Handwritten Digit Recognition

This is a Verilog HDL design that demonstrates handwritten digit recognition using a CNN accelerator on the Atum A3 Nano board.

## Description

This design implements a real-time handwritten digit classification system (0–9). Images captured from a D8M camera module are processed by an FPGA-based CNN hardware accelerator for low-latency edge AI inference. The demo leverages **ONE WARE Studio** for model training and hardware IP generation from an ONNX model.

## Project Details

- **Family**: Agilex 3
- **Quartus Version**: 25.1
- **Development Kit**: Atum A3 Nano
- **Device Part**: A3CZ135BB18AE7S
- **Category**: AI, Machine Learning, Image Processing, Demonstration
- **URL**: https://github.com/terasic/Atum-A3-Nano/tree/rel/25.1/Handwritten_Digit_Recognition
- **Design Package**: Atum_A3_Nano_Handwritten_Digit_Recognition.zip

## Hardware Requirements

- Terasic Atum A3 Nano Kit
- Terasic D8M Camera Module

## Software Requirements

- Altera Quartus Prime Pro Edition 25.1
- ONE WARE Studio (for model training and IP export)
