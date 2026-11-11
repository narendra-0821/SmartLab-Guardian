
/*******************************************
Program: Password Authentication System
Controller: LPC2148 (ARM7)

Purpose:
Implement keypad-based password authentication,
allow administrators to check and change the PIN,
and restrict access after multiple failed attempts.
********************************************/

// Header file for LPC21xx register definitions
#include <LPC21xx.h>

// Header file for user-defined data types
#include "types.h"

// Header file for bit manipulation macros and common definitions
#include "defines.h"

// Header file for software delay functions
#include "delay.h"

// Header file for password authentication function declarations
#include "passwordCheck.h"

// Header file for keypad pin and configuration definitions
#include "keyBoard_defines.h"

// Header file for keypad interfacing functions
#include "keyBoard.h"

// Header file for LCD interfacing functions
#include "lcd_16x2.h"

// Header file for LCD commands and cursor position definitions
#include "lcd_16x2_defines.h"


/*=========================================================
        LCD CUSTOM CHARACTER AND USER PIN
=========================================================*/

// CGRAM symbol for smile emoji
u8 smileLUT[8]=
{
0x00,0x0A,0x0A,0x00,0x11,0x0E,0x00,0x00
};

// Store the default user PIN
u32 userPIN=9999;

/*=========================================================
        PASSWORD AUTHENTICATION
=========================================================*/

/* Function to check whether the entered password is correct or not */

void passWord(){
			u32 password=0,attempts=1;
			u8 lastKey,key;

			CmdLCD(CLEAR_LCD);
			StrLCD("   Enter PIN");
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD(" To UNLOCK LAB");
			delay_s(3);

			while(1){

/* Giving 2 options for admin:
   CHECK PIN AND CHANGE PIN */

// FIRST label: Entry point for displaying the password menu
FIRST:	CmdLCD(CLEAR_LCD);
				StrLCD("1.Check PIN");
				CmdLCD(GOTO_LINE2_POS0);
				StrLCD("2.Change PIN");

				// Wait until a keypad key is pressed
				while(ColScan()!=0){
					key=KeyScan();                 // Read the pressed key
				}

				// Check whether the administrator selected Check PIN
				if(key=='1'){
					delay_ms(500);

// TRY_AGAIN label: Entry point for password verification
TRY_AGAIN:    // If attempts are more than 3, display a warning message to admin

					if(attempts>3){               // Check whether the allowed attempt count is exceeded
						CmdLCD(CLEAR_LCD);
						StrLCD("TOO MANY ATTEMPTS");
						TryAgain();               // Display the retry countdown
						CmdLCD(CLEAR_LCD);
						attempts=0;               // Reset the attempt counter
						goto FIRST;               // Return to the password menu
					}

					CmdLCD(CLEAR_LCD);
					StrLCD("enter PIN 4digits");
					CmdLCD(GOTO_LINE2_POS0);

					// Read the PIN entered through the keypad
					ReadNums(&password,&lastKey);

					/* Authenticate user with entered PIN */

					// Compare the entered password with the stored user PIN
					if(password==userPIN){

						CmdLCD(CLEAR_LCD);
						StrLCD(" Password Match");

						// Load the smile emoji pattern into LCD CGRAM
						BuildCGRAM(smileLUT,8);

						// Display the custom smile character four times
						CmdLCD(GOTO_LINE2_POS0+4);
						CharLCD(0);CharLCD(0);CharLCD(0);CharLCD(0);

						delay_s(3);
						CmdLCD(CLEAR_LCD);

						// Return to the calling function after successful authentication
						return;

					}else{

						// Increment the failed password attempt counter
						attempts+=1;

						CmdLCD(CLEAR_LCD);
						StrLCD("Password Fail");
						delay_s(3);
						CmdLCD(CLEAR_LCD);

						// Return to ENTER to allow another password attempt
						goto TRY_AGAIN;
					}

			// Check whether the administrator selected Change PIN
			}else if(key=='2'){
				delay_ms(500);
				changePIN();                   // Call the function to change the current PIN
			}
	}
}


/*=========================================================
        CHANGE PASSWORD / PIN
=========================================================*/

/* Function to change PIN/PASSWORD */

void changePIN(){

	u32 oldPIN=0,newPIN=0,verifyNewPIN=0;
	u8 lastKey;

	CmdLCD(CLEAR_LCD);
	StrLCD("Enter Old PIN");
	CmdLCD(GOTO_LINE2_POS0);

	// Read the currently stored PIN entered by the administrator
	ReadNums(&oldPIN,&lastKey);

	delay_s(1);

	// Check whether the entered old PIN matches the stored user PIN
	if(oldPIN==userPIN){

		CmdLCD(CLEAR_LCD);
		StrLCD("Enter new PIN");
		CmdLCD(GOTO_LINE2_POS0);

		// Read the new PIN entered by the administrator
		ReadNums(&newPIN,&lastKey);

		delay_s(1);
		CmdLCD(CLEAR_LCD);
		StrLCD("Re Enter PIN");
		CmdLCD(GOTO_LINE2_POS0);

		// Read the new PIN again for confirmation
		ReadNums(&verifyNewPIN,&lastKey);

		delay_s(1);

		// Compare the new PIN with the re-entered confirmation PIN
		if(verifyNewPIN==newPIN){

			CmdLCD(CLEAR_LCD);
			StrLCD(" Password Changed");

			// Display the custom smile emoji after successful PIN change
			BuildCGRAM(smileLUT,8);
			CmdLCD(GOTO_LINE2_POS0+4);
			CharLCD(0);CharLCD(0);CharLCD(0);CharLCD(0);

			userPIN=newPIN;                  // Update the stored PIN with the new PIN

			delay_s(2);
			return;

		}else{

			// Display an error if the two new PIN entries do not match
			CmdLCD(CLEAR_LCD);
			StrLCD("PIN not Matched");
			delay_s(2);
		}

	}else{

		// Display an error if the entered old PIN is incorrect
		CmdLCD(CLEAR_LCD);
		StrLCD("PIN change Fail");
		delay_s(3);
		CmdLCD(CLEAR_LCD);
	}
}


/*=========================================================
        RETRY COUNTDOWN
=========================================================*/

void TryAgain(void){

	// Display the retry waiting time as a countdown
	  CmdLCD(GOTO_LINE2_POS0);
		StrLCD("TRY AGAIN 5S");
		delay_s(1);

		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("TRY AGAIN 4S");
		delay_s(1);

		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("TRY AGAIN 3S");
		delay_s(1);

		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("TRY AGAIN 2S");
		delay_s(1);

		CmdLCD(GOTO_LINE2_POS0);
		StrLCD("TRY AGAIN 1S");
		delay_s(1);
}
