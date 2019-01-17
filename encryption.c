#include <string.h>
//typedef uint8_t io_addr_t; 
#ifndef __TYPEDEF_T__
#define __TYPEDEF_T__
typedef unsigned char T; // ranges from 0 to 26 inclusive 
#endif

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

int ps[6][27] = {
    { 2, 20,  0, 12, 14,  8,  3, 21, 22, 23,  6,  4, 10, 24,  7, 18, 16,
       17,  9,  5, 11, 25, 13, 15,  1, 19},
    {16, 14,  8,  5, 12, 20,  2, 15,  9, 25,  4, 19,  0, 21, 18, 17,  6,
       22,  7,  3, 24, 13, 11, 10,  1, 23},
    {17,  4, 24, 21,  8,  0,  2, 10, 22,  9, 12, 13, 16, 18,  7,  5,  6,
       19, 15,  1, 11,  3, 23, 14, 25, 20},
    {17, 22, 15, 20,  3, 19, 23, 21, 24,  0, 25, 12, 13,  2, 18, 14,  4,
        1,  5,  7,  8,  6, 16,  9, 11, 10},
    {12, 10,  5, 22, 21, 17, 19,  6, 20,  8, 13, 23, 15, 24, 18, 14,  3,
        1,  2,  0,  7,  9, 16,  4, 11, 25},
    { 8,  1, 15,  2, 10, 21, 11, 22, 24,  0,  3, 25, 13,  7, 20,  9, 19,
       16, 18,  5, 14,  4, 17, 12, 23,  6}
};

T phase(int rotor_num, T input, T rotation)
{
    return ps[rotor_num][input + rotation] - rotation;
}

void nextState(Rotors *r)
{
    r->rot1++;
    r->rot2++;
    r->rot3++;
}

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
