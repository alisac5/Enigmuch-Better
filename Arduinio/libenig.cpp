/**********************************************
Constants for the Eigenmuch Better
**********************************************/
#include <stdint.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "libenig.h"

Adafruit_MCP23017 IOEXP0;
Adafruit_MCP23017 IOEXP1;
Adafruit_MCP23017 IOEXP2;
Adafruit_MCP23017 IOEXP3;

uint16_t lastKeyScanA; // State of buttons A-?
uint16_t lastKeyScanB; // State of buttons from ?+1-Z

void enigenmuchbetter_setup()
{
    // SETUP IO Expanders
    IOEXP0.begin(IOEXP0_ADDR); // IO Exapnder 0
    IOEXP1.begin(IOEXP1_ADDR); // IO Exapnder 0
    IOEXP2.begin(IOEXP2_ADDR); // IO Exapnder 0
    IOEXP3.begin(IOEXP3_ADDR); // IO Exapnder 0

    // Setup Output s
    for (int i = 0; i < 16; i++)
    {
        IOEXP0.pinMode(i, OUTPUT);
        IOEXP1.pinMode(i, OUTPUT);
        IOEXP2.pinMode(i, INPUT);
        IOEXP3.pinMode(i, INPUT);
 

        IOEXP0.pullUp(i, LOW);
        IOEXP1.pullUp(i, LOW);
        IOEXP2.pullUp(i, HIGH);
        IOEXP3.pullUp(i, HIGH);
        
        IOEXP0.digitalWrite(i,LOW);
        IOEXP1.digitalWrite(i,LOW);
    }
    // Setup IOExapnder 0 pinmode
    //   IOEXP0.pinMode(0,OUTPUT);
}

void keyboard_write(char key, int value)
{
    /* 
    Convert key to pin number and GPIO Bank
    */
}

void LedWrite(unsigned int input, uint8_t d)
{
    io_addr_t addr = intToLEDPin(input);
    GPIOExpanderWrite(addr, d);
}

void scanKeys()
{
    // Scans all the keyboard keys and stores the results
    uint16_t IOEXP2_data;
    uint16_t IOEXP3_data;
    const io_addr_t bank2Addr = {0, IOEXP2_ADDR};
    const io_addr_t bank3Addr = {0, IOEXP3_ADDR};

    IOEXP2_data = GPIOExpanderReadBank(bank2Addr);
    IOEXP3_data = GPIOExpanderReadBank(bank3Addr);
    // Now clean up the raw data. We want a bitvector 27 bits long
    // that shows keypres state for A-Z + space buttons

    IOEXP2_data = ~IOEXP2_data;
    IOEXP3_data = ~IOEXP3_data; // Swap active low to active high

    // First 5 bit sof IOEXPander 2 not connected to buttons. Remove them
    IOEXP2_data = (IOEXP2_data >> 5) & 0b0000011111111111;
    // Now shift down the first 5 bit sfrom IOExpander3 data
    IOEXP2_data = IOEXP2_data | ((IOEXP3_data & 0b0000000000011111) << 10);
    // Now shift down first 5 bit of IOEXpander 3
    IOEXP3_data = (IOEXP3_data >> 5) & 0b0000011111111111;

    // Now data is clean. Assign to buffer
    lastKeyScanA = IOEXP2_data;
    lastKeyScanB = IOEXP3_data;
}

/* Note : 
getActiveKey only reports values that have changed since 
the last time it was called make sure getActiveKey is 
called regularly by some point in the program*/

T getActiveKey()
{

    for (int i = 0; i < 27; i++)
    {
        if (getKey((T)i))
        {
            return (T)i;
        }
    }

    return 29;
}

T getKey(T pin)
{
    // Key gey from last scan
    return (pin < 16) ? (lastKeyScanA >> pin) & 0x1 : (lastKeyScanB >> (pin - 16)) & 0x1;
    //return 0x1&((pin >= 16)? (lastKeyScanA >> pin) : (lastKeyScanB >> (pin-16)));
}

io_addr_t TtoLEDPin(T pin)
{
    return intToLEDPin((unsigned int)pin);
}

io_addr_t TtoButtonPin(T pin)
{
    return intToButtonPin((unsigned int)pin);
}

io_addr_t intToLEDPin(unsigned int input)
{
    io_addr_t io_address;
    //@requires input <= 26;
    //@requires input >= 0;
    if (input < 16)
    {
        // GPIO Expander 1
        io_address.pin = 0x0F & input;
        io_address.io_expander_addr = IOEXP0_ADDR;
        return io_address;
    }

    if (input < 30)
    {
        io_address.pin = 0xF & (input - 16);
        io_address.io_expander_addr = IOEXP1_ADDR;
        return io_address;
    }
    // ERROR. OUT OF BOUNDS
    io_address.pin = 0xF;
    io_address.io_expander_addr = 0xF;
    return io_address;
}

io_addr_t intToButtonPin(unsigned int input)
{
    //@requires input <= 26;
    //@requires input >= 0;
    io_addr_t io_address;

    if (input < 11)
    {
        io_address.pin = input + 5;
        io_address.io_expander_addr = IOEXP2_ADDR;
        return io_address;
    }

    if (input < 27)
    {
        io_address.pin = input - 11;
        io_address.io_expander_addr = IOEXP3_ADDR;
        return io_address;
    }
    // ERROR. OUT OF BOUNDS
    io_address.pin = 0xF;
    io_address.io_expander_addr = 0xF;
    return io_address;
}

void GPIOExpanderWrite(io_addr_t ioReg_x, uint8_t d)
{
    // Check. If Out of bounds ignore
    if (d > 16)
    {
        return; // Preform no operation
    }
    switch (ioReg_x.io_expander_addr)
    {
    case IOEXP0_ADDR:
        IOEXP0.digitalWrite((uint8_t)ioReg_x.pin, d);
        break;
    case IOEXP1_ADDR:
        IOEXP1.digitalWrite((uint8_t)ioReg_x.pin, d);
        break;
    case IOEXP2_ADDR:
        IOEXP2.digitalWrite((uint8_t)ioReg_x.pin, d);
        break;
    case IOEXP3_ADDR:
        IOEXP3.digitalWrite((uint8_t)ioReg_x.pin, d);
        break;
    }
}

uint8_t GPIOExpanderRead(io_addr_t ioReg_x)
{
    switch (ioReg_x.io_expander_addr)
    {
    case IOEXP0_ADDR:
        return IOEXP0.digitalRead((uint8_t)ioReg_x.pin);
        break;
    case IOEXP1_ADDR:
        return IOEXP1.digitalRead((uint8_t)ioReg_x.pin);
        break;
    case IOEXP2_ADDR:
        IOEXP2.digitalRead((uint8_t)ioReg_x.pin);
        break;
    case IOEXP3_ADDR:
        IOEXP3.digitalRead((uint8_t)ioReg_x.pin);
        break;
    }
}

uint16_t GPIOExpanderReadBank(io_addr_t ioReg_x)
{
    switch (ioReg_x.io_expander_addr)
    {
    case IOEXP0_ADDR:
        return IOEXP0.readGPIOAB();
        break;
    case IOEXP1_ADDR:
        return IOEXP1.readGPIOAB();
        break;
    case IOEXP2_ADDR:
        return IOEXP2.readGPIOAB();
        break;
    case IOEXP3_ADDR:
        return IOEXP3.readGPIOAB();

        break;
    }
}

uint16_t exposeBank()
{
    return lastKeyScanA;
}

uint16_t exposeBank2()
{
    return lastKeyScanB;
}
