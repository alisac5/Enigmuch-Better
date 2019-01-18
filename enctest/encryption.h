#ifndef _H_ENCRYPT_
#define _H_ENCRYPT_

#include <string.h>
#include <stdio.h>

//typedef uint8_t io_addr_t; 
#ifndef __TYPEDEF_T__
#define __TYPEDEF_T__
typedef unsigned char T; // ranges from 0 to 26 inclusive 
#endif

/*
#ifndef ARDUINO
typedef int bool;


#define false 0
#define true 1
#endif
*/

// the possible modes the board can be in
enum MODE
{
    ENCRYPT,
    DECRYPT
};

/* A collection of three rotors and their respective rotations
   each rotor is specified as an integer index into the array
   of permutations, and the rotations are specified as which
   character appears "on top" at the current moment. Also includes
   a count of how many characters have been passed through, which
   is used to determine how to appropriately change the state of
   the rotors.
*/
typedef struct rotors
{
    int r1;
    T rot1;
    int r2;
    T rot2;
    int r3;
    T rot3;
    int count;
} Rotors;

//Rotors ROTORS;

// Set the initial state given the rotors and their rotations.
void setInitialState(int r1, T rot1, int r2, T rot2, int r3, T rot3);

// Change the rotations of the rotors being used.
void nextState();

// Encrypt a given character using the current state of the three rotors.
//@requires input <= 26;
//@ensures \result <= 26;
T encrypt(T input);

// Decrypt a given character using the current state of the three rotors.
T decrypt(T output);

//Converts a T type to a char type
char T2Char(T t);
#endif
