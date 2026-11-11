# Test Cases and Validation

## 1. Purpose

This document provides a validation checklist for SmartLab Guardian.
Expected behaviour is based on the project description and functions
represented in the reviewed source files.

**Important:** These are proposed test cases. Their initial status is
**Not Run**. Update a result only after performing the test and
recording evidence.

## 2. Test environment

  Item                      Test setup
  ------------------------- -----------------------------------------
  Microcontroller board     LPC2148 --- record exact board/revision
  Development environment   Keil µVision --- record version
  Compiler                  Record ARMCC version
  Programming tool          Flash Magic --- record version, if used
  Supply voltage            Record measured value
  Oscillator frequency      Verify and record
  Test type                 Simulation / Physical hardware
  Test date                 To be recorded

## 3. Functional test cases

  --------------------------------------------------------------------------------
  Test ID        Function         Procedure      Expected result    Status
  -------------- ---------------- -------------- ------------------ --------------
  TC-01          System           Reset or power Initialization     Not Run
                 initialization   on the         completes and      
                                  controller     startup            
                                                 information is     
                                                 displayed          

  TC-02          LCD interface    Run the        Commands and       Not Run
                                  application    characters appear  
                                  and observe    in intended        
                                  the LCD        positions          

  TC-03          Keypad scanning  Press each     Corresponding key  Not Run
                                  keypad key     value is detected  
                                  individually                      

  TC-04          Correct password Enter the      Password module    Not Run
                                  configured     accepts the valid  
                                  valid PIN      PIN                

  TC-05          Incorrect        Enter an       Failure/retry      Not Run
                 password         invalid PIN    handling follows   
                                                 the implemented    
                                                 logic              

  TC-06          PIN change       Execute the    Confirmation and   Not Run
                                  PIN-change     validation follow  
                                  flow           the implementation 

  TC-07          RTC time display Read and       Displayed time     Not Run
                                  display RTC    corresponds to RTC 
                                  time           values             

  TC-08          RTC date/day     Read and       Displayed date/day Not Run
                 display          display RTC    corresponds to RTC 
                                  date/day       values             

  TC-09          Operating-hour   Test before,   `IsWithinTime()`   Not Run
                 check            within, and    returns the result 
                                  after          defined by its     
                                  configured     boundary           
                                  hours          comparisons        

  TC-10          IR reception     Press          Valid RC5 frames   Not Run
                                  supported      are decoded into   
                                  buttons on the address, command,  
                                  intended       and toggle values  
                                  remote                            

  TC-11          LED indication   Exercise       LED outputs change Not Run
                                  application    as defined by the  
                                  paths          application        
                                  controlling                       
                                  each LED                          

  TC-12          Emergency        Activate the   Configured         Not Run
                 interrupt        emergency      emergency ISR      
                                  switch         executes its       
                                                 programmed         
                                                 response           

  TC-13          RTC-edit         Activate the   Configured         Not Run
                 interrupt        RTC-edit       RTC-edit routine   
                                  switch         is entered         

  TC-14          RTC editing      Enter new RTC  RTC values are     Not Run
                                  values         updated according  
                                                 to the implemented 
                                                 input flow         

  TC-15          Buzzer           Exercise the   Buzzer output      Not Run
                 indication       emergency path follows the        
                                                 programmed         
                                                 emergency          
                                                 indication         
  --------------------------------------------------------------------------------

## 4. Operating-hour boundary tests

The reviewed `IsWithinTime()` implementation compares hour values. Test
the configured start and end hours, including boundaries.

  Test ID              Current hour Expected check
  --------- ----------------------- -------------------------------------------
  OH-01       One hour before start Outside operating period
  OH-02                  Start hour Verify exact comparison behaviour in code
  OH-03       Between start and end Inside operating period
  OH-04                    End hour Verify exact comparison behaviour in code
  OH-05          One hour after end Outside operating period

If an overnight schedule is intended, test it separately and record the
actual return values.

## 5. Test result record

For each test, record: - Test ID and date. - Test environment
(simulation or hardware). - Input/action performed. - Expected result. -
Actual observed result. - Pass / Fail / Blocked. - Screenshot or video
filename. - Notes or defect reference.

  Field             Value
  ----------------- ------------------------------------
  Test ID           Example: TC-01
  Date              YYYY-MM-DD
  Environment       Simulation / Hardware
  Input             Power-on or reset
  Expected result   Initialization and startup display
  Actual result     Fill after testing
  Status            Not Run
  Evidence          Add screenshot/video filename

## 6. Evidence storage

Store genuine test evidence in:

``` text
Testing/
├── Test_Cases.md
├── Output_Screenshots/
└── Output_Videos/
```

Suggested filenames: - `TC01_System_Initialization.png` -
`TC04_Valid_Password.png` - `TC10_IR_Receiver_Test.png` -
`TC12_Emergency_Interrupt.mp4`

Only include screenshots and recordings from your own simulation or
hardware tests.

## 7. Verification points

1.  **IR polling:** The main application contains a blocking wait for a
    valid IR frame. Check whether keypad and other main-loop operations
    remain responsive during IR reception.
2.  **Keypad validation:** Test short and incomplete entries, especially
    password and time-entry flows.
3.  **Operating-hour boundaries:** The current check compares hours.
    Verify start/end boundary behaviour and any overnight schedule.
4.  **Interrupt pin mapping:** Confirm configured EINT pins match the
    LPC2148 connections and circuit diagram.
5.  **RTC configuration:** Confirm target device, oscillator, clock
    configuration, and RTC settings match the hardware.
6.  **Emergency response:** Verify every output expected to be disabled
    or activated during an emergency.

These are verification points, not claims that a test has failed.

## 8. Overall test summary

Update after executing the test cases.

  Metric                       Result
  ------------------ ----------------
  Total test cases                 15
  Passed               To be recorded
  Failed               To be recorded
  Blocked              To be recorded
  Not run                15 initially
