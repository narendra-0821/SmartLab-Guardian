
/*******************************************
File: IR_Receiver.h
Controller: LPC2148 (ARM7)

Purpose:
Declare RC5 infrared receiver functions
and define TSOP receiver pin configuration,
RC5 bit timings and polling interval.
********************************************/


/*================ IR RECEIVER FUNCTION DECLARATIONS ================*/

// Receive and decode an RC5 frame and return address, command and toggle values
unsigned char RC5_Receive(unsigned int *address,
                          unsigned int *command,
                          unsigned int *toggle);

// Wait until the TSOP output remains HIGH for the required idle duration
static void WaitIdle(void);

// Wait for the TSOP signal to change from the specified logic level within the time limit
static unsigned char WaitEdge(unsigned int lvl, unsigned int max_us);

// Read the TSOP receiver, validate the received frame and return its status
int readTSOP(unsigned int *address,unsigned int *command, unsigned int *toggle);


/*================ RC5 SETTINGS ================*/

// TSOP receiver output connected to GPIO P0.20
#define TSOP_PIN 20                     // TSOP output is connected to P0.20 (moved away from the LCD RS pin P0.16)

// Read TSOP output and shift bit 20 to bit 0; mask the remaining bits to obtain 0 or 1
#define RC5_PIN  ((IO0PIN >> TSOP_PIN)&1)

// RC5 timing constants
#define RC5_T   1778                    // Full RC5 bit time = 1778 microseconds
#define RC5_H   889                     // Half bit time = 889 microseconds (T/2)
#define RC5_Q   444                     // Quarter bit time = 444 microseconds (T/4)
#define STEP_US 20                      // Polling interval in microseconds while waiting for a signal edge
