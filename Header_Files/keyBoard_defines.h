
/*******************************************
File: keyBoard_defines.h
Controller: LPC2148 (ARM7)

Purpose:
Define GPIO pin numbers connected to the
row and column lines of the 4x4 matrix keypad.
********************************************/


/*================ KEYPAD ROW PIN DEFINITIONS ================*/

// GPIO pins connected to the four keypad rows (configured as outputs)
#define ROW0 16  // P1.16 - Keypad Row 0
#define ROW1 17  // P1.17 - Keypad Row 1
#define ROW2 18  // P1.18 - Keypad Row 2
#define ROW3 19  // P1.19 - Keypad Row 3


/*================ KEYPAD COLUMN PIN DEFINITIONS ================*/

// GPIO pins connected to the four keypad columns (configured as inputs)
#define COL0 20  // P1.20 - Keypad Column 0
#define COL1 21  // P1.21 - Keypad Column 1
#define COL2 22  // P1.22 - Keypad Column 2
#define COL3 23  // P1.23 - Keypad Column 3
