# PES Project4  README  
Author: Nitish Vazalwar 

**Repository Comments**  
_Contains:_  
* I2CDemo.c - The main script that iterates through the test script and calls the correct command with agruments from test script. Leds turn red on test failure and green on test success. Option to run through entire test script or step through it.
* led.c - File containing an led initialization function, and functions to turn on red, green, and blue leds. 
* led.h - Header file for led_control.c defining the led functions.
* logger.c - File containing logging functions such as enable/disable logger, printing memory address contents, strings, or integers. 
* logger.h - Header file for logger.c defining the logger functions.
* i2c.c - File containing I2C functions
* i2c.h - Header file of the I2C functions
* mma8541.c - Source file for all the internal accelerometer functions
* mma8541.h - Header file for the internal accelerometer of the FRDM KL25z
* selftest.c - Source file for running the initial  test on the accelerometer
* selftest.h - Header file for the test
* statemachine1.c - Source file of the first state machine (state based)
* statemachine.h - Header file for the state based state machine
* statemachine2.c - Source file for secod state machine (table driven)
* statemachine.h - Header file for the table driven state machine
* timer.c - SysTick timer source file
* timer.h - Header file for the timer functions
* touchSen.c - Source file for the Capacitive Touch Slider of KL25z
* touchSen.h - Header file for the Slider functions
* uCUnit-v1.0.h - Header file for uCUnit test suite  

**Project Comments**  
This project was pretty straightforward. I started with implementing I2C protocol for the LIS331 external accelerometer, even captured the scope images for read and write commands with the acknowlegement back from the accelerometer. However, it proved troublesome with the LIS to get the correct data, hence I shifted to the internal accelerometer. I found implementing State Machine 2 (table driven) a bit challenging. Matiching the timing of the Slider with the Systick Timer proved difficult.

**Installation/Execution Notes**  
Developed using MCUExpresso on Windows.  
Two  configurations:
* DEFINE ENABLE_LOGGER - Print debug messages using logger to UART terminal.
* UNDEFINE ENABLE_LOGGER - Don't print debug messages using to UART terminal.

