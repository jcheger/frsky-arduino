#include <FrskyD.h>
#include <SoftwareSerial.h>
#include <Streaming.h>

#define DEBUG 1

FrskyD FrskyD (10, 11);

void setup() {
  Serial.begin (115200);
  Serial.println ("BEGIN");
  //FrskyD.debug = 1;
}

void loop () {
  static int i = 0;
  static float alt = 10.0;

  Serial << "Alt: " << alt << "\n";

  // Frame1 (200ms)
  
  // Frame2 (1s)
  
  // Frame3 (5s)
  
  //FrskyD.sendData (FRSKY_D_ACCX,  1.1 * 1000.0);  // OK
  //FrskyD.sendData (FRSKY_D_ACCY, -1.2 * 1000.0);  // OK
  //FrskyD.sendData (FRSKY_D_ACCZ,  1.3 * 1000.0);  // OK
  
  //FrskyD.sendFloat (FRSKY_D_ALT_B, FRSKY_D_ALT_A, alt);  // OK

/*  
  FrskyD.sendCellVolt (0, 3.01);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (1, 3.02);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (2, 3.03);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (3, 3.04);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (4, 3.05);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (5, 3.06);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (6, 3.07);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (7, 3.08);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (8, 3.09);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (9, 3.10);       // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (10, 3.11);      // 0 ~ 8.18V                OK
  FrskyD.sendCellVolt (11, 3.12);      // 0 ~ 8.18V                OK
*/

  //FrskyD.sendData (FRSKY_D_FUEL, 23);  // OK
  
  //FrskyD.sendFloat (FRSKY_D_GPS_ALT_B, FRSKY_D_GPS_ALT_A, alt);  // OK
  
  //FrskyD.sendGpsDayMonth (10, 12);
  FrskyD.sendData (FRSKY_D_GPS_DM, 10 << 8 | 12);
  
  //FrskyD.sendGpsHourMin (14, 25);
  FrskyD.sendData (FRSKY_D_GPS_HM, 14 << 8 | 25);
  
  //FrskyD.sendData (FRSKY_D_GPS_LAT_B, 46 << 8 | 42);  // FAIL
  //FrskyD.sendData (FRSKY_D_GPS_LAT_A, 46 << 8 | 43);  // FAIL
  
  //FrskyD.sendData (FRSKY_D_GPS_LONG_B, 6 << 8 | 34);  // FAIL
  //FrskyD.sendData (FRSKY_D_GPS_LONG_A, 20 << 8 | 61); // FAIL
  
  // FrskyD.sendGpsSec (36);
  FrskyD.sendData (FRSKY_D_GPS_SEC, 36);
  
  //FrskyD.sendFloat (FRSKY_D_GPS_SPEED_B, FRSKY_D_GPS_SPEED_A, 100 / 1.852);  // OK, BUG

  // FrskyD.sendGpsYear (14);
  FrskyD.sendData (FRSKY_D_GPS_YEAR, 14);
  
  //FrskyD.sendData (FRSKY_D_TEMP1, 28);
  //FrskyD.sendData (FRSKY_D_TEMP2, -18);
  //FrskyD.sendData (FRSKY_D_RPM, 11111 / 60);  // OK

  //FrskyD.sendData (FRSKY_D_CURRENT, 23.4 * 10);  // OK
  FrskyD.sendFloat (FRSKY_D_VOLTAGE_B, FRSKY_D_VOLTAGE_A, 100.0 / 1.92); // FAIL

  FrskyD.sendEOF ();
  alt += 1;
  
  delay (200);
}
