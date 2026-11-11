
/*******************************************
File: defines.h
Controller: LPC2148 (ARM7)

Purpose:
Provide reusable macros for bitwise operations
on hardware registers, including reading,
setting, clearing and writing bits, nibbles
and bytes.
********************************************/

/*================ BYTE OPERATIONS ================*/

// Write an 8-bit value into the specified byte position of a register
#define WRITEBYTE(WORD,STARTBIT,BYTE) \
                WORD=((WORD&~(0XFF<<STARTBIT)) | (BYTE<<STARTBIT))


/*================ BIT OPERATIONS ================*/

// Read the value of a single bit from a register (returns 0 or 1)
#define READBIT(WORD,BIT) ((WORD>>BIT)&1)

// Set the specified bit to 1 without changing other bits
#define SETBIT(WORD,BIT) WORD |= 1<<BIT

// Clear the specified bit to 0 without changing other bits
#define CLRBIT(WORD,BIT) WORD &= (~(1<<BIT))


/*================ NIBBLE OPERATIONS ================*/

// Write a 4-bit value into the specified nibble position of a register
#define WRITENIBBLE(WORD,STARTBIT,BYTE) \
                WORD=((WORD&~(0XF<<STARTBIT)) | (BYTE<<STARTBIT))

// Read a 4-bit nibble from the specified starting bit position
#define READNIBBLE(WORD,BIT) ((WORD>>BIT)&0xF)


/*================ PORT PIN CONFIGURATION ================*/

// Configure the alternate function of a port pin (currently commented out)
//#define CFGPORTPIN(WORD,PIN,FUNC) \
                (WORD=((WORD&~(0X03<<((PIN%16)*2))) |(FUNC<<((PIN%16)*2))))


/*================ REUSABILITY ================*/

// These macros can be reused across different peripheral register operations
