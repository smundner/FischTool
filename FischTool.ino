#import <Wire.h>
#import "bmp180.h"
#import "display.h"
#import "logger.h"

char temp_a[5];
char temp_s[5];
char pressure[6];

LogStruct logging;

Bmp180 test;
Display dsp;

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(9600);
  test.begin();
  dsp.begin();
  dsp.display();
  analogReference(INTERNAL);

}

int8_t analog_to_temp(uint16_t data){
  long temp = map(data,0,930,0,100);
  return (int8_t) temp;//+0.5;
  }

void loop() {
  logging.temp_a = (int8_t)test.getTemperatur()+0.5;
  logging.temp_s = analog_to_temp(analogRead(A0));
  logging.pressure = test.getPressure() / 100;
  
  sprintf(temp_a, "%3i", logging.temp_a);
  sprintf(temp_s, "%3i", logging.temp_s);
  sprintf(pressure, "%4i",logging.pressure);

  dsp.writeChars(50, 10, pressure);
  dsp.writeChars(17,45,temp_a);
  dsp.writeChars(85,45,temp_s);
  dsp.display();

  Serial.println("!!");
  Serial.print("A:");Serial.println(logging.temp_a);
  Serial.print("S:");Serial.println(logging.temp_s);
  Serial.print("P:");Serial.println(logging.pressure);
  Serial.println("&&");
  delay(1000);

}
