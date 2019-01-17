/**********************************************
Constants for the Eigenmuch Better
**********************************************/
#include <stdint.h>

#ifndef _ENIGMUCHBETTER_CONSTANTS_H_
#define _ENIGMUCHBETTER_CONSTANTS_H_

/* IO Expander addres definitons. Matches board schematic names*/
#define IOEXP0_ADDR 0x0
#define IOEXP1_ADDR 0x4
#define IOEXP2_ADDR 0x2
#define IOEXP3_ADDR 0x6

#define NO_KEY_ACTIVE  29

//typedef uint8_t io_addr_t; 
#ifndef __TYPEDEF_T__
#define __TYPEDEF_T__
typedef unsigned char T; // ranges from 0 to 26 inclusive typedef unsigned int K; // ranges from 0 to 2^20 - 1 
#endif


/** io_addr_t is a composite value 
 * bits [0-3] Encode which Expander pin the key is on
 * bits [4-8] Encode which GPIO Expander the key is on
 * */
typedef struct io_addr{
    unsigned int pin : 4; 
    unsigned int io_expander_addr :  4; 
} io_addr_t; 






/* Sets up the IO Expanders. Call in main setup(); function*/
void enigenmuchbetter_setup();


/*******************
 * IO FUNCTIONS
 ********************/

/*  
  LedWrite takes in a T (valid from 0 to 26)
  and writes out the value (HIGH or LOW) to the 
  LED.

  This will mark the changes and the LED will be updated at a later time. 
  As such this method is preferred over LedWrite
  NOTE : NOT IMPLEMENTED YET
  */
void writeKeyboardLed(T pin, uint8_t d);
/* 
  LedWrite takes in a T (valid from 0 to 26)
  and writes out the value (HIGH or LOW) to the 
  LED.

  This will immediately update the LED 
*/
void LedWrite(unsigned int input,uint8_t d);

/* Take a guess*/
void LedAllOn();
void LedAllOff();

/* Reads a keyboard button and returns it's state
  LOL DON'T USE 
s*/
uint8_t readKeyboardButton(T pin);


/*
Gets the status of each key on the keyboard and stores it in a 
cache. This should be called by either a loop or an ISR 
*/
void scanKeys();

/* 
  Gets the currently presed key on the keyboard. If multiple keys are presssed, 
  the key with the lowest numerical value (A=0,Z=26 _ = 27) will be returned.
  Key data is buffered from last time scanKeys was called
*/
T getActiveKey(); 

/* Get the status of a Key. Key data is buffered from last time scanKeys was called*/
uint8_t getKey(T pin);


/*******************
 * UTILITY FUNCTIONS
 ********************/



/* Converts a type T to a physical LED pin.
@requires 0 to 26*/
io_addr_t TtoLEDPin(T pin);

/* Converts an int to a order of keys on querty order*/
T intToQuertyOrder(unsigned int pin);

/* Converts a type T to a physiscal button pin*/
io_addr_t TtoButtonPin(T pin);

/* Converts a type int to a physical LED pin.*/
io_addr_t intToLEDPin(unsigned int input);

/* Converts a type int to a physiscal button pin*/
io_addr_t intToButtonPin(unsigned int input);

/* Write data to a pin on the GPIO Expanders.
  @requires ioReg_x.pin 0 <= X < 16
  @requires ioReg_x.io_expander_addr 0 <=X<4
*/
void GPIOExpanderWrite(io_addr_t ioReg_x,uint8_t d);
/* Read data to the GPIO Expanders.*/

/* Read data from a pin on the GPIO Expanders.
  @requires ioReg_x.pin 0 <= X < 16
  @requires ioReg_x.io_expander_addr 0 <=X<4
*/
uint8_t  GPIOExpanderRead(io_addr_t ioReg_x);

/* Read data from an entire GPIO Expander at once 
  @requires ioReg_x.pin 0 <= X < 16
  @requires ioReg_x.io_expander_addr 0 <=X<4
*/
uint16_t GPIOExpanderReadBank(io_addr_t ioReg_x);

/* Ignore. For testing. Will be removed*/
uint16_t exposeBank();
uint16_t exposeBank2();

#endif 
