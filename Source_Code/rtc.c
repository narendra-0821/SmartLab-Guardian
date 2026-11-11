
/*******************************************
Program: RTC (Real-Time Clock) Support
Controller: LPC2148 (ARM7)

Purpose:
Initialize the RTC, read and update time,
date and day information, and display the
retrieved RTC values on the LCD.
********************************************/

// Header file for LPC21xx peripheral register definitions
#include <LPC21xx.h>

// Header files for LCD interfacing and display commands
#include "lcd_16x2.h"
#include "lcd_16x2_defines.h"

// Header files for RTC function declarations and register definitions
#include "rtc.h"
#include "rtc_defines.h"


/*================ DAY OF WEEK LOOKUP TABLE ================*/

// Stores the three-letter names corresponding to RTC day values (0 to 6)
char week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};


/*================ RTC INITIALIZATION ================*/

void RTC_Init(void) 
{
    // Disable and reset the RTC
    //CCR = RTC_RESET;

    #if (CPU == LPC2129)
    // Configure integer and fractional parts of the RTC prescaler
    PREINT = PREINT_VAL;
    PREFRAC = PREFRAC_VAL;

    // Enable the RTC using the configured prescaler
    CCR = RTC_ENABLE; 

    #elif (CPU == LPC2148)
    // Enable the RTC and select the external clock source
    CCR = RTC_ENABLE | RTC_CLKSRC; 
    #endif
}


/*================ READ RTC TIME ================*/

// Read current hours, minutes and seconds into the supplied variables
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
    *hour = HOUR;       // Read RTC hour register
    *minute = MIN;      // Read RTC minute register
    *second = SEC;      // Read RTC second register
}


/*================ DISPLAY RTC TIME ================*/

// Display time in HH:MM:SS format on the first LCD line
void DisplayRTCTime(u32 hour, u32 minute, u32 second)
{
    CmdLCD(GOTO_LINE1_POS0);

    CharLCD((hour/10)+48);          // Extract tens digit and convert it to ASCII
    CharLCD((hour%10)+48);          // Extract units digit and convert it to ASCII
    CharLCD(':');

    CharLCD((minute/10)+0x30);      // Extract tens digit and convert it to ASCII
    CharLCD((minute%10)+0x30);      // Extract units digit and convert it to ASCII
    CharLCD(':');

    CharLCD((second/10)+'0');       // Extract tens digit and convert it to ASCII
    CharLCD((second%10)+'0');       // Extract units digit and convert it to ASCII
}


/*================ READ RTC DATE ================*/

// Read current date, month and year into the supplied variables
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year)
{
    *date = DOM;        // Read day-of-month register
    *month = MONTH;     // Read month register
    *year = YEAR;       // Read year register
}


/*================ DISPLAY RTC DATE ================*/

// Display date in DD/MM/YYYY format on the second LCD line
void DisplayRTCDate(u32 date, u32 month, u32 year)
{
    CmdLCD(GOTO_LINE2_POS0);

    CharLCD((date/10)+48);          // Extract tens digit and convert it to ASCII
    CharLCD((date%10)+48);          // Extract units digit and convert it to ASCII
    CharLCD('/');

    CharLCD((month/10)+0x30);       // Extract tens digit and convert it to ASCII
    CharLCD((month%10)+0x30);       // Extract units digit and convert it to ASCII
    CharLCD('/');

    U32LCD(year);                   // Display the four-digit year as an unsigned integer
}


/*================ SET RTC TIME ================*/

// Write the supplied hours, minutes and seconds into RTC registers
void SetRTCTimeInfo(u32 hour, u32 minute, u32 second)
{
    HOUR = hour;        // Update RTC hour register
    MIN = minute;       // Update RTC minute register
    SEC = second;       // Update RTC second register
}


/*================ SET RTC DATE ================*/

// Write the supplied date, month and year into RTC registers
void SetRTCDateInfo(u32 date, u32 month, u32 year)
{
    DOM = date;         // Update day-of-month register
    MONTH = month;      // Update month register
    YEAR = year;        // Update year register
}


/*================ READ RTC DAY ================*/

// Read the current day-of-week value from the RTC
void GetRTCDay(s32 *day)
{
    *day = DOW;         // Read day-of-week register (0 = Sunday, 6 = Saturday)
}


/*================ DISPLAY RTC DAY ================*/

// Display the corresponding three-letter weekday name on the LCD
void DisplayRTCDay(u32 day)
{
    CmdLCD(GOTO_LINE1_POS0+11);
    StrLCD(week[day]);  // Use the day value as an index into the weekday lookup table
}


/*================ SET RTC DAY ================*/

// Write the supplied day-of-week value into the RTC
void SetRTCDay(u32 day)
{
    DOW = day;          // Update day-of-week register
}
