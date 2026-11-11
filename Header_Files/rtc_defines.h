
/*******************************************
File: rtc_defines.h
Controller: LPC2148 (ARM7)

Purpose:
Define system clock and peripheral clock
frequencies, RTC prescaler values, RTC control
register bits, and target CPU selection.
********************************************/

// Header file for user-defined data types
#include "types.h"


/*================ SYSTEM CLOCK DEFINITIONS ================*/

// External oscillator frequency: 12 MHz
#define FOSC 12000000

// CPU clock frequency: FOSC multiplied by PLL factor (5)
#define CCLK (FOSC*5)

// Peripheral clock frequency: CPU clock divided by 4
#define PCLK (CCLK/4)


/*================ RTC PRESCALER CALCULATION ================*/

/*
RTC requires a 32.768 kHz clock for a one-second counter increment.

PREINT and PREFRAC registers are used to divide PCLK
to generate the required RTC clock frequency.

PREINT = int(PCLK / 32768) - 1
PREFRAC = PCLK - ((PREINT + 1) * 32768)

Prescaler calculation based on the LPC2129 user manual.
*/

// Calculate the integer part of the RTC prescaler value
#define PREINT_VAL  ((int) ((PCLK / 32768) - 1))

// Calculate the fractional part of the RTC prescaler value
#define PREFRAC_VAL (PCLK -((PREINT_VAL + 1) * 32768))


/*================ RTC CONTROL REGISTER (CCR) BITS ================*/

// Bit 0: Enable or disable the RTC counters
// 1 = Enable RTC counters, 0 = Disable RTC counters
#define RTC_ENABLE (1<<0)

// Bit 1: Reset the RTC counters
// 1 = Reset RTC counters, 0 = Normal operation
#define RTC_RESET (1<<1)


// LPC2148-specific clock source selection
// Bit 4: Select the RTC clock source
// 1 = External 32.768 kHz oscillator
// 0 = Internal PCLK as RTC clock source
#define RTC_CLKSRC  (1<<4)


/*================ DAY OF WEEK DEFINITIONS ================*/

// Assign numeric values to weekdays (SUN = 0 to SAT = 6)
enum days{SUN,MON,TUE,WED,THU,FRI,SAT};


/*================ CPU SELECTION DEFINITIONS ================*/

// Numeric identifiers used to select the target CPU configuration
#define LPC2129 0
#define LPC2148 1

// Select LPC2129 as the target CPU for conditional compilation
#define CPU LPC2129
