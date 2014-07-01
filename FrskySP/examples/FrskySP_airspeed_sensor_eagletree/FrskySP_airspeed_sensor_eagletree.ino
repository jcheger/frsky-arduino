/*
 * This code is not ready for production yet !
 * 
 * Documentation
 * -------------
 * http://www.eagletreesystems.com/Manuals/microsensor-i2c.pdf
 *
 *  
 * origin: https://github.com/jcheger/frsky-arduino
 * author: Jean-Christophe Heger <jcheger@ordinoscope.net>
 */

#include <Wire.h>

// Use DEBUG 1 to compile the serial debug support
#define DEBUG 1

#define ASP_V3 0xEA >> 1
#define ALT_V3 0xE8 >> 1
#define ALT_V4 0xEC >> 1  // barometric + 3000 decimeter offset
#define GFORCE 0xA8 >> 1

void setup () {
  #if DEBUG
  Serial.begin (115200);
  Serial.println ("DEBUG mode");
  #endif

  Wire.begin();
}

void loop () {
  int raw;
  Wire.beginTransmission (ASP_V3);
  Wire.write (0x07);
  Wire.endTransmission ();

  Wire.beginTransmission (ASP_V3);
  Wire.requestFrom (ASP_V3, 2);
  
  if (Wire.available ()) raw  = Wire.read ();
  #if DEBUG
  else Serial.println ("ERROR on 1st byte");
  #endif

  if (Wire.available ()) raw |= (int) Wire.read () << 8;
  #if DEBUG
  else Serial.println ("ERROR on 2nd byte");
  #endif

  Wire.endTransmission ();

  #if DEBUG
  Serial.print (raw, HEX);
  Serial.print (" ");
  Serial.println (raw, DEC);
  #endif
  
  delay(200);
}
