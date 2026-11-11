
/*******************************************
File: delay.h
Controller: LPC2148 (ARM7)

Purpose:
Declare software delay functions used to
generate delays in microseconds, milliseconds
and seconds.
********************************************/

// Header file for user-defined data types
#include "types.h"


/*================ DELAY FUNCTION DECLARATIONS ================*/

// Generate a delay in microseconds
void delay_us(u32 dlyUS);

// Generate a delay in milliseconds
void delay_ms(u32 dlyMS);

// Generate a delay in seconds
void delay_s(u32 dlyS);
