# SmartLab Guardian: Secure and Automated Laboratory Control System

An LPC2148-based embedded system designed to improve laboratory security, automate operating-hour control, provide infrared remote supervision, and support emergency operations.

## 1. Project Overview

**SmartLab Guardian** is a secure and automated laboratory control system developed using the LPC2148 ARM7 microcontroller and Embedded C.

The system combines keypad-based password authentication, Real-Time Clock (RTC)-based operating-hour verification, infrared (IR) remote control, LCD status display, and external interrupt mechanisms.

Its purpose is to provide controlled access to laboratory equipment, support scheduled operation, and enable quick response to emergency situations.

## 2. Objectives

- Implement password-based authentication for laboratory access.
- Control laboratory operation according to predefined operating hours using the RTC.
- Provide supervisory control through a standard TV remote and TSOP1738 IR receiver.
- Display system information and operating status using a 16x2 LCD.
- Indicate equipment status using LEDs.
- Implement an emergency shutdown mechanism using an external interrupt.
- Provide an interrupt-based option for editing RTC settings.

## 3. System Architecture

The LPC2148 acts as the central controller and interfaces with the input and output peripherals.

### Input Components
- 4x4 Matrix Keypad – Password entry and user interaction.
- TSOP1738 IR Receiver – Receives infrared signals from a TV remote.
- Emergency Switch – Initiates emergency interrupt handling.
- RTC Edit Switch – Initiates RTC configuration mode.

### Processing Unit
- LPC2148 ARM7 Microcontroller – Processes inputs, manages authentication, checks operating hours, and controls the connected peripherals.
- Real-Time Clock (RTC) – Maintains date and time information.

### Output Components
- 16x2 LCD – Displays time, authentication information, and system messages.
- LED 1 – Device 1 status indication.
- LED 2 – Device 2 status indication.
- Buzzer – Emergency warning indication.

## 4. Block Diagram

The system follows this general architecture:

```text
              +----------------+
              |     Keypad     |
              +--------+-------+
                       |
              +--------v-------+
              |                |       +---------+
              |                +------>|   LCD   |
              |                |       +---------+
+-------------+                |
| Emergency   |                |       +---------+
| Switch      +--------------->|------>| Device 1|
+-------------+                |       |  (LED1) |
              |                |       +---------+
+-------------+    LPC2148     |
| RTC Edit    +--------------->|       +---------+
| Switch      |                +------>| Device 2|
+-------------+                |       |  (LED2) |
              |                |       +---------+
+-------------+                |
| IR Receiver +--------------->|       +---------+
| TSOP1738    |                +------>| Buzzer  |
+-------------+                |       +---------+
              |                |
              |     RTC        |
              +----------------+
```

*This is a simplified representation of the system. Refer to the actual circuit diagram for hardware connections.*

## 5. Project Workflow

### Step 1: System Initialization
- Initialize the LCD.
- Configure the RTC.
- Initialize the keypad and LED/buzzer outputs.
- Configure the external interrupts.
- Display the welcome message.

### Step 2: Password Authentication
- The user enters a password through the keypad.
- The system checks the entered password.
- Access is handled according to the authentication result.
- The password management functionality also provides an option to change the PIN.

### Step 3: Operating-Hour Verification
- The RTC provides the current time.
- The system compares the current hour with the configured laboratory operating hours.
- Laboratory operation is evaluated according to the configured time boundaries.

### Step 4: IR Remote Control
- The TSOP1738 receives signals from a standard TV remote.
- The IR receiver software decodes the RC5-format signals.
- The decoded address, command, and toggle information are made available to the application.

### Step 5: Emergency Shutdown
- The emergency switch activates the configured external interrupt.
- The emergency interrupt routine handles the shutdown and warning indications.

### Step 6: RTC Configuration
- The RTC edit switch activates the second external interrupt.
- The RTC editing functionality allows date/time settings and operating-hour boundaries to be configured.

## 6. Hardware Requirements

| Component | Purpose |
|---|---|
| LPC2148 | Main microcontroller |
| 16x2 LCD | Display and user information |
| 4x4 Matrix Keypad | Password entry and user input |
| LEDs | Device status indication |
| TSOP1738 | Infrared signal reception |
| TV Remote | Remote supervisory input |
| Buzzer | Emergency indication |
| Switches | Emergency and RTC editing inputs |

Additional interfacing components, power supplies, and driver circuits should be selected according to the actual hardware circuit.

## 7. Software Requirements

| Software / Language | Purpose |
|---|---|
| Embedded C | Application programming |
| Keil µVision | Project development and compilation |
| ARMCC Compiler | Compiling the LPC2148 application |
| Flash Magic | Programming the microcontroller |

**Note:** Use the compiler and device configuration compatible with the supplied Keil project.

## 8. Repository Structure

```text
SmartLab-Guardian/
│
├── README.md
├── LICENSE
├── .gitignore
│
├── Source_Code/
│   ├── Startup.s
│   ├── delay.c
│   ├── lcd_16x2.c
│   ├── keyPad.c
│   ├── rtc.c
│   ├── Initializations.c
│   ├── p_test.c
│   ├── passwordCheck.c
│   ├── IR_Receiver.c
│   └── ExtInterruptSwitches.c
│
├── Header_Files/
│   ├── all_defines.h
│   ├── defines.h
│   ├── delay.h
│   ├── ExtInterruptSwitches.h
│   ├── IR_Receiver.h
│   ├── keyBoard.h
│   ├── keyBoard_defines.h
│   ├── lcd_16x2.h
│   ├── lcd_16x2_defines.h
│   ├── main.h
│   ├── passwordCheck.h
│   ├── rtc.h
│   ├── rtc_defines.h
│   └── types.h
│
├── Keil_Project/
│   └── smartLab.uvproj
│
├── Hardware/
│   ├── Circuit_Diagram/
│   ├── Block_Diagram/
│   └── Hardware_Images/
│
├── Software/
│   └── Tools_and_Libraries.md
│
├── Documentation/
│   ├── Project_Report.pdf
│   └── Workflow.md
│
├── Testing/
│   ├── Test_Cases.md
│   ├── Output_Screenshots/
│   └── Output_Videos/
│
└── Challenges_and_Future_Enhancements.md
```

## 9. How to Build the Project

Follow these steps to build the project using Keil µVision.

1. Install Keil µVision with a compatible ARMCC toolchain.
2. Clone or download this repository.
3. Open the `smartLab.uvproj` project file from the `Keil_Project` folder.
4. Verify that the selected target device is LPC2148.
5. Check the source-file paths and header-file include paths in the project settings.
6. Verify the startup assembly file and required compiler settings.
7. Select **Project → Build Target**.
8. Check the build output for compilation and linking errors.
9. Generate the HEX file if it is not generated automatically.

**Important:** Since source and header files are maintained in separate directories, verify the paths in Keil before building.

## 10. How to Program the Microcontroller

After successfully building the project:

1. Connect the LPC2148 development board to the computer.
2. Identify the correct serial COM port.
3. Open Flash Magic.
4. Select the appropriate LPC2148 device and programming interface settings.
5. Load the generated HEX file.
6. Configure the required programming settings according to the board.
7. Program the microcontroller.
8. Reset the board and observe the system operation.

Refer to the LPC2148 board documentation for the correct bootloader and programming connections.

## 11. Testing and Validation

The following functional areas should be tested:

| Test ID | Functionality | Expected Behaviour |
|---|---|---|
| TC01 | System initialization | LCD and peripherals initialize correctly |
| TC02 | Correct password | Authentication succeeds |
| TC03 | Incorrect password | Authentication failure is handled |
| TC04 | PIN change | New PIN is accepted according to the implemented validation |
| TC05 | RTC display | Current date and time are displayed |
| TC06 | Operating-hour verification | System evaluates the configured operating schedule |
| TC07 | IR receiver | Valid RC5 signals are decoded |
| TC08 | Emergency switch | Emergency interrupt routine is executed |
| TC09 | RTC edit switch | RTC editing routine is entered |
| TC10 | LED and buzzer | Output indications respond as implemented |

**Testing status:** Actual results, pass/fail status, and hardware screenshots should be added to the `Testing/` directory after validation.

## 12. Challenges and Solutions

Document the actual development challenges encountered during implementation, including:

- Peripheral initialization and GPIO configuration.
- LCD interfacing and display control.
- Matrix keypad scanning and input validation.
- RTC configuration and time management.
- Infrared RC5 signal decoding.
- External interrupt configuration and handling.
- Integration of multiple peripherals with the LPC2148.

The specific issue, its cause, the solution applied, and the final verification result should be recorded in `Challenges_and_Future_Enhancements.md`.

## 13. Future Enhancements

Potential improvements include:

- Integration of RFID or biometric authentication.
- Remote laboratory monitoring through an IoT dashboard.
- Event logging and access-history storage.
- Mobile application-based monitoring.
- Additional safety sensors and fault detection.
- Improved non-blocking IR signal reception.
- More detailed equipment monitoring and energy management.

These are proposed enhancements and are not claimed as implemented features.

## 14. Project Applications

- Educational laboratories.
- Electronics and embedded systems laboratories.
- Research facilities.
- Industrial training centers.
- Smart campus environments.

## 15. Author and Contribution

**Project:** SmartLab Guardian – Secure and Automated Laboratory Control System

**Controller:** LPC2148 ARM7 Microcontroller

**Programming Language:** Embedded C

This repository contains the project source code, header files, Keil project, hardware documentation, software requirements, and testing documentation.

---

**Note:** This README describes the project architecture and intended operation. Actual hardware functionality and test results must be verified against the implemented circuit and recorded testing evidence.
