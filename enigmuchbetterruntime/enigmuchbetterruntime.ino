
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "libenig.h"
#include "HT16K33.h"


HT16K33_H disp; 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Light Read test");
  // Run EigenMuchBetter setup function 
  enigenmuchbetter_setup();
  randomSeed(analogRead(0));

  // Initalize Quad Character Alphanumeric Display 
  disp.begin();
  // Turn on Quad Character Alphanumeric Display s
  disp.setVisible(1); 

}

void loop() {
  // Run Test Active Key Subroutine 
  // preformEncryption();
  flood(); 
  disp.setText("GO  "); 
  delay(2000);
    disp.setText("TEAM"); 
  delay(2000);
  

}


/* Sample application loop. 
Notes : 
This is timer based. As such it will only update once 
every second and a keypress will be double counted if 
it is held for over 1 second.
*/
void preformEncryption(){
  T input;  // The currently pressed key
  T output; // What the key was mapped to 
  
  // Update Keyboard Values 
  scanKeys();
  // Get the active key 
  input = getActiveKey();
  if(input == NO_KEY_ACTIVE){
    return; // No key pressed 
  }

  // Run Cipher here 
  output = stupidCipher(input); 

  // Update LED and display 
  // Put input on LEFT LED 
  disp.setChar((char) input+ 65,2);
  // Put output on Right LED 
  disp.setChar((char) output+ 65,3);
  LedWrite(output,HIGH);
  // Also Output Cipher data to Serial 
  Serial.print("input = ");
  Serial.print(input); 
  Serial.print(" output = ");
  Serial.print(output); 
  Serial.println();

  delay(1000); 
  LedWrite(output,LOW);


  
}
/* Example Cipher Function*/
T stupidCipher(T input){
  return (T) random(0, 27); // +1 for space
}


void testKeyboardLED(){
 for (int i=0; i < 27; i++){
           io_addr_t addr = intToLEDPin((unsigned int)i);
          Serial.print("LETTER : ");
          char text = (char)i+65;
          Serial.print(text);
          Serial.print(" PIN : ");
          Serial.print(addr.pin);
          Serial.print(" EXPANDER : ");
          Serial.println(addr.io_expander_addr);
           LedWrite(i,HIGH);
           delay(200);
           LedWrite(i,LOW);
           delay(200);
         }
}

void testButtons(){
   char a[5]  = "000_";

   for (unsigned int i=0; i < 27; i++){
     a[3] = (char) i + 65;
     disp.setText(a);
    
   io_addr_t addr = intToLEDPin((unsigned int)i);
   uint8_t button = getKey((T) i);
    LedWrite((T) i, button);
     delay(1000);
     LedWrite((T)i,LOW);
     scanKeys();


   }

}

void testActiveKey(){
          scanKeys(); 
       T hi = getActiveKey();
     for (unsigned int i=0; i < 27; i++){
      if(T(i) != hi)
            LedWrite((T)i,LOW);
     }

       char text = (char)hi+65; 
       disp.setChar(text,3); 
       LedWrite(hi,HIGH);
}

void testButtonGPIO(){
  const io_addr_t bank2Addr = {0,IOEXP2_ADDR}; 
  scanKeys();
    uint16_t baz  = exposeBank();

  uint16_t foo  = exposeBank2();
  //GPIOExpanderReadBank(bank2Addr);
  
  for(int i=0; i < 16; i++){
    uint8_t bit = (baz >>i)&0x1; 
    Serial.print(bit);
  }

   for(int i=0; i < 16; i++){
    uint8_t bit = (foo >>i)&0x1; 
    Serial.print(bit);
  }
         Serial.println();

}

void flood(){
for(int i=0; i < 27; i++){

  LedWrite((T)i,HIGH); 

}}
