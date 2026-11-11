# Challenges and Future Enhancements

## 1. Development Overview

SmartLab Guardian is developed around the LPC2148 ARM7 microcontroller using Embedded C. The project integrates keypad input, LCD display, RTC functions, infrared remote reception, GPIO outputs, and external interrupts.

This document separates implementation considerations identified during code review from future features. A challenge is not described as solved unless the corresponding fix and successful retest have been confirmed.

## 2. Development Challenges and Verification Notes

### 2.1 Integration of Multiple Peripherals

**Challenge:** The application combines several peripherals, including the LCD, keypad, RTC, IR receiver, LEDs, buzzer, and external interrupt inputs. Each peripheral requires suitable GPIO configuration and software routines.

**Implementation approach:** The project separates peripheral functionality into source modules such as `lcd_16x2.c`, `keyPad.c`, `rtc.c`, `IR_Receiver.c`, and `ExtInterruptSwitches.c`, with initialization support in `Initializations.c`.

**Verification required:** Build the complete Keil project and test each peripheral individually before validating their combined operation.

### 2.2 Keypad Input and Validation

**Challenge / review observation:** Keypad scanning and multi-digit input require correct row/column detection and input-length validation. The reviewed keypad routines contain input-count conditions that should be tested with incomplete and boundary-length entries.

**Implementation approach:** Keypad scanning and input routines are implemented in `keyPad.c`; password-related processing is in `passwordCheck.c`.

**Verification required:** Test every key, valid and invalid PINs, short entries, retry behaviour, and PIN-change confirmation. Record any correction and retest result here.

### 2.3 RTC Configuration and Operating-Hour Checking

**Challenge / review observation:** RTC operation depends on correct device, clock, prescaler, and hardware configuration. The reviewed `IsWithinTime()` routine compares hour values, so minute-level schedule precision should not be assumed.

**Implementation approach:** RTC operations are in `rtc.c`; initialization and operating-hour checking are in `Initializations.c`.

**Verification required:** Confirm the target device and oscillator configuration, then test times before, at, within, and after the configured operating-hour boundaries. Also verify overnight schedules if they are required.

### 2.4 Infrared Remote Signal Reception

**Challenge / review observation:** The IR receiver decodes RC5-format signals by polling the receiver input and measuring signal transitions. The main application includes a blocking wait for a valid IR frame. While this wait is active, other main-loop tasks may not be serviced.

**Implementation approach:** IR decoding is implemented in `IR_Receiver.c` and called from `main.c`.

**Verification required:** Test the intended TV remote and confirm the decoded address, command, and toggle values. Check whether keypad and other application functions remain responsive during IR reception.

### 2.5 External Interrupt Configuration

**Challenge / review observation:** External interrupt operation depends on matching the LPC2148 pin-selection configuration, interrupt configuration, ISR declaration, and physical switch wiring. The pin definitions and configuration should be cross-checked against the final circuit.

**Implementation approach:** Interrupt initialization is in `Initializations.c`; interrupt service routines and related editing functions are in `ExtInterruptSwitches.c`.

**Verification required:** Confirm the actual EINT pin mapping and trigger configuration. Test emergency and RTC-edit switches separately, then test their behaviour during normal application operation.

### 2.6 Keil Project File Organization

**Challenge:** The repository stores source files and header files in separate directories. The Keil project must therefore reference the correct source-file locations and header include paths.

**Implementation approach:** The Keil project file is `Keil_Project/smartLab.uvproj`; source files are in `Source_Code/` and headers are in `Header_Files/`.

**Verification required:** Open the project after organizing the folders, check all paths and target settings, build it, and confirm that the expected output file is generated.

## 3. Future Enhancements

The following are proposed improvements. They are not claimed as features of the current implementation.

1. **RFID or biometric authentication:** Add an additional authentication method for stronger access control.
2. **IoT-based monitoring:** Add network connectivity and a dashboard for remote status monitoring.
3. **Event logging:** Store authentication attempts, equipment state changes, and emergency events with timestamps.
4. **Mobile application:** Provide a mobile interface for authorized monitoring and control.
5. **Additional safety sensors:** Integrate suitable sensors for detecting environmental or equipment-related abnormal conditions.
6. **Non-blocking IR reception:** Replace long blocking polling with a suitable interrupt- or timer-based reception approach, after checking the LPC2148 pin and timer resources.
7. **Improved schedule resolution:** Extend operating-hour checks to include minutes if required by the application.
8. **Configuration persistence:** Store configurable values in non-volatile memory if persistent settings are required and safely implemented.

## 4. Issue Tracking Record

Use this table to record issues found during testing and the verified solutions.

| Issue ID | Observed issue | Root cause | Correction applied | Retest evidence | Status |
|---|---|---|---|---|---|
| CH-01 | To be recorded | To be investigated | To be recorded | Add evidence filename | Open |
| CH-02 | To be recorded | To be investigated | To be recorded | Add evidence filename | Open |

Do not mark an issue as resolved until the correction has been tested successfully.

## 5. Final Validation Notes

Before declaring the project complete:

- Confirm the Keil project builds with the intended LPC2148 configuration.
- Verify the hardware pin connections against the final circuit diagram.
- Test password authentication and keypad input boundaries.
- Verify RTC time/date and operating-hour boundary behaviour.
- Test IR decoding and application responsiveness.
- Test both external interrupt inputs and their programmed responses.
- Save genuine test screenshots or videos in the `Testing/` directory.
- Update the issue-tracking table with actual observations and retest evidence.
