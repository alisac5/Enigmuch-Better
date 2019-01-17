#include <string.h>
typedef unsigned char T; // ranges from 0 to 26 inclusive
typedef unsigned int K; // ranges from 0 to 2^20 - 1 inclusive

T encrypt(K key, T input)
//@requires input <= 26;
//@requires key <= 1<<20 - 1;
//@ensures \result <= 26;
{
  return 0;
}

// Helper function
T char2T(char c)
{
  if(c == 32)
    return 26;
  else if(65 <= c && c <= 90)
    return c - 65;
  else if(97 <= c && c <= 122)
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
  
  // Initialization
  K key = nextKey(NULL);
  K originalKey = key;
  
  // Encryption
  for(i = 0; i < 14; i++)
  {
    ciphertext[i] = encrypt(key, plaintext[i]);
    key = nextKey(key);
  }
  
  // Decryption
  key = originalKey;
  for(i = 0; i < 14; i++)
  {
    decipheredtext[i] = encrypt(key, ciphertext[i]);
    key = nextKey(key);
  }
   
  // Check
  if(!strcmp(plaintext, decipheredtext))
    abort();
  else
    return 0;
  }
