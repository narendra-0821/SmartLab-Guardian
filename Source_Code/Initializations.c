
/*******************************************
Program: Initialization and RTC Support
Controller: LPC2148 (ARM7)

Purpose:
Configure external interrupts, initialize RTC,
configure LED and buzzer GPIO pins, and check
whether the current time is within laboratory
operating hours.
********************************************/

// Header file for LPC21xx register definitions
#include <LPC21xx.h>

// Header file for software delay functions
#include "delay.h"

// Header file for bit manipulation macros
#include "defines.h"
#include "lcd_16x2_defines.h"
#include "lcd_16x2.h"
// Header file for external interrupt configurations
#include "ExtInterruptSwitches.h"

// Header file for RTC functions
#include "rtc.h"

// Header file for project-specific definitions
#include "all_defines.h"


/*================ EXTERNAL VARIABLES ================*/

// RTC variables defined in another source file
extern s32 hour,min,sec,date,month,year,day;

// Variables containing laboratory operating time boundaries
extern u32 startTime, endTime;

/* To check status of LED1 and LED2 */
extern int led1_status;
extern int led2_status;


/*================ EXTERNAL INTERRUPT INITIALIZATION ================*/

void EINT_Init(){

    // Configure P0.1 as EINT0 and P0.30 as EINT3
    PINSEL0 &=~((3<<(1*2)));             // Clear function selection bits for P0.1
    PINSEL1 &=~(3 <<((30-16)*2));        // Clear function selection bits for P0.30

    PINSEL0|= EINT0_PIN_FUNC;            // Select EINT0 alternate function for P0.1
    PINSEL1|= EINT3_PIN_FUNC;            // Select EINT3 alternate function for P0.30

    // Configure EINT0 as FIQ
    // In Startup.s, replace the default FIQ handler:
    // FIQ_Handler     B       FIQ_Handler
    // With the following:
    // IMPORT  eint0_fiq_isr
    // FIQ_Handler     B       eint0_fiq_isr

    // VIC configuration
    VICIntSelect=1<<EINT0_VIC_CHNO;      // Select EINT0 as FIQ; other channels remain IRQ

    VICIntEnable |= 1<<EINT0_VIC_CHNO|1<<EINT3_VIC_CHNO; // Enable EINT0 and EINT3 interrupt channels

    VICDefVectAddr=(unsigned)eint3_nvirq_isr; // Assign EINT3 non-vectored IRQ service routine

    // External interrupt peripheral configuration
    // EXTINT=0;                         // Clear pending external interrupt flags (currently commented)

    EXTMODE=1<<3|1<<0;                  // Configure EINT3 and EINT0 as edge-triggered interrupts

    // EXTPOLAR=0;                       // Falling-edge triggering is the default configuration
}


/*================ RTC INITIALIZATION ================*/

void SetRTC(){

    SetRTCTimeInfo(15,2,0);            // Set RTC time: 16:44:00
    SetRTCDateInfo(12,11,2026);          // Set RTC date: 21/04/202

    SetRTCDay(THU);                     // Set the day of the week to Wednesday
}


/*================ RTC DISPLAY FUNCTION ================*/

void DisplayRTC(void){

    GetRTCTimeInfo(&hour,&min,&sec);    // Read current hours, minutes and seconds
    DisplayRTCTime(hour,min,sec);       // Display the retrieved time on LCD

    GetRTCDateInfo(&date,&month,&year); // Read current date, month and year
    DisplayRTCDate(date,month,year);    // Display the retrieved date on LCD

    GetRTCDay(&day);                    // Read the current day of the week
    DisplayRTCDay(day);                 // Display the retrieved day on LCD
}

void DisplayTimeBoundaries(void){
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("START TIME=");
	U32LCD(startTime);
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("END TIME=");
	U32LCD(endTime);
} 

/*================ LED AND BUZZER INITIALIZATION ================*/

void Init_LEDS(){

    // Configure LED and buzzer GPIO pins as output (IODIR bit = 1)
    IODIR0|= 1<<LED1_DEVICE1;           // Set LED1 pin as output
    IODIR0|= 1<<LED2_DEVICE2;           // Set LED2 pin as output
    IODIR0|= 1<<BUZZER;                 // Set buzzer pin as output

    /* Initially both LEDs are ON */
    IOCLR0 = 1<<LED1_DEVICE1;           // Clear LED1 output bit to turn ON LED1
    led1_status=1;                      // Update software status of LED1

    IOCLR0 = 1<<LED2_DEVICE2;           // Clear LED2 output bit to turn ON LED2
    led2_status = 1;                    // Update software status of LED2
	/* Initially both BUZZER are ON */
    IOCLR0 = 1<<BUZZER;                 // Set buzzer output bit
}


/*================ LABORATORY OPERATING TIME VALIDATION ================*/

unsigned int IsWithinTime(void)
{
    if(startTime < endTime){            // Normal schedule: start time is earlier than end time
        if(HOUR >= startTime && HOUR < endTime) // Check whether current hour is within the schedule
            return 1;                   // Return 1 if laboratory operating time is valid

    }else if(startTime > endTime) {     // Overnight schedule: operating hours cross midnight
        if(HOUR >= startTime || HOUR < endTime) // Check time before midnight OR after midnight
            return 1;                   // Return 1 if current hour is within the schedule
    }

    return 0;                           // Return 0 if current hour is outside operating hours
}

