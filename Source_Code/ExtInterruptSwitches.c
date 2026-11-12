
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

// Header files for LCD interfacing functions and LCD commands
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

// Flags used to indicate that the corresponding interrupt has occurred
extern u32 emergency;
extern u32 rtc_button_clicked;

// Variables used to store the status of LED1 and LED2
extern int led1_status;
extern int led2_status;

// Variables used to store the received IR address, command and toggle bit
extern unsigned int address, command, toggle;

// Variables containing the laboratory operating start and end hours
extern u32 startTime, endTime;

// Variables used to store RTC date and time values
extern s32 hour,min,sec,date,month,year,day;

// Variable used to store the keypad key selected by the administrator
u8 key;

// Variables used to temporarily store the entered start and end hours
u32 sTime,eTime;

// Stores the previously detected keypad key
extern u8 lastKey;

// Indicates whether the administrator has successfully opened the lock
extern int Lockopened;


/*=========================================================
        EINT0 - EMERGENCY SWITCH (FIQ)
=========================================================*/

/* External Interrupt EINT0 functionality for EMERGENCY SWITCH */

void eint0_fiq_isr(void) __irq{

	// Set the emergency flag when the EINT0 interrupt occurs
	emergency=1;

	// Clear the EINT0 interrupt flag so that the interrupt can be detected again
	EXTINT=1<<0;
}


/*=========================================================
        EINT3 - RTC CONFIGURATION (IRQ)
=========================================================*/

/* External Interrupt EINT3 functionality for RTC CONFIGURATION */

void eint3_nvirq_isr(void) __irq{

	// Set the flag to indicate that the RTC configuration switch was pressed
	rtc_button_clicked=1;

	// Clear the EINT3 interrupt flag in the external interrupt controller
	EXTINT=1<<3;

	// Signal the end of the current interrupt service routine to the VIC
	VICVectAddr=0;
}




/*=========================================================
        EMERGENCY ALERT HANDLING
=========================================================*/

void emergencyAlert(void){

	/* TURNING OFF all devices and TURNING ON BUZZER FOR ALERT*/

	// Display the emergency notification on the LCD
	CmdLCD(CLEAR_LCD);
	StrLCD("!! EMERGENCY !!");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("ALL DEVICES OFF");

	// Switch OFF both devices and update their software status
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

	// Stop the buzzer after receiving the SOUND command
	BUZZER_OFF;

	// Clear the previously received IR command and address
	command=0;
	address=0;

	CmdLCD(CLEAR_LCD);

}


/*=========================================================
        RTC CONFIGURATION INTERRUPT HANDLING
=========================================================*/

void rtc_interrupt(void){

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
}



/*=========================================================
        RTC TIME EDITING
=========================================================*/

void editRTC(){

	// Display the RTC editing screen
	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET HOUR= ");
	CmdLCD(GOTO_LINE2_POS0+10);

	// Read the administrator-entered hour value
	hour=HOUR;
	ReadTime((u32*)&hour,&lastKey);

	// Accept the entered hour only if it is within 0-23
	// and different from the current RTC hour
	if(hour<=23 && hour!=HOUR) HOUR=hour;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	// Display the minute editing screen
	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET MIN = ");
	CmdLCD(GOTO_LINE2_POS0+10);

	// Read the administrator-entered minute value
	min=MIN;
	ReadTime((u32*)&min,&lastKey);

	// Accept the entered minute only if it is within 0-59
	// and different from the current RTC minute
	if(min<=59 && min!=MIN) MIN=min;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	// Display the second editing screen
	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET SEC= ");
	CmdLCD(GOTO_LINE2_POS0+10);

	// Read the administrator-entered second value
	sec=SEC;
	ReadTime((u32*)&sec,&lastKey);

	// Accept the entered second only if it is within 0-59
	// and different from the current RTC second
	if(sec<=59  && sec!=SEC) SEC=sec;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	// Display the date editing screen
	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET DATE= ");
	CmdLCD(GOTO_LINE2_POS0+10);

	// Read the administrator-entered date value
	date=DOM;
	ReadTime((u32*)&date,&lastKey);

	// Accept the entered date only if it is within the specified range
	if(date>0 && date<=31 && date!=DOM) DOM=date;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	// Display the month editing screen
	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET MONTH= ");
	CmdLCD(GOTO_LINE2_POS0+11);

	// Read the administrator-entered month value
	month=MONTH;
	ReadTime((u32*)&month,&lastKey);

	// Accept the entered month only if it is within 1-12
	if(month>0 && month<=12 && month!=MONTH) MONTH=month;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	// Display the year editing screen
	StrLCD("RTC EDIT MODE");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("SET YEAR= ");
	CmdLCD(GOTO_LINE2_POS0+11);

	// Read the administrator-entered year value
	year=YEAR;
	ReadYear((u32*)&year,&lastKey);

	// Accept the entered year only if it is within the specified range
	if(year>=1000 && year<=4056 && YEAR!=year) YEAR=year;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	return;
}


/*=========================================================
        LABORATORY OPERATING TIME EDITING
=========================================================*/

void editTimeBoundaries(){

	// Display the laboratory operating-time configuration screen
	StrLCD("Time Boundaries");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("START TIME=");
	CmdLCD(GOTO_LINE2_POS0+13);

	// Read the administrator-entered laboratory start hour
	sTime=startTime;
	ReadTime(&sTime,&lastKey);

	// Update the start time only if the entered hour is valid (0-23)
	if(sTime<=23 && startTime!=sTime) startTime=sTime;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	// Display the laboratory end-time configuration screen
	StrLCD("Time Boundaries");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("END TIME=");
	CmdLCD(GOTO_LINE2_POS0+12);

	// Read the administrator-entered laboratory end hour
	eTime=endTime;;
	ReadTime(&eTime,&lastKey);

	// Update the end time only if the entered hour is valid (0-23)
	if(eTime<=23 && endTime!=eTime) endTime=eTime;

	delay_s(1);
	CmdLCD(CLEAR_LCD);

	return;
}

