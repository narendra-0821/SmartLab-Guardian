
/*******************************************
File: rtc.h
Controller: LPC2148 (ARM7)

Purpose:
Declare RTC functions used to initialize
the RTC, read and update time, date and
day information, and display RTC values
on the LCD.
********************************************/

// Header file for user-defined data types
#include "types.h"


/*================ RTC INITIALIZATION ================*/

// Initialize the RTC peripheral
void RTC_Init(void);


/*================ RTC TIME FUNCTIONS ================*/

// Read current RTC hours, minutes and seconds through pointer arguments
void GetRTCTimeInfo(s32 *,s32 *,s32 *);

// Display hours, minutes and seconds on the LCD
void DisplayRTCTime(u32,u32,u32);

// Set RTC hours, minutes and seconds
void SetRTCTimeInfo(u32,u32,u32);


/*================ RTC DATE FUNCTIONS ================*/

// Read current RTC date, month and year through pointer arguments
void GetRTCDateInfo(s32 *,s32 *,s32 *);

// Display date, month and year on the LCD
void DisplayRTCDate(u32,u32,u32);

// Set RTC date, month and year
void SetRTCDateInfo(u32,u32,u32);
void DisplayTimeBoundaries(void);

/*================ RTC DAY FUNCTIONS ================*/

// Read the current day-of-week value through a pointer argument
void GetRTCDay(s32 *);

// Display the day of the week on the LCD
void DisplayRTCDay(u32);

// Set the day-of-week value in the RTC
void SetRTCDay(u32);
