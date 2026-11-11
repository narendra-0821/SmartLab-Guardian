# Tools and Libraries

This document records the hardware, development tools, and software
components identified for **SmartLab Guardian: Secure and Automated
Laboratory Control System**.

## Hardware

  Component                      Purpose
  ------------------------------ ----------------------------------------------
  LPC2148 ARM7 microcontroller   Main controller
  16x2 LCD                       Displays system messages and RTC information
  4x4 matrix keypad              Password entry and user input
  LEDs                           Device status indication
  TSOP1738 IR receiver           Receives infrared signals from a TV remote
  TV remote                      Supervisory control input
  Buzzer                         Warning indication
  Switches                       Emergency and RTC-edit inputs
  RTC                            Maintains date and time

Confirm exact board revision, oscillator, wiring, supply, and driver
circuits against the final circuit diagram.

## Development software

  Tool / Language                  Purpose
  -------------------------------- ---------------------------------
  Embedded C                       Application programming
  Keil µVision / Keil C Compiler   Editing, compiling, and linking
  Flash Magic                      Microcontroller programming

The supplied project is `Keil_Project/smartLab.uvproj`. The source uses
the LPC21xx device header and the ARMCC-style `__irq` declaration. Use a
compatible Keil/ARMCC toolchain and verify the selected target.

## Source modules

  -----------------------------------------------------------------------
  File                                Responsibility
  ----------------------------------- -----------------------------------
  `Startup.s`                         Startup and processor
                                      initialization support

  `main.c`                          Main application flow

  `Initializations.c`                 Peripheral/interrupt initialization
                                      and operating-hour check

  `delay.c`                           Software delay routines

  `lcd_16x2.c`                        LCD driver

  `keyPad.c`                          Keypad scanning and input

  `rtc.c`                             RTC read, write, and display
                                      functions

  `passwordCheck.c`                   Password, PIN change, and retry
                                      handling

  `IR_Receiver.c`                     RC5-format IR signal decoding

  `ExtInterruptSwitches.c`            External interrupt routines and
                                      RTC/time-boundary editing
  -----------------------------------------------------------------------

Header files are stored in `Header_Files/`.

## Libraries and dependencies

The reviewed source uses `LPC21xx.h` and project-specific headers such
as `types.h`, `defines.h`, and peripheral headers. No separate
third-party software library dependency has been confirmed from the
source files reviewed. The peripheral functions are implemented in the
project source modules.

Do not add library names or versions unless they are required by the
final Keil project.

## Build and programming

1.  Open `Keil_Project/smartLab.uvproj` in a compatible Keil µVision
    installation.
2.  Check the target device, compiler, startup file, source paths, and
    include paths.
3.  Build the project and resolve any path/toolchain errors.
4.  Generate the HEX file using the configured output options.
5.  Use Flash Magic with the correct device, interface, COM port, and
    board programming settings.

## Record exact versions before release

  Item                     Version / configuration
  ------------------------ ------------------------------------------
  Keil µVision             To be recorded
  ARMCC compiler           To be recorded
  Flash Magic              To be recorded
  LPC2148 board revision   To be recorded
  Oscillator frequency     Verify from hardware and target settings

Clock-related source macros must match the physical board and Keil
target configuration, particularly for timing-sensitive routines.
