
/*******************************************
Program: External Interrupt and Switch Control
Controller: LPC2148 (ARM7)

Purpose:
Handle emergency shutdown through EINT0,
RTC configuration through EINT3, and allow
the administrator to edit RTC time and
laboratory operating time boundaries.
********************************************/

// Header file for LPC2148 register definitions
#include <LPC21xx.h>

// Header file for LCD interfacing functions and LCD commands
#include "lcd_16x2.h"
#include "lcd_16x2_defines.h"

// Header file for software delay functions
#include "delay.h"

// Header file for bit manipulation macros and common definitions
#include "defines.h"

// Header file for external interrupt and switch function declarations
#include "ExtInterruptSwitches.h"

// Header file for keypad interfacing functions
#include "keyBoard.h"

// Header file for project-specific macros, pin definitions and constants
#include "all_defines.h"

// Header file for IR receiver functions and remote command definitions
#include "IR_Receiver.h"

// Header file for password authentication functions
#include "passwordCheck.h"


/*=========================================================
        EXTERNAL VARIABLE DECLARATIONS
=========================================================*/

/* To check status of LED1 and LED2 */
extern int led1_status;
extern int led2_status;

extern unsigned int address, command, toggle;
extern u32 startTime, endTime;
extern s32 hour,min,sec,date,month,year,day;

u8 key;
u32 sTime,eTime;
extern u8 lastKey;
extern int Lockopened;


/*=========================================================
        EINT0 - EMERGENCY SWITCH (FIQ)
=========================================================*/

/* External Interrupt EINT0 functionality for EMERGENCY SWITCH */

void eint0_fiq_isr(void) __irq{

	// ISR activity start

	/* TURNING OFF all devices and TURNING ON BUZZER FOR ALERT*/

	// Display emergency notification on LCD
	CmdLCD(CLEAR_LCD);
	StrLCD("!! EMERGENCY !!");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("ALL DEVICES OFF");

	AL_LED1_OFF;                         // Turn OFF Device 1
	AL_LED2_OFF;                         // Turn OFF Device 2
	led1_status=0;                       // Update software status of LED1
	led2_status=0;                       // Update software status of LED2
	BUZZER_ON;                           // Activate buzzer for emergency alert

	/* Wait until user clicks SOUND button on remote,
	   if they click SOUND button, come back to original position */

	// Continuously receive IR commands until the SOUND key is detected
	while(!(SOUND_KEY_PRESSED)){
		readTSOP(&address,&command,&toggle);
	}

	BUZZER_OFF;                          // Stop the emergency buzzer
	command=0;                           // Clear the previously received IR command
	address=0;                           // Clear the previously received IR address

	CmdLCD(CLEAR_LCD);

	// ISR activity end

	// Clear EINT0 interrupt flag in the external interrupt peripheral
	EXTINT=1<<0;
}


/*=========================================================
        EINT3 - RTC CONFIGURATION (IRQ)
=========================================================*/

/* External Interrupt EINT3 functionality for RTC CONFIGURATION */

void eint3_nvirq_isr(void) __irq{

	// ISR activity start

	// Check whether the lock is closed
	// If Lockopened is 0, password authentication is required
	if(!(Lockopened)){
		passWord();
		Lockopened=1;                     // Mark the lock as opened after authentication
	}

	/* If time boundaries are within the START and END TIME,
	   then admin need to change required TIME AND DATE */

	// Display the RTC configuration menu
	CmdLCD(CLEAR_LCD);
	StrLCD("1.EDIT RTC");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("2.TIME BOUNDARIES");

	// Wait until a keypad key is pressed
	while(ColScan()!=0){
		key=KeyScan();                    // Read the pressed key from the keypad
	}
			
	// Check whether the administrator selected RTC editing
	if(key=='1'){
		delay_ms(500);
		CmdLCD(CLEAR_LCD);
		editRTC();                        // Enter RTC time editing mode
		CmdLCD(CLEAR_LCD);
	}

	// Check whether the administrator selected operating-time editing
	if(key=='2'){
		delay_ms(500);
		CmdLCD(CLEAR_LCD);
		editTimeBoundaries();             // Enter laboratory time-boundary editing mode
		CmdLCD(CLEAR_LCD);
	}

	// Clear EINT3 interrupt flag in the external interrupt peripheral
	EXTINT=1<<3;

	// ISR activity end

	// Signal end of interrupt to the VIC
	VICVectAddr=0;
}


/*=========================================================
        RTC TIME EDITING
=========================================================*/

void editRTC(){

	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET HOUR= ");
	CmdLCD(GOTO_LINE2_POS0+10);

	// Read the administrator-entered hour value
	ReadTime((u32*)&hour,&lastKey);

	// Accept the entered hour only if it is within 0-23
	// and different from the current RTC hour
	if(hour<=23 && hour!=HOUR) HOUR=hour;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET MIN = ");
	CmdLCD(GOTO_LINE2_POS0+10);

	// Read the administrator-entered minute value
	ReadTime((u32*)&min,&lastKey);

	// Accept the entered minute only if it is within 0-59
	// and different from the current RTC minute
	if(min<=59 && min!=MIN) MIN=min;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET SEC= ");
	CmdLCD(GOTO_LINE2_POS0+10);

	// Read the administrator-entered second value
	ReadTime((u32*)&sec,&lastKey);

	// Accept the entered second only if it is within 0-59
	// and different from the current RTC second
	if(sec<=59  && sec!=SEC) SEC=sec;

	delay_s(1);
	CmdLCD(CLEAR_LCD);
	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET DATE= ");
	CmdLCD(GOTO_LINE2_POS0+10);

	// Read the administrator-entered date value
	ReadTime((u32*)&date,&lastKey);

	// Accept the entered date only if it is within 1-31
	if(date>=0 && date<=31 && date!=DOM) DOM=date;
	delay_s(1);
	CmdLCD(CLEAR_LCD);
	
	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET MONTH= ");
	CmdLCD(GOTO_LINE2_POS0+11);

	// Read the administrator-entered month value
	ReadTime((u32*)&month,&lastKey);

	// Accept the entered month only if it is within 1-12
	if(month>0 && month<=12 && month!=MONTH) MONTH=month;
	delay_s(1);
	CmdLCD(CLEAR_LCD);
	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET YEAR= ");
	CmdLCD(GOTO_LINE2_POS0+11);

	// Read the administrator-entered month value
	ReadTime((u32*)&year,&lastKey);

	// Accept the entered month only if it is within 1-12
	if(year>=1000 && year<=4056 && YEAR!=MONTH) YEAR=year;
	delay_s(1);
	CmdLCD(CLEAR_LCD);
	return;
}


/*=========================================================
        LABORATORY OPERATING TIME EDITING
=========================================================*/

void editTimeBoundaries(){

	StrLCD("Time Boundaries");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("START TIME=");
	CmdLCD(GOTO_LINE2_POS0+13);

	// Read the administrator-entered laboratory start hour
	ReadTime(&sTime,&lastKey);

	// Update the start time only if the entered hour is valid (0-23)
	if(sTime<=23) startTime=sTime;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	StrLCD("Time Boundaries");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("END TIME=");
	CmdLCD(GOTO_LINE2_POS0+12);

	// Read the administrator-entered laboratory end hour
	ReadTime(&eTime,&lastKey);

	// Update the end time only if the entered hour is valid (0-23)
	if(eTime<=23) endTime=eTime;

	delay_s(1);
	CmdLCD(CLEAR_LCD);
	return;
}
