
/*******************************************
Program: 4x4 Matrix Keypad Interfacing
Controller: LPC2148 (ARM7)

Purpose:
Interface a 4x4 matrix keypad, identify the
pressed key, and read numeric inputs for
RTC configuration and password authentication.
********************************************/

// Header file for LPC21xx register definitions
#include <LPC21xx.h>

// Header file for user-defined data types
#include "types.h"

// Header file for bit manipulation macros
#include "defines.h"

// Header file for keypad pin configuration and definitions
#include "keyBoard_defines.h"

// Header files for LCD interfacing and display commands
#include "lcd_16x2.h"
#include "lcd_16x2_defines.h"

// Header file for software delay functions
#include "delay.h"


/*================ KEYPAD LOOKUP TABLE ================*/

/* Original numeric lookup table
u8 kpmLUT[4][4]=
{
    {0,1,2,3},
    {4,5,6,7},
    {8,9,10,11},
    {12,13,14,15}
};
*/

// Maps the detected row and column positions to their corresponding keypad characters
u8 kpmLUT[4][4]=
{
    {'7','8','9','/'},
    {'4','5','6','*'},
    {'1','2','3','-'},
    {'C','0','=','+'}
};


/*================ KEYPAD INITIALIZATION ================*/

void InitKPM(void){

    // Configure all four row pins as output and drive them LOW
    WRITENIBBLE(IODIR1,ROW0,0XF);

    // Column pins are configured as inputs and remain HIGH by default
}


/*================ COLUMN SCANNING ================*/

// Check whether any column detects a key press
u8 ColScan(void){
    u8 t;
    t=(READNIBBLE(IOPIN1,COL0)<15)?0:1; // If any column is LOW, return 0; otherwise return 1
    return t;
}


/*================ ROW IDENTIFICATION ================*/

// Identify the row containing the pressed key
u8 RowCheck(void){
    u8 r;
    for(r=0;r<4;r++){

        // Drive one row LOW at a time while keeping the other rows HIGH
        WRITENIBBLE(IOPIN1,ROW0,~(1<<r));

        // Check whether the pressed key pulls any column LOW
        if(!ColScan())
            break;
    }

    // Restore all row outputs to LOW after scanning
    WRITENIBBLE(IOPIN1,ROW0,0);
    return r;                          // Return the detected row index (0 to 3)
}


/*================ COLUMN IDENTIFICATION ================*/

// Identify the column containing the pressed key
u8 ColCheck(void){
    u8 c;
    for(c=0;c<4;c++){
        if(READBIT(IOPIN1,(COL0+c))==0) // A LOW input indicates the pressed key's column
            break;
    }
    return c;                          // Return the detected column index (0 to 3)
}


/*================ KEYPAD KEY SCANNING ================*/

// Scan the keypad and return the character corresponding to the pressed key
u8 KeyScan(void){
    u8 r,c,keyV;

    while(ColScan());                  // Wait until any key is pressed

    r=RowCheck();                      // Identify the pressed key's row
    c=ColCheck();                      // Identify the pressed key's column

    keyV=kpmLUT[r][c];                 // Retrieve the key character using row and column indices
    return keyV;
}


/*================ READ RTC TIME INPUT ================*/

// Read numeric input from the keypad during RTC time configuration
void ReadTime(u32 *num,u8 *lastKey){

    u32 cnt=0;
    u32 num2=0;

    while(1){
        *lastKey=KeyScan();            // Store the most recently pressed key

        /* Accept numeric keys from 0 to 9, allowing a maximum of two digits */
        if(((*lastKey>='0' && *lastKey<='9'))&&(cnt<2)){
            num2=(num2 * 10)+(*lastKey-'0'); // Build the number by shifting previous digits left by one decimal place
            CmdLCD(GOTO_LINE2_POS0 + cnt++ + 10);
            CharLCD(*lastKey);         // Display the entered digit on LCD
            *num=num2;                 // Update the output with the entered number
            while(ColScan()==0);       // Wait until the pressed key is released

        }else{
            if(*lastKey=='C'){         //'C' clears the entered number
                CmdLCD(CLEAR_LCD);
                cnt=0;
                *num=0;
                num2=0;

            }else if(*lastKey=='*' && cnt>0){ //' * ' deletes the last entered digit
                num2=num2/10;          // Remove the last decimal digit
                --cnt;                 // Move the cursor position back
                CmdLCD(GOTO_LINE2_POS0 + cnt + 10);
                CharLCD(' ');          // Clear the previous digit from LCD
                *num=num2;             // Update the output after deletion

            }else if(*lastKey == '='){ // '=' acts as the enter key
                break;
            }

            while(ColScan()==0);       // Wait until the pressed key is released
        }
    }
}


/*================ READ PASSWORD / NUMERIC INPUT ================*/

// Read a maximum of four numeric digits for password authentication
void ReadNums(u32 *num,u8 *lastKey){
    u32 cnt=0;
    *num=0;                            // Initialize the entered number

    while(1){
        *lastKey=KeyScan();            // Read the pressed keypad character

        /* Accept numeric keys from 0 to 9, up to four digits */
        if(((*lastKey>='0' && *lastKey<='9'))&&(cnt<4)){
            *num=(*num*10)+(*lastKey-'0'); // Build the four-digit number
            CmdLCD(GOTO_LINE2_POS0 + cnt++);
            CharLCD('*');              // Display '*' instead of the actual password digit
            while(ColScan()==0);        // Wait until the pressed key is released

        }else{
            if(*lastKey=='C'){          //'C' clears the entered password
                CmdLCD(CLEAR_LCD);
                StrLCD("enter PIN 4digits");
                cnt=0;
                *num=0;

            }else if(*lastKey=='*' && cnt>0){ //' * ' deletes the last entered digit
                *num=*num/10;           // Remove the last decimal digit
                --cnt;                  // Move the cursor position back
                CmdLCD(GOTO_LINE2_POS0 + cnt);
                CharLCD(' ');           // Clear the previous '*' from LCD

            }else if(*lastKey== '='){   //'=' acts as the enter key
                if(cnt!=4) {            // Check whether exactly four digits were entered
                    CmdLCD(CLEAR_LCD);
                    StrLCD("Please Enter");
                    CmdLCD(GOTO_LINE2_POS0);
                    StrLCD("4Digits To UNLOCK");
                    delay_s(2);
                    break;
                }

                if(*lastKey== '=' && cnt==4)
                    break;              // Accept the input when four digits are entered
            }

            while(ColScan()==0);        // Wait until the pressed key is released
        }
    }
}

// Read numeric input from the keypad during RTC year configuration
void ReadYear(u32 *num,u8 *lastKey){

    u32 cnt=0;
    u32 num2=0;

    while(1){
        *lastKey=KeyScan();            // Store the most recently pressed key

        /* Accept numeric keys from 0 to 9, allowing a maximum of four digits */
        if(((*lastKey>='0' && *lastKey<='9'))&&(cnt<4)){
            num2=(num2 * 10)+(*lastKey-'0'); // Build the number by shifting previous digits left by one decimal place
            CmdLCD(GOTO_LINE2_POS0 + cnt++ + 10);
            CharLCD(*lastKey);         // Display the entered digit on LCD
            *num=num2;                 // Update the output with the entered number
            while(ColScan()==0);       // Wait until the pressed key is released

        }else{
            if(*lastKey=='C'){         //'C' clears the entered number
                CmdLCD(CLEAR_LCD);
                cnt=0;
                *num=0;
                num2=0;

            }else if(*lastKey=='*' && cnt>0){ //' * ' deletes the last entered digit
                num2=num2/10;          // Remove the last decimal digit
                --cnt;                 // Move the cursor position back
                CmdLCD(GOTO_LINE2_POS0 + cnt + 10);
                CharLCD(' ');          // Clear the previous digit from LCD
                *num=num2;             // Update the output after deletion

            }else if(*lastKey == '='){ // '=' acts as the enter key
                break;
            }

            while(ColScan()==0);       // Wait until the pressed key is released
        }
    }
}
