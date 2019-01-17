/***** Small library for driving alphanumeric displays via HT16K33 Chip*/
#ifndef _HT16K33_H 
#define _HT16K33_H


// Segments

class HT16K33_H{
    public:
    /* Initalize HT16K33_H Chip. 
    Starts system oscilator*/
    void begin();
    /* Ignore. Used for Testing. Will be removed*/
    void loop();

    /* Turns the display on/off 1 = on 0 = off)
       Note : Display is off on startup
    */
    void setVisible(uint8_t visibility); 

    /** 
     * @requires strlen(ascii) = 4
     Sets the display to a ascii string of length 4. 
     The string *MUST* be of length 4 otherwisse undefined behaviore will occur. 
    */
    void setText(const char * ascii);
    /* @requires  0 <= display < 4
        Sets a single charachter on the display. 
    */
   /* sets a single charachter on the display. 
   @requires 0<= digit <4
   */
    void setChar(const char value, uint8_t digit);
    

    private: 
    /* Updates the display*/
    void update();
    /* Sets an ASCII Character in the display memory*/
    void setASCIIChar(const char value, uint8_t digit);
    /* The display memory*/
    uint16_t displayBuffer[8];

    
};


#endif 
