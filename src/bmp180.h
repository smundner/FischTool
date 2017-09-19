#ifndef __bmp180_h__
#define __bmp180_h__

#include <Arduino.h>

#define BMP180_ADDRESSE 0x77
#define BMP180_MODE 0
#define BMP180_MODE_ULTRALOWPOWER 0
#define BMP180_MODE_STANDARD 1
#define BMP180_MODE_HIGHRES 2
#define BMP180_MODE_ULTRAHIGHRES 3


struct {
  short ac1;
  short ac2;
  short ac3;
  unsigned short ac4;
  unsigned short ac5;
  unsigned short ac6;
  short b1;
  short b2;
  short mb;
  short mc;
  short md;
} BMP180CONST;


class Bmp180 {
  public:
    void begin();
    byte getID();
    float getTemperatur();
    int32_t getPressure();
  private:

};

#endif
