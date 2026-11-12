
/*******************************************
File: ExtInterruptSwitches.h
Controller: LPC2148 (ARM7)

Purpose:
Declare external interrupt service routines
for emergency handling and RTC configuration,
along with functions for editing RTC values
and laboratory operating time boundaries.
********************************************/


/*================ EXTERNAL INTERRUPT SERVICE ROUTINES ================*/

// FIQ service routine for EINT0, used for emergency handling
void eint0_fiq_isr(void) __irq;

// Non-vectored IRQ service routine for EINT3, used for configuration operations
void eint3_nvirq_isr(void) __irq;


/*================ RTC AND OPERATING TIME CONFIGURATION ================*/

// Allow the administrator to edit RTC time and date information
void editRTC(void);

// Allow the administrator to modify laboratory start and end time boundaries
void editTimeBoundaries(void);
void emergencyAlert(void);
void rtc_interrupt(void);
