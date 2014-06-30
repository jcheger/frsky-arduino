/*
 * Requirements
 * ------------
 * - FrskyD library - https://github.com/jcheger/frsky-arduino
 *
 * 
 * origin: https://github.com/jcheger/frsky-arduino
 * author: Jean-Christophe Heger <jcheger@ordinoscope.net>
 */

#include <FrskyD.h>
#include <SoftwareSerial.h>

FrskyD FrskyD (10, 11);

void setup() {
  Serial.begin (115200);
  Serial.println ("FrskyD sensor demo");
}

void loop () {
  static int i = 0;
  static float alt = 10.0;

  Serial << "Alt: " << alt << "\n";

  FrskyD.sendData (FRSKY_D_ACCX,  1.1 * 1000.0);
  FrskyD.sendData (FRSKY_D_ACCY, -1.2 * 1000.0);
  FrskyD.sendData (FRSKY_D_ACCZ,  1.3 * 1000.0);
  
  FrskyD.sendFloat (FRSKY_D_ALT_B, FRSKY_D_ALT_A, alt);

  delay (200);  // wait a bit to flush buffer

  FrskyD.sendCellVolt (0, 3.01);
  FrskyD.sendCellVolt (1, 3.02);
  FrskyD.sendCellVolt (2, 3.03);
  FrskyD.sendCellVolt (3, 3.04);
  FrskyD.sendCellVolt (4, 3.05);
  FrskyD.sendCellVolt (5, 3.06);
  FrskyD.sendCellVolt (6, 3.07);
  FrskyD.sendCellVolt (7, 3.08);
  FrskyD.sendCellVolt (8, 3.09);
  FrskyD.sendCellVolt (9, 3.10);
  FrskyD.sendCellVolt (10, 3.11);
  FrskyD.sendCellVolt (11, 3.12);

  delay (200);  // wait a bit to flush buffer

  FrskyD.sendData (FRSKY_D_FUEL, 23);

  FrskyD.sendFloat (FRSKY_D_GPS_ALT_B, FRSKY_D_GPS_ALT_A, alt);
  
  FrskyD.sendFloat (FRSKY_D_GPS_COURSE_B, FRSKY_D_GPS_COURSE_A, 12.34);
  
  FrskyD.sendData  (FRSKY_D_GPS_DM, 10 << 8 | 12);
  FrskyD.sendData  (FRSKY_D_GPS_HM, 14 << 8 | 25);
  
  // 46° 56' 52.52''
  FrskyD.sendData (FRSKY_D_GPS_LAT_B, 46 * 100 + 42);
  FrskyD.sendData (FRSKY_D_GPS_LAT_A, 52.52 * 10000 / 60);
  
  // 7° 26' 40.59''
  FrskyD.sendData (FRSKY_D_GPS_LONG_B, 7 * 100 + 26);
  FrskyD.sendData (FRSKY_D_GPS_LONG_A, 40.59 * 10000 / 60);

  FrskyD.sendData  (FRSKY_D_GPS_SEC, 36);
  FrskyD.sendFloat (FRSKY_D_GPS_SPEED_B, FRSKY_D_GPS_SPEED_A, 100 / 1.852);  // OK, BUG
  FrskyD.sendData  (FRSKY_D_GPS_YEAR, 14);

  delay (200);  // wait a bit to flush buffer

  FrskyD.sendData  (FRSKY_D_TEMP1, 28);
  FrskyD.sendData  (FRSKY_D_TEMP2, -18);
  FrskyD.sendData  (FRSKY_D_RPM, 11111 / 60);

  FrskyD.sendData  (FRSKY_D_CURRENT, 23.4 * 10);
  FrskyD.sendData  (FRSKY_D_VFAS, 12.3 * 10);

  alt += .1;
  
  delay (200);  // wait a bit to flush buffer
}
