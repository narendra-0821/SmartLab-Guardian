
/*******************************************
Program: SmartLab Guardian - Main Application
Controller: LPC2148 (ARM7)

Purpose:
Initialize all peripherals, authenticate the
administrator, validate laboratory operating
hours, control devices through IR remote commands,
display device status, and handle external
interrupt events.
********************************************/

// Header file for LPC21xx peripheral register definitions
#include <LPC21xx.h>

// Header file for software delay functions
#include "delay.h"

// Header file for main application declarations, global variables and definitions
#include "main.h"

// Header files for LCD interfacing functions and LCD command definitions
#include "lcd_16x2.h"
#include "lcd_16x2_defines.h"

// Header file for matrix keypad interfacing functions
#include "keyBoard.h"

// Header file for project-specific pin definitions and control macros
#include "all_defines.h"

// Header file for RTC interfacing functions
#include "rtc.h"

// Header file for password authentication and PIN management functions
#include "passwordCheck.h"

// Header file for IR receiver functions and signal decoding
#include "IR_Receiver.h"

// Header file for external interrupt service routines and related functions
#include "ExtInterruptSwitches.h"

/*================ MAIN APPLICATION ================*/

int main(){

    /* Initialize LCD and load custom characters */
    InitLCD();                              // Initialize LCD in 8-bit, 2-line mode
    BuildCGRAM((u8 *)cgramLUT_ON,1);        // Load custom ON symbol into CGRAM slot 1
    BuildCGRAM((u8 *)cgramLUT_OFF,2);       // Load custom OFF symbol into CGRAM slot 2
    BuildCGRAM((u8 *)guardianLUT,3);        // Load SmartLab Guardian symbol into CGRAM slot 3
    BuildCGRAM((u8 *)cgramLUT_CLOCK,4);     // Load clock symbol into CGRAM slot 4

    /* Initialize RTC and external interrupts */
    RTC_Init();                             // Initialize the Real-Time Clock peripheral
    EINT_Init();                            // Configure external interrupts for emergency and RTC editing

    /* Initialize devices and input peripherals */
    Init_LEDS();                            // Configure LED and buzzer GPIO pins
    SetRTC();                               // Set the initial RTC time, date and day
    InitKPM();                              // Initialize the matrix keypad

    /* Display project welcome screen */
    CharLCD(3);                             // Display the custom SmartLab Guardian symbol
    StrLCD("  SMART LAB  ");                // Display project name
    CmdLCD(GOTO_LINE1_POS0+13);             // Move cursor to the remaining position on line 1
    CharLCD(3);                             // Display the Guardian symbol again
    CmdLCD(GOTO_LINE2_POS0);                // Move cursor to the beginning of line 2
    StrLCD("   GUARDIAN  ");                // Display project title
    delay_s(3);                             // Keep the welcome screen visible for 3 seconds
    CmdLCD(CLEAR_LCD);                      // Clear the welcome screen

    /* Administrator password authentication */
    if(!(Lockopened)) passWord();           // Request password authentication if the laboratory is locked
    Lockopened=1;                           // Update the software flag to indicate that authentication flow has completed

    /*================ CONTINUOUS MAIN LOOP / SUPER LOOP ================*/

    while(1){

        /* Check whether the current time is within laboratory operating hours */
        if(IsWithinTime()){

            // Check whether the decoded IR command corresponds to Device 1
            if(KEY_1_PRESSED){
                LED1_TOGGLE;                 // Toggle the output connected to Device 1
                led1_status = led1_status^1; // Toggle the software status flag using XOR

            // Check whether the decoded IR command corresponds to Device 2
            }else if(KEY_2_PRESSED){
                LED2_TOGGLE;                 // Toggle the output connected to Device 2
                led2_status = led2_status^1; // Toggle the software status flag using XOR

            }

        }else{

            // Inform the user that the laboratory is outside its operating hours
            CmdLCD(CLEAR_LCD);
            StrLCD("THESE ARE NOT");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("OPERATING HOURS");
            delay_s(3);                      // Keep the operating-hours message visible for 3 seconds
            CmdLCD(CLEAR_LCD);
        }

        /*================ RTC INFORMATION DISPLAY ================*/

        if(TIME_KEY_PRESSED){
            CmdLCD(CLEAR_LCD);
            DisplayRTC();                    // Display current RTC time, date and day
            delay_s(5);                      // Keep RTC information visible for 5 seconds
            CmdLCD(CLEAR_LCD);
        }

        /* Display configured laboratory operating-time boundaries */
        if(TIME_BOUNDARY_KEY_PRESSED){
            CmdLCD(CLEAR_LCD);
            DisplayTimeBoundaries();         // Display configured laboratory operating-time boundaries
            delay_s(3);                      // Keep time-boundary information visible for 3 seconds
            CmdLCD(CLEAR_LCD);
        }

        /*================ DISPLAY DEVICE STATUS ================*/

        // Display the current software status of Device 1
        if(led1_status == LED_ON){
            CmdLCD(GOTO_LINE1_POS0);         // Move cursor to the beginning of LCD line 1
            StrLCD("D1:ON");                  // Display Device 1 ON status
            CmdLCD(GOTO_LINE1_POS0+5);       // Move cursor to the ON symbol position
            CharLCD(1);                      // Display custom ON symbol
        }else{
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("D1:OFF");                 // Display Device 1 OFF status
            CmdLCD(GOTO_LINE1_POS0+6);       // Move cursor to the OFF symbol position
            CharLCD(2);                      // Display custom OFF symbol
        }

        // Display the current software status of Device 2
        if(led2_status == LED_ON){
            CmdLCD(GOTO_LINE1_POS0+8);       // Move cursor to the Device 2 status position
            StrLCD("D2:ON");                  // Display Device 2 ON status
            CmdLCD(GOTO_LINE1_POS0+13);      // Move cursor to the ON symbol position
            CharLCD(1);                      // Display custom ON symbol
        }else{
            CmdLCD(GOTO_LINE1_POS0+8);
            StrLCD("D2:OFF");                 // Display Device 2 OFF status
            CmdLCD(GOTO_LINE1_POS0+14);      // Move cursor to the OFF symbol position
            CharLCD(2);                      // Display custom OFF symbol
        }

        /*================ DISPLAY TIME PROMPT ================*/

        CmdLCD(GOTO_LINE2_POS0);             // Move cursor to the beginning of LCD line 2
        StrLCD("FOR TIME:");                 // Display the time information prompt
        CharLCD(4);                          // Display the custom clock symbol

        /*================ CHECK EXTERNAL INTERRUPT FLAGS ================*/

        if(emergency==1){
            emergencyAlert();                // Execute the emergency response routine
            emergency=0;                    // Clear the software emergency flag after handling
        }

        if(rtc_button_clicked==1){
            rtc_interrupt();                 // Enter the RTC configuration routine
            rtc_button_clicked=0;            // Clear the RTC button event flag after handling
        }

        /*================ IR REMOTE COMMAND RECEPTION ================*/

        // Continue checking the TSOP receiver until a valid IR frame is received
        while((readTSOP(&address,&command,&toggle))==0){

            // Check whether the emergency switch was activated while waiting for an IR frame
            if(emergency==1){
                emergencyAlert();            // Execute the emergency response routine
                emergency=0;                 // Clear the emergency flag after handling
                break;                        // Exit the IR reception loop after emergency handling
            }

            // Check whether the RTC-edit switch was activated while waiting for an IR frame
            if(rtc_button_clicked==1){
                rtc_interrupt();              // Enter the RTC configuration routine
                rtc_button_clicked=0;         // Clear the RTC button event flag after handling
                break;                        // Exit the IR reception loop after RTC handling
            }
        }
    }
}
