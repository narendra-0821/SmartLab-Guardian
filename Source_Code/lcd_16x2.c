
/*******************************************
Program: 16x2 LCD Interfacing
Controller: LPC2148 (ARM7)

Purpose:
Interface a 16x2 LCD in 8-bit mode, send
commands and characters, display strings
and numbers, and create custom characters
using CGRAM.
********************************************/

// Header file for LPC21xx peripheral register definitions
#include <LPC21xx.h>

// Header file for bit manipulation macros
#include "defines.h"

// Header file for LCD pin definitions and command macros
#include "lcd_16x2_defines.h"

// Header file for user-defined data types
#include "types.h"

// Header file for software delay functions
#include "delay.h"


/*================ WRITE DATA TO LCD ================*/

// Send an 8-bit command or character to the LCD data pins
void WriteLCD(u8 dat)
{
    IOCLR0 = (0xFF << LCD_DATA);       // Clear the previous data from LCD data pins P0.8 to P0.15

    IOSET0 = ((u32)dat << LCD_DATA);   // Place the required 8-bit data on the LCD data pins

    IOCLR0 = 1<< LCD_RW;               // Select write operation (RW = 0)

    IOSET0 = 1<<LCD_EN;                // Set EN HIGH to start data transfer
    delay_us(1);                       // Maintain the enable pulse for the required duration
    IOCLR0 =1<<LCD_EN;                 // Set EN LOW to latch the data into the LCD

    delay_ms(2);                       // Allow the LCD time to process the command or data
}


/*================ SEND COMMAND TO LCD ================*/

// Send an instruction or command to the LCD
void CmdLCD(u8 cmd){

    IOCLR0=1<<LCD_RS;                  // Select command register (RS = 0)
    WriteLCD(cmd);                     // Send the command through the LCD data pins
}


/*================ DISPLAY CHARACTER ON LCD ================*/

// Send a character to the LCD for display
void CharLCD(u8 asciiVal){

    IOSET0=1<<LCD_RS;                  // Select data register (RS = 1)
    WriteLCD(asciiVal);                // Send the ASCII character through the data pins
}


/*================ LCD INITIALIZATION ================*/

// Configure GPIO pins and initialize the LCD in 8-bit, 2-line mode
void InitLCD(void){

    IODIR0 |=((0XFF<<LCD_DATA) | (1<<LCD_RS) |(1<<LCD_RW) | (1<<LCD_EN)); // Configure LCD data and control pins as outputs

    delay_ms(15);                      // Wait for LCD power-on initialization

    CmdLCD(0x30);                      // Send initial function-set command
    delay_ms(4);                       // Wait for LCD initialization
    delay_us(100);

    CmdLCD(0x30);                      // Repeat function-set command
    delay_us(100);

    CmdLCD(0x30);                      // Complete the initial function-set sequence

    CmdLCD(MODE_8BIT_2LINE);           // Configure 8-bit interface and 2-line display
    CmdLCD(DSP_ON_CUR_OFF);            // Turn ON display and turn OFF cursor
    CmdLCD(CLEAR_LCD);                 // Clear the LCD display
    CmdLCD(SHIFT_CUR_RIGHT);           // Set cursor movement direction to right
}


/*================ DISPLAY STRING ON LCD ================*/

// Display a null-terminated string character by character
void StrLCD(s8 *s){
    while(*s){
        CharLCD(*s++);                 // Display current character and advance the string pointer
    }
}


/*================ DISPLAY UNSIGNED 32-BIT INTEGER ================*/

// Convert an unsigned integer into decimal digits and display it on LCD
void U32LCD(u32 n){

    u8 a[10];                          // Maximum 10 decimal digits for a 32-bit unsigned integer
    s32 i=0;

    if(n==0)
        CharLCD('0');                  // Display zero directly

    else{
        while(n>0){
            a[i]=(n%10)+48;            // Extract the last digit and convert it to ASCII
            n/=10;                     // Remove the extracted digit
            i++;
        }

        for(--i;i>=0;i--)
            CharLCD(a[i]);             // Display digits in reverse extraction order
    }
}


/*================ CREATE CUSTOM LCD CHARACTER ================*/

// Store an 8-row custom character pattern in the selected CGRAM slot
void BuildCGRAM(u8 *p,u8 slot){

    u32 i;

    // CmdLCD(GOTO_CGRAM_START);       // Alternative CGRAM start command (currently commented)

    CmdLCD(0x40 | (slot << 3));        // Select CGRAM address corresponding to the requested character slot

    IOSET0=1<<LCD_RS;                  // Select LCD data register
    IOCLR0=1<<LCD_RW;                  // Select write operation

    for(i=0;i<8;i++){
        WriteLCD(p[i]);                // Write one row of the 8-row custom character pattern
    }

    CmdLCD(GOTO_LINE1_POS0);           // Return LCD address selection to the first DDRAM position
}
