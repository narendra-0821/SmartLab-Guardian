
/*******************************************
File: all_defines.h
Controller: LPC2148 (ARM7)

Purpose:
Define project-wide hardware pin numbers,
LED and buzzer control macros, IR remote
command values, external interrupt settings,
and day-of-week constants.
********************************************/

// Header file for LPC21xx peripheral register definitions
#include <LPC21xx.h>

// Header file for user-defined data types
#include "types.h"


/*================ OUTPUT DEVICE PIN DEFINITIONS ================*/

// GPIO pin numbers connected to the output devices
#define LED1_DEVICE1 5  // P0.5 connected to Device 1 indication LED
#define LED2_DEVICE2 4  // P0.4 connected to Device 2 indication LED
#define BUZZER       24 // P1.24 connected to emergency buzzer


/*================ LED CONTROL MACROS ================*/

// Toggle LED1 output by inverting its current pin value
#define LED1_TOGGLE (IOPIN0^=(1<<LED1_DEVICE1))

// Toggle LED2 output by inverting its current pin value
#define LED2_TOGGLE (IOPIN0^=(1<<LED2_DEVICE2))


/*================ IR REMOTE COMMAND DEFINITIONS ================*/

// RC5 command values assigned to the remote-control buttons
#define IR_KEY_1 1
#define IR_KEY_2 2
#define IR_KEY_TIME 47

// Check whether the received IR address and command match Device 1 control
#define KEY_1_PRESSED (address==0 && command==IR_KEY_1)

// Check whether the received IR address and command match Device 2 control
#define KEY_2_PRESSED (address==0 && command==IR_KEY_2)

// Check whether the received IR address and command match RTC display control
#define TIME_KEY_PRESSED (address==0 && command==IR_KEY_TIME)

// Check whether the received IR address and command match the buzzer sound key
#define SOUND_KEY_PRESSED (address==3 && command==11)


/*================ LED AND BUZZER STATUS DEFINITIONS ================*/

// Constant representing the ON state in software LED status variables
#define LED_ON 1

// Turn OFF LED1 and LED2 by setting their GPIO output bits
#define AL_LED1_OFF IOSET0=1<<LED1_DEVICE1
#define AL_LED2_OFF IOSET0=1<<LED2_DEVICE2

// Control the buzzer using its GPIO output bit
#define BUZZER_ON IOSET1=1<<BUZZER   // Set buzzer output bit
#define BUZZER_OFF IOCLR1=1<<BUZZER  // Clear buzzer output bit


/*================ EXTERNAL INTERRUPT DEFINITIONS ================*/

// Pin selection and VIC channel definitions for EINT0 and EINT3

// EINT0 configuration
#define EINT0_PIN_0_1 0              // Pin identifier for P0.1
#define EINT0_PIN_FUNC 0x0000000C    // PINSEL0 value to select EINT0 function
#define EINT0_VIC_CHNO 14            // VIC interrupt channel number for EINT0

// EINT3 configuration
#define EINT3_PIN_0_9 0              // Pin identifier as defined in this project
#define EINT3_PIN_FUNC 0x25400000    // PINSEL1 value used to select EINT3 function
#define EINT3_VIC_CHNO 17            // VIC interrupt channel number for EINT3


/*================ DAY OF WEEK DEFINITIONS ================*/

// Enumeration assigns numeric values to the days of the week (SUN = 0 to SAT = 6)
enum days{SUN,MON,TUE,WED,THU,FRI,SAT};
