#include "encryption.h"
#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Light Read test");
 // setInitialState(0,0,0,0,0,0); 

}

void loop() {
  // put your main code here, to run repeatedly:
 
    /*while (Serial.available() > 0) {
    char _byte = Serial.read();
    Serial.println((char)encrypt((T)_byte-65)+65);
    }
    */

}
