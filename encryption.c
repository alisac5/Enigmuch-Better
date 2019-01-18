#include <string.h>
//typedef uint8_t io_addr_t; 
#ifndef __TYPEDEF_T__
#define __TYPEDEF_T__
typedef unsigned char T; // ranges from 0 to 26 inclusive 
#endif

typedef int bool;
#define false 0
#define true 1

// the possible modes the board can be in
enum MODE
{
    ENCRYPT,
    DECRYPT
}

// a collection of three rotors and their respective rotations
// each rotor is specified as an integer index into the array
// of permutations, and the rotations are specified as which
// character appears "on top" at the current moment
typedef struct Rotors
{
    int r1;
    T rot1;
    int r2;
    T rot2;
    int r3;
    T rot3;
} Rotors;
Rotors ROTORS;

// permutations from permutations.txt
int ps[6][27] = {
    {5, 4, 11, 2, 10, 6, 7, 17, 22, 9, 23, 26, 13, 8, 25, 20, 0, 14, 16, 21, 1, 12, 19, 24, 18, 15, 3}, 
    {13, 5, 4, 17, 10, 11, 0, 23, 22, 20, 2, 3, 25, 15, 16, 12, 19, 8, 1, 9, 18, 24, 26, 7, 6, 21, 14},
    {13, 11, 15, 0, 10, 6, 25, 16, 23, 2, 1, 8, 26, 24, 7, 18, 9, 4, 12, 19, 5, 14, 22, 3, 20, 17, 21}, 
    {12, 0, 15, 25, 24, 10, 9, 17, 23, 20, 14, 1, 18, 8, 7, 4, 11, 13, 19, 6, 21, 22, 5, 16, 26, 2, 3}, 
    {23, 16, 13, 2, 1, 7, 12, 5, 3, 21, 19, 22, 0, 11, 9, 15, 6, 14, 17, 24, 10, 26, 8, 20, 18, 25, 4}, 
    {20, 6, 22, 26, 9, 0, 16, 10, 7, 24, 15, 25, 13, 12, 8, 23, 5, 21, 19, 18, 14, 17, 1, 2, 4, 11, 3}, 
    {22, 19, 23, 10, 17, 9, 20, 7, 16, 2, 6, 5, 24, 12, 0, 15, 25, 13, 1, 26, 18, 14, 11, 4, 21, 3, 8}, 
    {4, 24, 25, 21, 14, 12, 13, 8, 26, 2, 9, 16, 3, 22, 20, 23, 15, 19, 17, 6, 11, 1, 0, 5, 7, 18, 10}, 
    {20, 0, 22, 14, 13, 3, 16, 21, 5, 15, 25, 2, 18, 26, 12, 4, 1, 19, 10, 6, 24, 17, 23, 8, 11, 9, 7}, 
    {15, 20, 25, 4, 13, 16, 21, 9, 19, 0, 10, 17, 2, 11, 5, 18, 6, 14, 12, 1, 23, 22, 26, 8, 3, 24, 7}
    }

// the inverse permutations
int invs[6][27] = {
    {16, 20, 3, 26, 1, 0, 5, 6, 13, 9, 4, 2, 21, 12, 17, 25, 18, 7, 24, 22, 15, 19, 8, 10, 23, 14, 11}, 
    {6, 18, 10, 11, 2, 1, 24, 23, 17, 19, 4, 5, 15, 0, 26, 13, 14, 3, 20, 16, 9, 25, 8, 7, 21, 12, 22},
    {3, 10, 9, 23, 17, 20, 5, 14, 11, 16, 4, 1, 18, 0, 21, 2, 7, 25, 15, 19, 24, 26, 22, 8, 13, 6, 12}, 
    {1, 11, 25, 26, 15, 22, 19, 14, 13, 6, 5, 16, 0, 17, 10, 2, 23, 7, 12, 18, 9, 20, 21, 8, 4, 3, 24},
    {12, 4, 3, 8, 26, 7, 16, 5, 22, 14, 20, 13, 6, 2, 17, 15, 1, 18, 24, 10, 23, 9, 11, 0, 19, 25, 21}, 
    {5, 22, 23, 26, 24, 16, 1, 8, 14, 4, 7, 25, 13, 12, 20, 10, 6, 21, 19, 18, 0, 17, 2, 15, 9, 11, 3}, 
    {14, 18, 9, 25, 23, 11, 10, 7, 26, 5, 3, 22, 13, 17, 21, 15, 8, 4, 20, 1, 6, 24, 0, 2, 12, 16, 19}, 
    {22, 21, 9, 12, 0, 23, 19, 24, 7, 10, 26, 20, 5, 6, 4, 16, 11, 18, 25, 17, 14, 3, 13, 15, 1, 2, 8}, 
    {1, 16, 11, 5, 15, 8, 19, 26, 23, 25, 18, 24, 14, 4, 3, 9, 6, 21, 12, 17, 0, 7, 2, 22, 20, 10, 13}, 
    {9, 19, 12, 24, 3, 14, 16, 26, 23, 7, 10, 13, 18, 4, 17, 0, 5, 11, 15, 8, 1, 6, 21, 20, 25, 2, 22}
    } 

// Return a "one level deep" encryption of a given character. Note that this
// function should be used multiple times if multiple rotors are being used
// in the encryption scheme.
T phase(int rotor_num, T input, T rotation)
{
    return ps[rotor_num][input + rotation] - rotation;
}

// The inverse of the above function.
T inv_phase(int rotor_num, T output, T rotation)
{
    // ps[input + rotation] - rotation = output
    // ps[input + rotation] = output + rotation
    // input + rotation = inv[output + rotation]
    // input = inv[output + rotation] - rotation
    return invs[rotor_num][output + rotation] - rotation;
}

// Change the rotations of the rotors being used.
void nextState(Rotors *r)
{
    r->rot1++;
    r->rot2++;
    r->rot3++;
}

// Encrypt a given character using the current state of the three rotors.
T encrypt(T input)
//@requires input <= 26;
//@ensures \result <= 26;
{
    Rotors r = ROTORS;
    T first_round = phase(r.r1, input, r.rot1);
    T second_round = phase(r.r2, first_round, r.rot2);
    T third_round = phase(r.r3, second_round, r.rot3);
    return third_round;
}

// Decrypt a given character using the current state of the three rotors.
T decrypt(T output)
{
    Rotors r = ROTORS;
    T third_inv = inv_phase(r.r3, output, r.rot3);
    T second_inv = inv_phase(r.r2, output, r.rot2);
    T first_inv = inv_phase(r.r1, output, r.rot1);
    return first_inv;
}

// Helper function
T char2T(char c)
{
  if(c == 32) //Space --> 26
    return 26;
  else if(65 <= c && c <= 90) //Uppercase letters mapped
    return c - 65;
  else if(97 <= c && c <= 122) //Lowercase letters mapped
    return c - 97;
  else abort();
}

// Demo driver function
int main()
{
  int i;
  char plaintext[] = "THIS IS A TEST"; // strlen(plaintext) = 14
  char ciphertext[14];
  char decipheredtext[14];
  
  /*
  // Initialization
  K key = nextState(NULL);
  K originalKey = key;
  
  // Encryption
  for(i = 0; i < 14; i++)
  {
    ciphertext[i] = encrypt(plaintext[i]);
    key = nextState(key);
  }
  
  // Decryption
  key = originalKey;
  for(i = 0; i < 14; i++)
  {
    decipheredtext[i] = encrypt(key, ciphertext[i]);
    key = nextState(key);
  }
   
  // Check
  if(!strcmp(plaintext, decipheredtext))
    abort();
  else
    return 0;
  */
}
