
/*******************************************
File: main.h
Controller: LPC2148 (ARM7)

Purpose:
Declare initialization and RTC-related functions,
define global variables used across project modules,
store laboratory operating-hour settings, and
define custom LCD characters for SmartLab Guardian.
********************************************/

// Header file for user-defined data types
#include "types.h"


/*================ FUNCTION DECLARATIONS ================*/

// Initialize external interrupts EINT0 and EINT3
void EINT_Init(void);

// Set the initial RTC time, date and day
void SetRTC(void);

// Configure LED and buzzer GPIO pins and their initial states
void Init_LEDS(void);

// Read and display current RTC time, date and day
void DisplayRTC(void);

// Check whether the current time is within laboratory operating hours
u32 IsWithinTime(void);


/*================ GLOBAL LED STATUS VARIABLES ================*/

// Maintain the software status of Device 1 and Device 2
int led1_status=0;
int led2_status=0;


/*================ LABORATORY OPERATING HOURS ================*/

// Store the configured laboratory opening and closing hours
u32 startTime=6, endTime=18;

// Maintain the software flag indicating whether the laboratory lock is open
u32 Lockopened=0;

u32 emergency=0;
u32 rtc_button_clicked=0;

/*================ RTC GLOBAL VARIABLES ================*/

// Store the current RTC time, date and day values
s32 hour,min,sec,date,month,year,day;


/*================ KEYPAD GLOBAL VARIABLE ================*/

// Store the most recently pressed keypad key
u8 lastKey;


/*================ IR RECEIVER GLOBAL VARIABLES ================*/

// Store the decoded RC5 address, command and toggle values
unsigned int address, command, toggle;


/*================ CUSTOM LCD CGRAM CHARACTERS ================*/

// Custom character pattern representing the ON status
const unsigned char cgramLUT_ON[8] =
{
    0x00,0x1F,0x1F,0x1F,
    0x1F,0x1F,0x1F,0x00
};

// Custom character pattern representing the OFF status
const unsigned char cgramLUT_OFF[8] =
{
    0x00,0x1F,0x11,0x11,
    0x11,0x11, 0x1F,0x00
};

// Custom character pattern representing a clock symbol
const u8 cgramLUT_CLOCK[8] =
{
    0x0E,0x15,0x17,0x11,
    0x15,0x11,0x0E,0x00
};

// Custom character pattern representing the SmartLab Guardian icon
const u8 guardianLUT[8] =
{
    0x04,0x0E,0x1F,0x15,
    0x0E,0x04,0x0A,0x00
};
