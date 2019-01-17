#ifndef _DEMO_H 
#define _DEMO_H
#include "HT16K33.h"


typedef enum {NONE =0,SLIDE=1} commandType;

typedef struct command{
  commandType  c; // The command 
  int arg0; 
  int arg1; 
  char c_arg; 
} cmd_t;

class DemoController{
    public:
    void begin(HT16K33_H * d);
    void loop();

    private:
    HT16K33_H * disp;
    cmd_t processSerialMessages();
    // Do functions in sync with slide number sent from PC
    void doSlide(int slide);
    commandType getCMD(const char data[4]);
    void stepDemoLighting();

    static const int CMD_NONE = -1; 
    static const int CMD_SLIDE = 0x10; 

};
#endif
