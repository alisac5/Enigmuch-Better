#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include "libenig.h"
#include "Demo.h"

char cmdBuf[4];




void DemoController::begin(HT16K33_H* d) {
  disp = d;
  cmdBuf[0] = '\0';
  cmdBuf[1] = '\0';
  cmdBuf[2] = '\0';
}

void DemoController::loop() {
  static cmd_t cmd;

  cmd_t newCMD = processSerialMessages();
  if (newCMD.c != NONE) {
    cmd = newCMD;
  }
  if (cmd.c == SLIDE) {
    doSlide(cmd.arg0);
  }

}

void DemoController::doSlide(int slide) {
  switch (slide) {
  
    case 20 :
      disp->setText("    ");
      LedAllOff();
      break;
    case 30 :
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


void DemoController::stepDemoLighting() {
  static int undefined_int; //LOL
  undefined_int++;
  uint8_t i = undefined_int % 27;
  T led = intToQuertyOrder(i);
  LedWrite(led, HIGH);
  
  disp->setChar((char)led+65,0);
  disp->setChar((char)led+65,1);
  disp->setChar((char)led+65,2);
  disp->setChar((char)led+65,3);

  delay(300);
  LedWrite(led, LOW);
  delay(300);
  

}



cmd_t DemoController::processSerialMessages() {
  // First Check for any data on serial
  int A, B;
  cmd_t command;
  command.c = NONE;
  while (Serial.available() > 0) {
    char _byte = Serial.read();
    cmdBuf[0] = cmdBuf[1];
    cmdBuf[1] = cmdBuf[2];
    cmdBuf[2] = _byte;
    cmdBuf[3] = '\0';

    command.c = getCMD(cmdBuf);

    if (command.c != NONE) {

      switch (command.c) {
        case SLIDE:
          command.arg0 = Serial.parseInt();

          break;
        case NONE :
          break;
      }

      return command;
    }

  }


  return command;

}

// Fetch a command from the serial interface
commandType DemoController::getCMD(const char data[4]) {
  if (strcmp(data, "SLD") == 0) {
    return SLIDE;
  }

  return NONE;
}
