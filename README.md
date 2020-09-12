# Xling, a pocket demon

![Image](Xling_v3.2.png)

This is a little tamagotchi-like toy I've created for my wife. I used to think
that this is how a modern Tamagotchi could look like initially. However, it can
also be used as a battery-powered DIY platform to experiment with FreeRTOS and
graphics.

## Specifications

  * 35x47mm board
  * Monochrome OLED 128x64px
  * Low energy consumption (~20mA @ active, 0.75mA @ sleep)
  * Hand-soldering friendly (TQFP-44 0.8mm pitch, smallest is 0603)
  * 3D-printable case
  * Open Source Hardware/Firmware

## Hardware

  * ATmega1284P 8-bit AVR MCU with 128 KiB flash and 16 KiB SRAM
  * SH1106G display controller
  * MCP73831 charge controller
  * AP3401 DC-DC buck converter
  * 400mAh 3.7V Li-Po battery (~26h @ active, ~30 days @ sleep)
  * Load-sharing circuit (can be powered by USB or battery, like a smartphone)
  * KiCad 5.1.0 project for schematic and board designs
  * Released under CERN-OHL-W v2 (Rev. 4) and CC-BY-SA (before Rev. 4)

![Image](hardware/output/Xling_v3.2_hw_bottom.png)

## Software

Please, note that the software part of the project isn't finished yet!

  * Shows a frame time, energy left and a battery charged status
  * Animation with Exy-in-the-bottle, a "personal daemon"
  * Programmed in C99 using the FreeRTOS kernel modified for ATmega1284P
  * There are three tasks only: Display, Battery Monitor and Sleep Mode
  * Released under GNU GPLv3+

## Website and Contact

Hackaday.io project: https://hackaday.io/project/165726-xling    
Email: dsl AT mcusim DOT org    
Dmitry Salychev
