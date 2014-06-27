#include <FrskyD.h>
#include <SoftwareSerial.h>
#include <Streaming.h>

FrskyD FrskyD (10, 11);

void setup() {
  Serial.begin (115200);
  Serial.println ("BEGIN");
}

void loop () {
  static byte buffer[100];
  static int idx = 0;
  int i;

  while (FrskyD.available ()) {
    buffer[idx] = FrskyD.read ();
    if (buffer[idx-1] == 0x5E && buffer[idx] == 0x5E) {
      Serial << "\n" << "--";
      for (i = 0; i < idx; i++) Serial << _HEX(buffer[i]) << " ";
      Serial << " (" << idx << ")\n";
      decode_frame (buffer, idx);
      buffer[0] = 0x5E;
      idx = 1;
    } else {
      idx++;
      if (idx > 100) {  // error
        buffer[0] = 0xE;
        idx = 1;
      }
    }
  }
}

void decode_frame (byte *buffer, int length) {
  int i = 1;
  
  while (i < length) {

    switch (buffer[i]) {

      case FRSKY_D_ACCX:         Serial << "AccelX:     " << FrskyD.decodeAcc  (&buffer[i+1]) << "\n"; break;       // OK
      case FRSKY_D_ACCY:         Serial << "AccelY:     " << FrskyD.decodeAcc  (&buffer[i+1]) << "\n"; break;       // OK
      case FRSKY_D_ACCZ:         Serial << "AccelZ:     " << FrskyD.decodeAcc  (&buffer[i+1]) << "\n"; break;       // OK
      case FRSKY_D_ALT_B:        Serial << "BaroAlt:    " << FrskyD.decodeAlt (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_ALT_A:        break;
      case FRSKY_D_CELL_VOLT:    Serial << "CellV[" << FrskyD.decodeCellVoltId (&buffer[i+1]) << "]:   " << FrskyD.decodeCellVolt (&buffer[i+1]) << "\n"; break;  // OK
      case FRSKY_D_FUEL:         Serial << "Fuel:       " << FrskyD.decodeFuel (&buffer[i+1]) << "\n"; break;       // OK
      case FRSKY_D_GPS_ALT_B:    Serial << "GpsAlt:     " << FrskyD.decodeGpsAlt (&buffer[i+1]) << "\n"; break;     // OK
      case FRSKY_D_GPS_ALT_A:    break;
      case FRSKY_D_GPS_COURSE_B: Serial << "GpsCourse:  " << FrskyD.decodeGpsCourse (&buffer[i+1]) << "\n"; break;  // OK
      case FRSKY_D_GPS_COURSE_A: break;
      case FRSKY_D_GPS_DM:       Serial << "Day, Month: " << FrskyD.decodeGpsDay (&buffer[i+1]) << " " << FrskyD.decodeGpsMonth (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_GPS_HM:       Serial << "Hour, Min:  " << FrskyD.decodeGpsHour (&buffer[i+1]) << " " << FrskyD.decodeGpsMin (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_GPS_LAT_B:    Serial << "GpsLat:     " << FrskyD.decodeGpsLat (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_GPS_LAT_A:    break;
      case FRSKY_D_GPS_LAT_NS:   Serial << "GpsLatNS:   " << FrskyD.decodeGpsLatNS (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_GPS_LONG_B:   Serial << "GpsLong:    " << FrskyD.decodeGpsLong (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_GPS_LONG_A:   break;
      case FRSKY_D_GPS_LONG_EW:  Serial << "GpsLongEW:  " << FrskyD.decodeGpsLongEW (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_GPS_SEC:      Serial << "Sec:        " << FrskyD.decodeGpsSec (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_GPS_SPEED_B:  Serial << "GpsSpeed:   " << FrskyD.decodeGpsSpeed (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_GPS_SPEED_A:  break;
      case FRSKY_D_GPS_YEAR:     Serial << "Year:       " << FrskyD.decodeGpsYear (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_RPM:          Serial << "Rpm:        " << FrskyD.decodeRpm  (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_TEMP1:        Serial << "Temp1:      " << FrskyD.decodeTemp (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_TEMP2:        Serial << "Temp2:      " << FrskyD.decodeTemp (&buffer[i+1]) << "\n"; break;
      default:
        Serial.print ("unknown:  ");
        while (buffer[i] != 0x5E) { Serial << _HEX(buffer[i]) << " "; i++; }
        i--;
        Serial.println ();
    }
    i++;
    while (buffer[i-1] != 0x5E) i++;
  }
}

