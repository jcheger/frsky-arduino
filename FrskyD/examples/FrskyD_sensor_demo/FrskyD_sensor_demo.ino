#include <FrskyD.h>
#include <SoftwareSerial.h>
#include <Streaming.h>

FrskyD FrskyD (10, 11);

void setup() {
  Serial.begin (115200);
  Serial.println ("BEGIN");
}

void loop () {
  static int i = 0;
  static float alt = 0.0;

  FrskyD.sendAcc (1.1, -1.2, 1.3);     // -8g ~ +8g                OK
  FrskyD.sendAlt (100 + alt);          // -500 ~ 9000m             FAIL
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
  FrskyD.sendFuel (23);                // 0 ~ 100%                 OK
  FrskyD.sendGpsAlt (alt);             //                          OK
  FrskyD.sendGpsDayMonth (10, 12);
  FrskyD.sendGpsHourMin (14, 25);
  FrskyD.sendGpsLat (46, 42, 46, 43);
  FrskyD.sendGpsLong (6, 34, 20, 61);
  FrskyD.sendGpsSec (36);
  FrskyD.sendGpsSpeedKmph (50);        //                          BAD conversion
  FrskyD.sendGpsYear (14);
  FrskyD.sendTemp1 (28);               // -30 ~ 250°C - Nil = -20  OK
  FrskyD.sendTemp2 (-18);              // -30 ~ 250°C - Nil = -20  OK
  FrskyD.sendRpm (11111);              // 0 ~ 60000rpm             OK

  if (i > 10) {
    alt += 1;
    Serial << "Alt: " << alt << "\n";
  } else {
    i++;
  }
  
  delay (100);
}

