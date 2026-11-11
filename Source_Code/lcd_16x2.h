
/*******************************************
File: lcd_16x2.h
Controller: LPC2148 (ARM7)

Purpose:
Declare LCD interfacing functions used for
sending commands, displaying characters,
strings and numbers, initializing the LCD,
and creating custom characters.
********************************************/

// Header file for user-defined data types
#include "types.h"


/*================ LCD FUNCTION DECLARATIONS ================*/

// Send an 8-bit command or instruction to the LCD
void WriteLCD(u8 dat);

// Select the LCD command register and send a command
void CmdLCD(u8 cmd);

// Select the LCD data register and display an ASCII character
void CharLCD(u8 asciiVal);

// Configure GPIO pins and initialize the LCD
void InitLCD(void);

// Display a null-terminated string on the LCD
void StrLCD(s8 *);

// Convert and display an unsigned 32-bit integer on the LCD
void U32LCD(u32 n);

// Create a custom character by writing its 8-row pattern into a CGRAM slot
void BuildCGRAM(u8 *,u8);
