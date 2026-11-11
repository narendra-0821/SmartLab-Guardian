
/*******************************************
File: passwordCheck.h
Controller: LPC2148 (ARM7)

Purpose:
Declare functions for administrator PIN
authentication, changing the existing PIN,
and handling retry timeout messages.
********************************************/

// Header file for user-defined data types
#include "types.h"


/*================ PASSWORD AUTHENTICATION FUNCTIONS ================*/

// Allow the administrator to change the existing PIN
void changePIN(void);

// Authenticate the administrator using the entered PIN
void passWord(void);

// Display the retry countdown after multiple incorrect PIN attempts
void TryAgain(void);
