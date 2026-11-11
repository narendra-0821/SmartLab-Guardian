
/*******************************************
File: keyBoard.h
Controller: LPC2148 (ARM7)

Purpose:
Declare keypad interfacing functions used
to initialize the keypad, scan rows and
columns, identify pressed keys, and read
numeric inputs from the administrator.
********************************************/

// Header file for user-defined data types
#include "types.h"


/*================ KEYPAD INITIALIZATION AND SCANNING ================*/

// Configure the GPIO pins connected to the keypad
void InitKPM(void);

// Check whether any keypad column detects a key press
u8 ColScan(void);

// Scan keypad rows to identify the row containing the pressed key
u8 RowScan(void);

// Identify the column containing the pressed key
u8 ColCheck(void);

// Identify the row containing the pressed key
u8 RowCheck(void);

// Read a numeric value from the keypad and return it
u32 ReadNum(void);

// Scan the keypad and return the corresponding pressed key character
u8 KeyScan(void);


/*================ NUMERIC INPUT FUNCTIONS ================*/

// Read numeric input and store the result through pointer arguments
void ReadNum2(u32 *,u8 *);

// Read numeric input for RTC time configuration
void ReadTime(u32 *num,u8 *lastKey);
void ReadYear(u32 *num,u8 *lastKey);

// Read up to four numeric digits for administrator PIN authentication
void ReadNums(u32 *num,u8 *lastKey);
