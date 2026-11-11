
/*******************************************
Program: IR Remote Receiver using RC5 Protocol
Controller: LPC2148 (ARM7)

Purpose:
Receive infrared signals through TSOP receiver,
decode RC5 protocol frames, extract address,
command and toggle information, and filter
duplicate remote-control commands.
********************************************/

// Header file for LPC214x register definitions
#include <LPC214x.h>
#include "delay.h"
#include "defines.h"
// Header file for IR receiver pin definitions and function declarations
#include "IR_Receiver.h"


/*================ DUPLICATE FILTER =================*/

// Store the information of the last accepted IR frame
unsigned int last_address = 0;          // Address of the last accepted frame
unsigned int last_command = 0;          // Command of the last accepted frame
unsigned int last_toggle  = 0;          // Toggle bit of the last accepted frame
unsigned char previous_key_valid = 0;   // Flag: 1 once a frame has been stored


/*================ EDGE / IDLE HELPERS ================*/

/* Wait until the line changes from 'lvl'. Returns 1 if the edge came within max_us, else 0 */

static unsigned char WaitEdge(unsigned int lvl, unsigned int max_us)
{
    unsigned int t = 0;                 // Elapsed time in us, starts at 0

    while(t < max_us)                   // Keep checking until the time limit is reached
    {
        if(RC5_PIN != lvl)              // Has the pin level changed from the starting level?
            return 1;                   // Yes: edge found, report success

        delay_us(STEP_US);              // No: wait one polling step
        t += STEP_US;                   // Add the step to the elapsed time
    }

    return 0;                           // Time ran out with no edge: report failure
}


/* Wait until the line has stayed HIGH for 4 ms (frame is over) */

static void WaitIdle(void)
{
    unsigned int t = 0;                 // Time the line has been continuously HIGH
	unsigned int timeout = 0;

    // Continue checking until the idle duration or maximum timeout is reached
    while(t < 4000 && timeout < 20000)
    {
        if(RC5_PIN == 0)                // Is the line LOW (IR burst still present)?
            t = 0;                      // Yes: restart the idle timer
        else                            // Line is HIGH
            t += STEP_US;               // Add one step to the idle time

        delay_us(STEP_US);              // Wait one polling step before checking again
		timeout += STEP_US;             // Track the total waiting time
	}
}


/*================ RC5 RECEIVE =================*/

/* Call right after the first falling edge (middle of start bit S1) */

unsigned char RC5_Receive(unsigned int *address,   // Output: 5-bit address
                          unsigned int *command,   // Output: 6-bit command
                          unsigned int *toggle)    // Output: toggle bit
{
    unsigned int data = 1;              // Frame data, starts with the start bit S1 = 1
    unsigned int i, lvl;                // i = bit counter, lvl = pin level before an edge

    // Wait until the sampling position inside the next RC5 bit
    delay_us(RC5_T - RC5_Q);            // Move from mid S1 to 0.75T, inside the first half of the next bit

    // Receive the remaining 13 bits of the RC5 frame
    for(i = 0; i < 13; i++)             // Receive S2, toggle, 5 address bits and 6 command bits
    {
        lvl = RC5_PIN;                  // Remember the current pin level

        // Wait for the next signal transition within one bit duration
        if(!WaitEdge(lvl, RC5_T))       // Wait for the mid-bit transition (resync point)
            return 0;                   // No edge within one bit time: bad frame, give up

        // Delay to reach a stable sampling position after the transition
        delay_us(RC5_Q);                // Wait a quarter bit after the edge, into the stable second half

        if(RC5_PIN == 0)                // Sample the pin: LOW means 1 (TSOP output is inverted)
            data = (data << 1) | 1;     // Shift data left and add a 1
        else                            // Pin is HIGH, so the bit is 0
            data = (data << 1);         // Shift data left and add a 0

        delay_us(RC5_H);                // Wait half a bit to reach 0.75T of the next bit
    }

    // Validate the second start bit before accepting the received frame
    if(((data >> 12) & 1) != 1)         // Check the second start bit S2 (bit 12), must be 1
        return 0;                       // Invalid start bit: reject the frame

    // Extract individual fields from the received RC5 frame
    *toggle  = (data >> 11) & 1;        // Extract the toggle bit (bit 11)
    *address = (data >> 6)  & 0x1F;     // Extract the 5 address bits (bits 10-6)
    *command =  data        & 0x3F;     // Extract the 6 command bits (bits 5-0)

    return 1;                           // Report that a valid frame was received
}


/*================ TSOP RECEIVER AND DUPLICATE FILTER =================*/

int readTSOP(unsigned int *address,unsigned int *command, unsigned int *toggle ){

    // Configure P0.20 as GPIO instead of its alternate peripheral function
    PINSEL1 &= ~(3 << 8);               // Set P0.20 to GPIO function (its function bits are 9:8 of PINSEL1)

    // Configure the TSOP receiver pin as input
    IO0DIR  &= ~(1 << TSOP_PIN);        // Make P0.20 an input (TSOP output)

		// Check whether an IR signal has started
		if(RC5_PIN == 1) return 0;            // Idle HIGH: return until the first falling edge is detected

		// Decode one received RC5 frame
		if(RC5_Receive(address,command,toggle))   // Decode one frame; true if it was valid
		{
					// Accept the frame only if it is not an exact duplicate
					// of the previously accepted address, command and toggle combination
			if(!(previous_key_valid &&          // Was there a previous key AND...
					 *address == last_address &&     // ...same address AND...
					 *command == last_command &&     // ...same command AND...
					 *toggle  == last_toggle))       // ...same toggle? If so it is a repeat, so skip it
			{
							// Store the newly accepted frame information for future comparison
				last_address = *address;         // Save this address for the next comparison
				last_command = *command;         // Save this command for the next comparison
				last_toggle  = *toggle;          // Save this toggle bit for the next comparison
				previous_key_valid = 1;         // Mark that a stored key now exists

				// Wait until the current IR frame has ended
				WaitIdle();

				return 1;                       // Report that a new valid command was received
			}
		}

		// Wait for the IR signal to become idle before the next reception attempt
		WaitIdle();                     // Wait until the line is HIGH for 4 ms so the same frame is not decoded twice

		return 0;                       // No new valid command was accepted
}
