
/*******************************************
Program: Software Delay Functions
Controller: LPC2148 (ARM7)

Purpose:
Generate approximate microsecond, millisecond
and second delays using software loops.
********************************************/

// Header file for user-defined data types
#include "types.h"


/*================ MICROSECOND DELAY ================*/

// Generate an approximate delay in microseconds
void delay_us(u32 dlyUS){
    for(dlyUS*=12;dlyUS>0;dlyUS--);    // Execute approximately 12 loop iterations per microsecond
                                      // Assumption: CCLK = 60 MHz
}

/*================ MILLISECOND DELAY ================*/

// Generate an approximate delay in milliseconds
void delay_ms(u32 dlyMS){
    for(dlyMS*=12000;dlyMS>0;dlyMS--); // Execute approximately 12,000 loop iterations per millisecond
                                      // Assumption: CCLK = 60 MHz
}

/*================ SECOND DELAY ================*/

// Generate an approximate delay in seconds
void delay_s(u32 dlyS){
    for(dlyS*=12000000;dlyS>0;dlyS--); // Execute approximately 12,000,000 loop iterations per second
                                       // Assumption: CCLK = 60 MHz
}
