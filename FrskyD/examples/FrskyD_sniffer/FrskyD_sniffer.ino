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
      Serial << "\n" << "--- [FRAME] --- ";
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
  
  int16_t        alt_a, gps_alt_a, gps_course_a, gps_lat_a, gps_long_a, gps_speed_a, voltage_a;
  static int16_t alt_b, gps_alt_b, gps_course_b, gps_lat_b, gps_long_b, gps_speed_b, voltage_b;
  
  while (i < length) {

    switch (buffer[i]) {

      case FRSKY_D_ACCX:         Serial << "AccX:       " << FrskyD.decodeInt (&buffer[i+1]) / 1000.0 << " [g]\n"; break;
      case FRSKY_D_ACCY:         Serial << "AccY:       " << FrskyD.decodeInt (&buffer[i+1]) / 1000.0 << " [g]\n"; break;
      case FRSKY_D_ACCZ:         Serial << "AccZ:       " << FrskyD.decodeInt (&buffer[i+1]) / 1000.0 << " [g]\n"; break;

      case FRSKY_D_ALT_B:        alt_b = FrskyD.decodeInt (&buffer[i+1]); break;
      case FRSKY_D_ALT_A:        alt_a = FrskyD.decodeInt (&buffer[i+1]);
                                 Serial << "Alt:        " << FrskyD.calcFloat (alt_b, alt_a) << " [m]\n";
                                 break;

      case FRSKY_D_CELL_VOLT:    Serial << "CellV[" << FrskyD.decodeCellVoltId (&buffer[i+1]) << "]:   " << FrskyD.decodeCellVolt (&buffer[i+1]) << " [V]\n"; break;

      case FRSKY_D_FUEL:         Serial << "Fuel:       " << FrskyD.decodeInt (&buffer[i+1]) << " [%]\n"; break;
 
      case FRSKY_D_GPS_ALT_B:    gps_alt_b = FrskyD.decodeInt (&buffer[i+1]); break;
      case FRSKY_D_GPS_ALT_A:    gps_alt_a = FrskyD.decodeInt (&buffer[i+1]);
                                 Serial << "GpsAlt:     " << FrskyD.calcFloat (gps_alt_b, gps_alt_a) << " [m]\n";
                                 break;
 
      case FRSKY_D_GPS_COURSE_B: gps_course_b = FrskyD.decodeInt (&buffer[i+1]); break;
      case FRSKY_D_GPS_COURSE_A: gps_course_a = FrskyD.decodeInt (&buffer[i+1]);
                                 Serial << "GpsCourse:  " << FrskyD.calcFloat (gps_course_b, gps_course_a) << " [" << char(176) << "]\n";
                                 break;
 
      case FRSKY_D_GPS_DM:       Serial << "Day, Month: " << FrskyD.decode1Int (&buffer[i+1]) << " " << FrskyD.decode1Int (&buffer[i+2]) << "\n"; break;
      case FRSKY_D_GPS_HM:       Serial << "Hour, Min:  " << FrskyD.decode1Int (&buffer[i+1]) << " " << FrskyD.decode1Int (&buffer[i+2]) << "\n"; break;
 
      case FRSKY_D_GPS_LAT_B:    gps_lat_b = FrskyD.decodeInt (&buffer[i+1]); break;
      case FRSKY_D_GPS_LAT_A:    gps_lat_a = FrskyD.decodeInt (&buffer[i+1]); break;
                                 Serial << "GpsLat:     " << FrskyD.decodeGpsLat (gps_lat_b, gps_lat_a) << "\n";
                                 break;
      
      case FRSKY_D_GPS_LAT_NS:   Serial << "GpsLatNS:   " << FrskyD.decodeInt (&buffer[i+1]) << "\n"; break;

      case FRSKY_D_GPS_LONG_B:   gps_long_b = FrskyD.decodeInt (&buffer[i+1]); break;
      case FRSKY_D_GPS_LONG_A:   gps_long_a = FrskyD.decodeInt (&buffer[i+1]);
                                 Serial << "GpsLong:    " << FrskyD.decodeGpsLong (gps_long_b, gps_long_a) << "\n";
                                 break;
      
      case FRSKY_D_GPS_LONG_EW:  Serial << "GpsLongEW:  " << FrskyD.decodeInt (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_GPS_SEC:      Serial << "Sec:        " << FrskyD.decodeInt (&buffer[i+1]) << "\n"; break;

      case FRSKY_D_GPS_SPEED_B:  gps_speed_b = FrskyD.decodeInt (&buffer[i+1]); break;
      case FRSKY_D_GPS_SPEED_A:  gps_speed_b = FrskyD.decodeInt (&buffer[i+1]);
                                 Serial << "GpsSpeed:   " << FrskyD.calcFloat (gps_speed_b, gps_speed_a) << " [knots]\n";
                                 break;

      case FRSKY_D_GPS_YEAR:     Serial << "Year:       " << FrskyD.decodeInt (&buffer[i+1]) << "\n"; break;
      case FRSKY_D_RPM:          Serial << "Rpm:        " << FrskyD.decodeInt (&buffer[i+1]) << " [rpm]\n"; break;
      case FRSKY_D_TEMP1:        Serial << "Temp1:      " << FrskyD.decodeInt (&buffer[i+1]) << " [" << char(176) << "C]\n"; break;
      case FRSKY_D_TEMP2:        Serial << "Temp2:      " << FrskyD.decodeInt (&buffer[i+1]) << " [" << char(176) << "C]\n"; break;
      case FRSKY_D_CURRENT:      Serial << "Current:    " << FrskyD.decodeInt (&buffer[i+1]) << " [A]\n"; break;

      case FRSKY_D_VOLTAGE_B:    voltage_b = FrskyD.decodeInt (&buffer[i+1]); break;
      case FRSKY_D_VOLTAGE_A:    voltage_a = FrskyD.decodeInt (&buffer[i+1]);;
                                 Serial << "Voltage:    " << (float) (voltage_b * 10 + voltage_a) * 21 / 110 << " [V]\n";
                                 break;
      
      default:
        Serial << "unknown ID:    " << _HEX(buffer[i]) << "\n";
        Serial << "decodeInt:     " << FrskyD.decodeInt (&buffer[i+1]) << "\n";
        Serial << "decode1Int[0]: " << FrskyD.decodeInt (&buffer[i+1]) << "\n";
        Serial << "decode1Int[1]: " << FrskyD.decodeInt (&buffer[i+2]) << "\n";
        Serial.print ("HEX: ");
        while (buffer[i] != 0x5E) { Serial << _HEX(buffer[i]) << " "; i++; }
        i--;
        Serial.println ();
    }
    i++;
    while (buffer[i-1] != 0x5E) i++;
  }
}
