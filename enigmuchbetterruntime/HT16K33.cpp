#include <Wire.h>
#include "HT16K33.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

const uint8_t addr = 0x70; // HT16K33 default address

static const uint8_t numbertable[] = {
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F, /* 9 */
    0x77, /* a */
    0x7C, /* b */
    0x39, /* C */
    0x5E, /* d */
    0x79, /* E */
    0x71  /* F */
};

// Copied from https://github.com/adafruit/Adafruit_LED_Backpack/blob/master/Adafruit_LEDBackpack.cpp

static const uint16_t alphafonttable[] = {

    0b0000000000000001,
    0b0000000000000010,
    0b0000000000000100,
    0b0000000000001000,
    0b0000000000010000,
    0b0000000000100000,
    0b0000000001000000,
    0b0000000010000000,
    0b0000000100000000,
    0b0000001000000000,
    0b0000010000000000,
    0b0000100000000000,
    0b0001000000000000,
    0b0010000000000000,
    0b0100000000000000,
    0b1000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0000000000000000,
    0b0001001011001001,
    0b0001010111000000,
    0b0001001011111001,
    0b0000000011100011,
    0b0000010100110000,
    0b0001001011001000,
    0b0011101000000000,
    0b0001011100000000,
    0b0000000000000000, //
    0b0000000000000110, // !
    0b0000001000100000, // "
    0b0001001011001110, // #
    0b0001001011101101, // $
    0b0000110000100100, // %
    0b0010001101011101, // &
    0b0000010000000000, // '
    0b0010010000000000, // (
    0b0000100100000000, // )
    0b0011111111000000, // *
    0b0001001011000000, // +
    0b0000100000000000, // ,
    0b0000000011000000, // -
    0b0000000000000000, // .
    0b0000110000000000, // /
    0b0000110000111111, // 0
    0b0000000000000110, // 1
    0b0000000011011011, // 2
    0b0000000010001111, // 3
    0b0000000011100110, // 4
    0b0010000001101001, // 5
    0b0000000011111101, // 6
    0b0000000000000111, // 7
    0b0000000011111111, // 8
    0b0000000011101111, // 9
    0b0001001000000000, // :
    0b0000101000000000, // ;
    0b0010010000000000, // <
    0b0000000011001000, // =
    0b0000100100000000, // >
    0b0001000010000011, // ?
    0b0000001010111011, // @
    0b0000000011110111, // A
    0b0001001010001111, // B
    0b0000000000111001, // C
    0b0001001000001111, // D
    0b0000000011111001, // E
    0b0000000001110001, // F
    0b0000000010111101, // G
    0b0000000011110110, // H
    0b0001001000000000, // I
    0b0000000000011110, // J
    0b0010010001110000, // K
    0b0000000000111000, // L
    0b0000010100110110, // M
    0b0010000100110110, // N
    0b0000000000111111, // O
    0b0000000011110011, // P
    0b0010000000111111, // Q
    0b0010000011110011, // R
    0b0000000011101101, // S
    0b0001001000000001, // T
    0b0000000000111110, // U
    0b0000110000110000, // V
    0b0010100000110110, // W
    0b0010110100000000, // X
    0b0001010100000000, // Y
    0b0000110000001001, // Z
    0b0000000000111001, // [
    0b0010000100000000, //
    0b0000000000001111, // ]
    0b0000110000000011, // ^
    0b0000000000001000, // _
    0b0000000100000000, // `
    0b0001000001011000, // a
    0b0010000001111000, // b
    0b0000000011011000, // c
    0b0000100010001110, // d
    0b0000100001011000, // e
    0b0000000001110001, // f
    0b0000010010001110, // g
    0b0001000001110000, // h
    0b0001000000000000, // i
    0b0000000000001110, // j
    0b0011011000000000, // k
    0b0000000000110000, // l
    0b0001000011010100, // m
    0b0001000001010000, // n
    0b0000000011011100, // o
    0b0000000101110000, // p
    0b0000010010000110, // q
    0b0000000001010000, // r
    0b0010000010001000, // s
    0b0000000001111000, // t
    0b0000000000011100, // u
    0b0010000000000100, // v
    0b0010100000010100, // w
    0b0010100011000000, // x
    0b0010000000001100, // y
    0b0000100001001000, // z
    0b0000100101001001, // {
    0b0001001000000000, // |
    0b0010010010001001, // }
    0b0000010100100000, // ~
    0b0011111111111111

};

void HT16K33_H::begin()
{
    // start i2c
    Wire.begin();
    Wire.beginTransmission(addr);
    Wire.write(0x20 | 1); // turn on oscillator
    Wire.endTransmission();
}

void HT16K33_H::update()
{
    Wire.beginTransmission(addr);
    Wire.write(0x00); // start at address 0x0

    for (int i = 0; i < 8; i++)
    {
        Wire.write(displayBuffer[i] & 0xFF);
        Wire.write(displayBuffer[i] >> 8);
    }
    Wire.endTransmission();
}

void HT16K33_H::setVisible(uint8_t visibility)
{
    uint8_t visisble;
    visisble = (visibility == 0) ? 0 : 1;
    Wire.beginTransmission(addr);
    Wire.write(0x80 | visisble);
    Wire.endTransmission();
}
void HT16K33_H::setText(const char * values)
{
    setASCIIChar(values[0], 0);
    setASCIIChar(values[1], 1);
    setASCIIChar(values[2], 2);
    setASCIIChar(values[3], 3);
    update();
}

    void HT16K33_H::setChar(const char value, uint8_t digit){
        setASCIIChar(value,digit);
        update();
    }

void HT16K33_H::setASCIIChar(const char value, uint8_t digit)
{
    // Check for in bound if not display error glyphs
    if (digit > 3)
    {
        displayBuffer[0] = alphafonttable[127];
        displayBuffer[1] = alphafonttable[127];
        displayBuffer[2] = alphafonttable[127];
        displayBuffer[3] = alphafonttable[127];

        return;
    }
    // check for valid ascii, if not display error glyphs
    if (value > 127)
    {
        displayBuffer[digit] = alphafonttable[127];
        return;
    }
    displayBuffer[digit] = alphafonttable[value];
}

void HT16K33_H::loop()
{
    const int dTime = 50;

    // Turn on all segments, one at a time
    for (int i = 0; i < 8; i++)
    {
        for (int k = 0; k < 16; k++)
        {
           this->setText("HELL");
            update();
            delay(dTime);
        }
    }
}



