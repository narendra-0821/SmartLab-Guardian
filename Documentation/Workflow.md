# Project Workflow

## 1. Overview

SmartLab Guardian is an LPC2148-based laboratory control project. Its
documented functions include keypad-based password authentication,
RTC-based operating-hour checking, infrared remote input, LCD/LED
indications, emergency interrupt handling, and RTC configuration through
an external interrupt.

This workflow summarizes the supplied project description and the source
modules reviewed. It does not claim that every function has been
validated on physical hardware.

## 2. System initialization

The main application is in `main.c`. The initialization code includes
LCD setup, RTC setup, keypad initialization, LED/buzzer GPIO
initialization, and external interrupt configuration. Refer to
`main.c` and `Initializations.c` for the exact call order and values.

## 3. Password authentication

1.  The application prompts the user for password entry through the
    keypad.
2.  Keypad scanning identifies the pressed key.
3.  The password module checks the entered value against the configured
    PIN.
4.  The module handles failed attempts/retry behaviour and provides a
    PIN-change routine.

Relevant files: `keyPad.c`, `passwordCheck.c`.

## 4. RTC and operating-hour check

1.  The RTC provides current time and date information.
2.  The application reads time through the RTC functions.
3.  `IsWithinTime()` compares the current hour with configured start and
    end hours.
4.  The application uses the result to evaluate whether the laboratory
    is within its configured operating period.

The reviewed implementation compares hour values; do not assume
minute-level schedule precision.

Relevant files: `rtc.c`, `Initializations.c`, `main.c`.

## 5. Infrared remote reception

1.  The TSOP1738 receives the demodulated remote-control signal.
2.  The IR receiver code polls the input and measures signal
    transitions.
3.  The decoder attempts to recover RC5 address, command, and toggle
    information.
4.  The decoded values are returned to the application.

Relevant files: `IR_Receiver.c`, `IR_Receiver.h`, `main.c`.

**Implementation consideration:** The main application contains a
blocking polling loop while waiting for a valid IR frame. During this
wait, other main-loop tasks may not be serviced. Verify responsiveness
before relying on simultaneous keypad and IR operation.

## 6. External interrupt operations

-   **Emergency interrupt:** The emergency-switch interrupt routine
    handles the response programmed in `ExtInterruptSwitches.c`.
-   **RTC-edit interrupt:** The second interrupt enters the RTC editing
    functionality. The associated routines provide RTC and
    operating-time-boundary editing.

Verify the configured EINT pin mapping against the LPC2148 pinout and
the final circuit.

## 7. LCD, LEDs, and buzzer

The LCD driver sends commands and character data to the 16x2 display.
LED macros and initialization functions control the device-status
indicators. Buzzer macros provide the warning output used by the
emergency routine.

Relevant files: `lcd_16x2.c`, `Initializations.c`, `all_defines.h`,
`main.c`, and `ExtInterruptSwitches.c`.

## 8. High-level flow

``` text
Power ON / Reset
       |
       v
Initialize LCD, RTC, Keypad,
LEDs, Buzzer and Interrupts
       |
       v
Display Welcome / Prompt
       |
       v
Keypad Password Authentication
       |
       v
Read RTC and Check Operating Hours
       |
       v
Main Application Loop
       |
       +--> Read keypad input
       +--> Read/display RTC information
       +--> Poll and decode IR input
       +--> Update output indications
       +--> Emergency switch interrupt
       +--> RTC-edit switch interrupt
```

This is a functional overview. Exact branching, blocking behaviour, and
output actions are determined by the source code.

## 9. Verification before claiming completion

-   Build the Keil project with the intended LPC2148 target
    configuration.
-   Verify all pin assignments against the circuit.
-   Test authentication and operating-hour boundaries.
-   Test IR decoding with the intended remote.
-   Test emergency and RTC-edit switches.
-   Record observed results and evidence in `Testing/`.
