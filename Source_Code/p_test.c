
/*******************************************
Program: SmartLab Guardian - Main Application
Controller: LPC2148 (ARM7)

Purpose:
Initialize all peripherals, authenticate the
administrator, validate laboratory operating
hours, control devices through keypad inputs,
display device status, and receive IR commands.
********************************************/

// Header file for LPC21xx peripheral register definitions
#include <LPC21xx.h>

// Header file for software delay functions
#include "delay.h"

// Header file for main application declarations and definitions
#include "main.h"

// Header files for LCD interfacing and command definitions
#include "lcd_16x2.h"
#include "lcd_16x2_defines.h"

// Header file for keypad interfacing functions
#include "keyBoard.h"

// Header file for project-specific pin definitions and macros
#include "all_defines.h"

// Header file for RTC functions
#include "rtc.h"

// Header file for password authentication functions
#include "passwordCheck.h"

// Header file for IR receiver functions and variables
#include "IR_Receiver.h"


/*================ MAIN APPLICATION ================*/

int main(){

    /* Initialize LCD and load custom characters */
    InitLCD();                              // Initialize LCD in 8-bit, 2-line mode
    BuildCGRAM((u8 *)cgramLUT_ON,1);        // Load custom symbol representing ON status into CGRAM slot 1
    BuildCGRAM((u8 *)cgramLUT_OFF,2);       // Load custom symbol representing OFF status into CGRAM slot 2
    BuildCGRAM((u8 *)guardianLUT,3);        // Load SmartLab Guardian symbol into CGRAM slot 3
    BuildCGRAM((u8 *)cgramLUT_CLOCK,4);     // Load clock symbol into CGRAM slot 4

    /* Initialize RTC and external interrupts */
    RTC_Init();                             // Initialize the Real-Time Clock
    EINT_Init();                            // Configure external interrupts EINT0 and EINT3

    /* Initialize devices and input peripherals */
    Init_LEDS();                            // Configure LED and buzzer GPIO pins
    SetRTC();                               // Set the initial RTC time, date and day
    InitKPM();                              // Initialize the matrix keypad

    /* Display project welcome screen */
    CharLCD(3);
    StrLCD("  SMART LAB  ");
    CmdLCD(GOTO_LINE1_POS0+13);
    CharLCD(3);
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("   GUARDIAN  ");
    delay_s(3);
    CmdLCD(CLEAR_LCD);

    /* Administrator password authentication */
    if(!(Lockopened)) passWord();           // Request password authentication if the laboratory is locked
    Lockopened=1;                           // Update the software flag to indicate that the lock is open


    /*================ CONTINUOUS MAIN LOOP ================*/

    while(1){

        /* Check whether the current time is within laboratory operating hours */
        if(IsWithinTime()){

            if(KEY_1_PRESSED){
                LED1_TOGGLE;                // Toggle the output connected to Device 1
                led1_status = led1_status^1; // Toggle the software status flag using XOR

            }else if(KEY_2_PRESSED){
                LED2_TOGGLE;                // Toggle the output connected to Device 2
                led2_status = led2_status^1; // Toggle the software status flag using XOR

            }else if(TIME_KEY_PRESSED){
                DisplayRTC();               // Display current RTC time, date and day
                delay_s(5);
                CmdLCD(CLEAR_LCD);
            }

        }else{
            CmdLCD(CLEAR_LCD);
            StrLCD("THESE ARE NOT");
            CmdLCD(GOTO_LINE2_POS0);
            StrLCD("OPERATING HOURS");
            delay_s(3);
            CmdLCD(CLEAR_LCD);
        }

				
        /*================ DISPLAY DEVICE STATUS ================*/

        // Display the current software status of Device 1
        if(led1_status == LED_ON){
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("D1:ON");
            CmdLCD(GOTO_LINE1_POS0+5);
            CharLCD(1);                     // Display custom ON symbol
        }else{
            CmdLCD(GOTO_LINE1_POS0);
            StrLCD("D1:OFF");
            CmdLCD(GOTO_LINE1_POS0+6);
            CharLCD(2);                     // Display custom OFF symbol
        }

        // Display the current software status of Device 2
        if(led2_status == LED_ON){
            CmdLCD(GOTO_LINE1_POS0+8);
            StrLCD("D2:ON");
            CmdLCD(GOTO_LINE1_POS0+13);
            CharLCD(1);                     // Display custom ON symbol
        }else{
            CmdLCD(GOTO_LINE1_POS0+8);
            StrLCD("D2:OFF");
            CmdLCD(GOTO_LINE1_POS0+14);
            CharLCD(2);                     // Display custom OFF symbol
        }
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("FOR TIME:");
				CharLCD(4);
			
        /*================ IR REMOTE COMMAND RECEPTION ================*/

        // Keep checking the TSOP receiver until a valid IR frame is received
       // while((readTSOP(&address,&command,&toggle))==0);

    }
}
