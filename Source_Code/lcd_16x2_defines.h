
/*******************************************
File: lcd_16x2_defines.h
Controller: LPC2148 (ARM7)

Purpose:
Define HD44780 LCD commands and GPIO pin
numbers used for LCD data and control signals.
********************************************/


/*================ HD44780 LCD COMMAND DEFINITIONS ================*/

// Clear all characters from the LCD display
#define CLEAR_LCD           0x01

// Return the cursor to the home position (first row, first column)
#define RET_CUR_HOME        0x02

// Move the cursor position one step to the right
#define SHIFT_CUR_RIGHT     0x06

// Move the cursor position one step to the left
#define SHIFT_CUR_LEFT      0x07

// Turn OFF the LCD display
#define DSP_OFF             0x08

// Turn ON the display and turn OFF the cursor
#define DSP_ON_CUR_OFF      0x0C

// Turn ON the display and make the cursor visible
#define DSP_ON_CUR_ON       0x0E

// Turn ON the display with a blinking cursor
#define DSP_ON_CUR_BLK      0x0F

// Shift the entire display to the left
#define SHIFT_DSP_LEFT      0x10

// Shift the entire display to the right
#define SHIFT_DSP_RIGHT     0x14

// Configure LCD for 8-bit data mode and single-line display
#define MODE_8BIT_1LINE     0x30

// Configure LCD for 4-bit data mode and single-line display
#define MODE_4BIT_1LINE     0x20

// Configure LCD for 8-bit data mode and two-line display
#define MODE_8BIT_2LINE     0x38

// Configure LCD for 4-bit data mode and two-line display
#define MODE_4BIT_2LINE     0x28

// Set cursor to the first column of LCD line 1
#define GOTO_LINE1_POS0     0x80

// Set cursor to the first column of LCD line 2
#define GOTO_LINE2_POS0     0xC0

// Set cursor to the first column of LCD line 3
#define GOTO_LINE3_POS0     0x94

// Set cursor to the first column of LCD line 4
#define GOTO_LINE4_POS0     0xD4

// Set the starting address of Character Generator RAM (CGRAM)
#define GOTO_CGRAM_START    0x40


/*================ LCD GPIO PIN DEFINITIONS ================*/

// LCD data bus and control signal connections to LPC2148

#define LCD_DATA 8          // P0.8 to P0.15: 8-bit LCD data bus (D0 to D7)

#define LCD_RS 16            // P0.16: Register Select (0 = Command, 1 = Data)

#define LCD_RW 17            // P0.17: Read/Write control (0 = Write, 1 = Read)

#define LCD_EN 18            // P0.18: Enable signal used to latch LCD data or commands
