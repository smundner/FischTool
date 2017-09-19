#include "bmp180.h"
#include <Wire.h>



static int32_t calcB5(int32_t ut) {
  int32_t X1 = (ut - (int32_t)BMP180CONST.ac6) * ((int32_t)BMP180CONST.ac5) >> 15;
  int32_t X2 = ((int32_t)BMP180CONST.mc << 11) / (X1 + (int32_t)BMP180CONST.md);
  return X1 + X2;
}

static byte getByte(byte addr, byte reg) {
  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();


  Wire.requestFrom((uint8_t)addr, 1);
  byte b = Wire.read();
  Wire.endTransmission();
  return b;
}
static unsigned short getUShort(byte addr, byte reg) {
  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t) reg);
  Wire.endTransmission();


  Wire.requestFrom((uint8_t)addr, 2);
  unsigned short us = Wire.read() << 8 | Wire.read();
  Wire.endTransmission();
  return us;
}
static short getShort(byte addr, byte reg) {
  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t) reg);
  Wire.endTransmission();


  Wire.requestFrom((uint8_t)addr, 2);
  unsigned short us = Wire.read() << 8 | Wire.read();
  short s = (short) us;
  Wire.endTransmission();
  return s;
}

static void setValue(byte addr, byte reg, byte val) {
  Wire.beginTransmission((uint8_t) addr);
  Wire.write((uint8_t) reg);
  Wire.write((uint8_t) val);
  Wire.endTransmission();
}

static int32_t getUT(){
  int32_t ut; 
  setValue(BMP180_ADDRESSE, 0xF4, 0x2E);
  delay(5);
  ut = (long)getShort(BMP180_ADDRESSE, 0xF6);
  return ut;
  }
  
static int32_t getUP(){
  int32_t up=0; 
  setValue(BMP180_ADDRESSE, 0xF4, 0x34 | (BMP180_MODE << 6));
  switch (BMP180_MODE) {
    case BMP180_MODE_ULTRALOWPOWER:
      delay(5);
      break;
    case BMP180_MODE_STANDARD:
      delay(8);
      break;
    case BMP180_MODE_HIGHRES:
      delay(14);
      break;
    case BMP180_MODE_ULTRAHIGHRES:
    default:
      delay(26);
      break;
  }
  up =  getUShort(BMP180_ADDRESSE, 0xF6);
  up <<= 8;
  up += getByte(BMP180_ADDRESSE, 0xF6 + 2);
  up >>= (8 - BMP180_MODE);
  return up;
  }
void Bmp180::begin() {
  BMP180CONST.ac1 = getShort(BMP180_ADDRESSE, 0xAA);
  BMP180CONST.ac2 = getShort(BMP180_ADDRESSE, 0xAC);
  BMP180CONST.ac3 = getShort(BMP180_ADDRESSE, 0xAE);
  BMP180CONST.ac4 = getUShort(BMP180_ADDRESSE, 0xB0);
  BMP180CONST.ac5 = getUShort(BMP180_ADDRESSE, 0xB2);
  BMP180CONST.ac6 = getUShort(BMP180_ADDRESSE, 0xB4);
  BMP180CONST.b1 = getShort(BMP180_ADDRESSE, 0xB6);
  BMP180CONST.b2 = getShort(BMP180_ADDRESSE, 0xB8);
  BMP180CONST.mb = getShort(BMP180_ADDRESSE, 0xBA);
  BMP180CONST.mc = getShort(BMP180_ADDRESSE, 0xBC);
  BMP180CONST.md = getShort(BMP180_ADDRESSE, 0xBE);
}

byte Bmp180::getID() {
  byte b = getByte(BMP180_ADDRESSE, 0xD0);
  return b;
}
float Bmp180::getTemperatur() {
  int32_t ut, b5, t;
  ut=getUT();

  b5 = calcB5(ut);
  t = (b5 + 8) / 16;
  return t * 0.1;
}

int32_t Bmp180::getPressure() {
  int32_t compp = 0,up=0,ut=0;
  int32_t p, b3, b5, b6, x1, x2, x3;
  uint32_t b4, b7;
// Temperatur auslesen
  ut = getUT();
  b5 = calcB5(ut);
//Luftdruck auslesen
  up = getUP();

  b6 = b5 - 4000;
  x1 = (BMP180CONST.b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = (BMP180CONST.ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((int32_t) BMP180CONST.ac1) * 4 + x3) << BMP180_MODE) + 2) >> 2;
  x1 = (BMP180CONST.ac3 * b6) >> 13;
  x2 = (BMP180CONST.b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (BMP180CONST.ac4 * (uint32_t) (x3 + 32768)) >> 15;
  b7 = ((uint32_t) (up - b3) * (50000 >> BMP180_MODE));
  
  if (b7 < 0x80000000) {
    p = (b7 << 1) / b4;
  } 
  else 
  {
    p = (b7 / b4) << 1;
  }
  
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  compp = p + ((x1 + x2 + 3791) >> 4);
  return compp;
}
