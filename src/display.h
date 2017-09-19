#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <Arduino.h>

#define DISPLAY_ADDRESS 0x3C

#define DISPLAY_COLUMNADDR 0x21
#define DISPLAY_PAGEADDR 0x22

#define DISPLAY_SETDISPLAYCLOCKDIV 0xD5
#define DISPLAY_SETMULTIPLEX 0xA8
#define DISPLAY_SETDISPLAYOFFSET 0xD3
#define DISPLAY_SETSTARTLINE 0x40
#define DISPLAY_CHARGEPUMP 0x8D
#define DISPLAY_MEMORYMODE 0x20
#define DISPLAY_SEGREMAP 0xA0
#define DISPLAY_COMSCANDEC 0xC8
#define DISPLAY_SETCOMPINS 0xDA
#define DISPLAY_SETCONTRAST 0x81

#define DISPLAY_SETPRECHARGE 0xD9
#define DISPLAY_SETVCOMDETECT 0xDB
#define DISPLAY_DISPLAYALLON_RESUME 0xA4
#define DISPLAY_NORMALDISPLAY 0xA6
#define DISPLAY_INVERTDISPLAY 0xA7
#define DISPLAY_DEACTIVATE_SCROLL 0x2E
#define DISPLAY_DISPLAYON 0xAF
#define DISPLAY_DISPLAYOFF 0xAE

class Display{
  public:
    void begin();
    void clear();
    void display();
    void drawLine(byte xb,byte xe,byte yb,byte ye);
    void writeChars(uint8_t xoff,uint8_t yoff,char *data);
  private:
    void writeChar(char data);
  };

#endif __DISPLAY_H__
