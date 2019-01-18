#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include "libenig.h"
#include "Demo.h"
#include "encryption.h"
char cmdBuf[4];

void DemoController::begin(HT16K33_H *d)
{
  disp = d;
  cmdBuf[0] = '\0';
  cmdBuf[1] = '\0';
  cmdBuf[2] = '\0';
}

void DemoController::loop()
{
  static cmd_t cmd;

  cmd_t newCMD = processSerialMessages();
  if (newCMD.c != NONE)
  {
    cmd = newCMD;
  }
  if (cmd.c == SLIDE)
  {
    doSlide(cmd.arg0);
  }

  if (cmd.c == DEMO_ENCRYPT)
  {
    demoInputLoop(0);
  }
  if (cmd.c == DEMO_DECRYPT)
  {
    demoInputLoop(1);
  }
}

void DemoController::doSlide(int slide)
{
  switch (slide)
  {

  case 20:
    disp->setText("    ");
    LedAllOff();
    break;
  case 30:
    disp->setText("HI  ");
    delay(1000);
    LedWrite(0, HIGH);
    break;
  case 100:
    stepDemoLighting();

    break;

  case 99:
    LedWrite(0, LOW);

    break;
  }
}
unsigned int baseTimer_;
unsigned int scanKeyTimer_; //Overflows sonce every 16 seconds

void DemoController::demoInputLoop(int mode)
{
  static T activeKey;

  scanKeyTimer_ = millis();
  if ((scanKeyTimer_ - baseTimer_) > 100)
  {
    baseTimer_ = millis();
    scanKeys();

    if (getActiveKey() != NO_KEY_ACTIVE)
    {
      LedWrite(activeKey, LOW);
      activeKey = demoCipher(mode);
      LedWrite(activeKey, HIGH);
    }
  }
}
/* Similar to preformEncryption()*/
T DemoController::demoCipher(int mode)
{
  T input;  // The currently pressed key
  T output; // What the key was mapped to
      // Get the active key
  input = getActiveKey();
  // Run Cipher here
  output = (mode == 0) ? encrypt(input) : decrypt(input);
  nextState();

  //Output Data to Serial 
  if(mode == 0){ // ENCRYPT
    Serial.print("ENC-");
  }else{
    Serial.print("DEC-");
  }
   char input_c = (char)input+65; 
   char output_c = (char)output+65;
   disp->setChar(input_c,2); 
      disp->setChar(output_c,3); 

   // disp->setText("HELP");

    Serial.print(input_c);
    Serial.print(output_c);
    Serial.println();
  return output;
}

void DemoController::stepDemoLighting()
{
  static int undefined_int; //LOL
  undefined_int++;
  uint8_t i = undefined_int % 27;
  T led = intToQuertyOrder(i);
  LedWrite(led, HIGH);

  disp->setChar((char)led + 65, 0);
  disp->setChar((char)led + 65, 1);
  disp->setChar((char)led + 65, 2);
  disp->setChar((char)led + 65, 3);

  delay(300);
  LedWrite(led, LOW);
  delay(300);
}

cmd_t DemoController::processSerialMessages()
{
  // First Check for any data on serial
  int A, B;
  cmd_t command;
  command.c = NONE;
  while (Serial.available() > 0)
  {
    char _byte = Serial.read();
    cmdBuf[0] = cmdBuf[1];
    cmdBuf[1] = cmdBuf[2];
    cmdBuf[2] = _byte;
    cmdBuf[3] = '\0';

    command.c = getCMD(cmdBuf);

    if (command.c != NONE)
    {

      switch (command.c)
      {
      case SLIDE:
        command.arg0 = Serial.parseInt();
        break;
      case DEMO_DECRYPT:
      setInitialState(0,0,0,0,0,0);
      break;
      case DEMO_ENCRYPT:
      setInitialState(0,0,0,0,0,0);
      break;
      case SET_STATE:
      int A,B,C,D,E,F,G;
        A = Serial.parseInt();
        B = Serial.parseInt();
        C = Serial.parseInt();
        D = Serial.parseInt();
        E = Serial.parseInt();
        G = Serial.parseInt();
        setInitialState(A,B,C,D,E,F);
        break;
      case SET_DEFAULT : 
        setInitialState(0,0,0,0,0,0);
        break;
      case NONE:
        break;
      }

      return command;
    }
  }

  return command;
}

// Fetch a command from the serial interface
commandType DemoController::getCMD(const char data[4])
{
  if (strcmp(data, "SLD") == 0)
  {
    return SLIDE;
  }

  if (strcmp(data, "ENC") == 0)
  {
    return DEMO_ENCRYPT;
  }

  if (strcmp(data, "DEC") == 0)
  {
    return DEMO_DECRYPT;
  }

   if (strcmp(data, "SET") == 0)
  {
    // SET THE INITAL STATE
    
    return SET_STATE;
  }

   if (strcmp(data, "DFT") == 0)
  {
    // SET THE INITAL STATE
    
    return SET_DEFAULT;
  }

  return NONE;
}
