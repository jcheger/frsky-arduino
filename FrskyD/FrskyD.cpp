/** \mainpage
 * Arduino library for Frsky D protocol.
 * 
 * This library is not designed to decode data on the transmitter, but on the receiver side. OpenTX makes the rest of
 *  the job on the transmitter.
 * 
 * This development is completely independent of Frsky or OpenTX.
 *
 * What to know about D protocol
 * -----------------------------
 * * use an inverted serial communication at 9600bds
 * * in Frsky's white paper, packets are supposed to be sent as frames, on a period base
 *  * Frame1 (200ms) - accelerometers, altitude, temperature, voltage, current, rpm
 *  * Frame2 (1s)    - GPS (except date and time), fuel level
 *  * Frame3 (5s)    - GPS date and time
 * * the frames are used by the D hub, be is not necessary to OpenTX - only one value can be sent at a time
 * 
 * Connection draft
 * ----------------
 *  \image html D_ports_bb.png
 * 
 * * pull down resistor on RX line (100k)
 * * pull down resistor on TX line (100k)
 * 
 * \version devel
 * \author Jean-Christophe Heger
 * \see http://www.frsky-rc.com/
 * \see http://www.open-tx.org/
 * \copyright 2014 - Jean-Christophe Heger - Released under the LGPL 3.0 license.
 * \ChangeLog 2014-06-27 - public devel release
 */
 
#include "Arduino.h"
#include "FrskyD.h"
#include "SoftwareSerial.h"

/**
 * Open a SoftwareSerial connection
 * \param pinRx RX pin
 * \param pinTx TX pin
 * \brief Class constructor
 */
FrskyD::FrskyD (int pinRx, int pinTx) {
    this->mySerial = new SoftwareSerial (pinRx, pinTx, true);
    this->mySerial->begin (9600);
}

/**
 * Check if a byte is available
 * \brief SoftwareSerial.available() passtrhough
 */
bool FrskyD::available () {
    return this->mySerial->available ();
}

/**
 * \brief Decode the \ref FRSKY_D_ACCX, \ref FRSKY_D_ACCY and \ref FRSKY_D_ACCZ packets
 * \param buffer buffer pointer
 */
float FrskyD::decodeAcc (byte *buffer) {
    return (buffer[1] << 8 | buffer[0]) / 1000.0;
}

/**
 * \brief Decode the \ref FRSKY_D_ALT_B packets
 * \param buffer buffer pointer
 */
float FrskyD::decodeAlt (byte *buffer) {
    int16_t  bp = (buffer[1] << 8 | buffer[0]);
    uint16_t ap = (buffer[4] << 8 | buffer[3]);
    return (float) bp + (float) ap / 100.0;
}

/**
 * \brief Decode the cell voltage of the \ref FRSKY_D_VOLTAGE_B packets
 * \param buffer buffer pointer
 * \return voltage [V]
 */
float FrskyD::decodeCellVolt (byte *buffer) {
    return ((buffer[0] & 0x0F) << 8 | buffer[1]) / 500.0;
}

/**
 * \brief Decode cell ID of the \ref FRSKY_D_VOLTAGE_B packets
 * \param buffer buffer pointer
 * \return cell ID
 */
int FrskyD::decodeCellVoltId (byte *buffer) {
    return (buffer[0] & 0xF0) >> 4;
}

/**
 * \brief Decode the \ref FRSKY_D_FUEL packets
 * \param buffer buffer pointer
 * \return fuel level [%]
 */
int FrskyD::decodeFuel (byte *buffer) {
    return (buffer[1] << 8 | buffer[0]);
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_ALT_B packets
 * \param buffer buffer pointer
 * \return altitude [m]
 */
float FrskyD::decodeGpsAlt (byte *buffer) {
    int16_t bp = (buffer[1] << 8 | buffer[0]);
    uint16_t ap = (buffer[5] << 8 | buffer[4]);
    return (float) bp + (float) ap / 100.0;
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_COURSE_B packets
 * \param buffer buffer pointer
 * \return course
 */
float FrskyD::decodeGpsCourse (byte *buffer) {
    int16_t  bp = (buffer[1] << 8 | buffer[0]);
    uint16_t ap = (buffer[5] << 8 | buffer[4]);
    return (float) bp + (float) ap / 100.0;
}

/**
 * \brief Decode the day of the \ref FRSKY_D_GPS_DM packets
 * \param buffer buffer pointer
 * \return day
 */
int FrskyD::decodeGpsDay (byte *buffer) {
    return buffer[0];
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_DM packets
 * \param buffer buffer pointer
 * \return hour
 */
int FrskyD::decodeGpsHour (byte *buffer) {
    return buffer[0];
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_LAT_B packets
 * \param buffer buffer pointer
 * \return latitude
 */
String FrskyD::decodeGpsLat (byte *buffer) {
    int16_t  bp = (buffer[1] << 8 | buffer[0]);
    uint16_t ap = (buffer[5] << 8 | buffer[4]);
  
    int   d    = bp / 100;            // two frist numbers of bp
    int   m    = bp % 100;            // two last numbers of bp
    float s    = ap * 0.006;          // * 60 / 1000
    int   sint = (int) s;             // decimal part of s
    int   sdec = (s - sint) * 10000;  // floating part of s
  
    String gpsLatRaw = String (String(bp) + "." + String(ap));
    String gpsLatStr = String (String(d) + "d " + String(m) + "m " + String(sint) + "." + String (sdec) + "s");
    return String (gpsLatStr+ " (raw: " + gpsLatRaw + ")");
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_LAT_NS packets
 * \param buffer buffer pointer
 * \return ?
 */
int FrskyD::decodeGpsLatNS (byte *buffer) {
    return 0;
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_LONG_B packets
 * \param buffer buffer pointer
 * \return longitude
 */
String FrskyD::decodeGpsLong (byte *buffer) {
    int16_t  bp = (buffer[1] << 8 | buffer[0]);
    uint16_t ap = (buffer[5] << 8 | buffer[4]);
  
    int   d    = bp / 100;            // two frist numbers of bp
    int   m    = bp % 100;            // two last numbers of bp
    float s    = ap * 0.006;          // * 60 / 1000
    int   sint = (int) s;             // decimal part of s
    int   sdec = (s - sint) * 10000;  // floating part of s
  
    String gpsLongRaw = String (String(bp) + "." + String(ap));
    String gpsLongStr = String (String(d) + "d " + String(m) + "m " + String(sint) + "." + String (sdec) + "s");
    return String (gpsLongStr + " (raw: " + gpsLongRaw + ")");
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_LONG_EW packets
 * \param buffer buffer pointer
 * \return ?
 */
int FrskyD::decodeGpsLongEW (byte *buffer) {
    return 0;
}

/**
 * \brief Decode the minutes the \ref FRSKY_D_GPS_HM packets
 * \param buffer buffer pointer
 * \return minutes
 */
int FrskyD::decodeGpsMin (byte *buffer) {
    return buffer[1];
}

/**
 * \brief Decode the month the \ref FRSKY_D_GPS_DM packets
 * \param buffer buffer pointer
 * \return month
 */
int FrskyD::decodeGpsMonth (byte *buffer) {
    return buffer[1];
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_SPEED_B packets
 * \param buffer buffer pointer
 * \return knots
 */
float FrskyD::decodeGpsSpeed (byte *buffer) {
    int16_t  bp = (buffer[1] << 8 | buffer[0]);
    uint16_t ap = (buffer[5] << 8 | buffer[4]);
    return (float) bp + (float) ap / 100.0;
}

/**
 * \brief Decode of the \ref FRSKY_D_GPS_SEC packets
 * \param buffer buffer pointer
 * \return seconds
 */
int FrskyD::decodeGpsSec (byte *buffer) {
    return buffer[0];
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_YEAR packets
 * \param buffer buffer pointer
 * \return year
 */
int FrskyD::decodeGpsYear (byte *buffer) {
    return buffer[0];
}

/**
 * \brief Decode the \ref FRSKY_D_TEMP1 or \ref FRSKY_D_TEMP2 packets
 * \param buffer buffer pointer
 * \return temperature [°C]
 */
int FrskyD::decodeTemp (byte *buffer) {
    return (buffer[1] << 8 | buffer[0]);
}

/**
 * \brief Decode the \ref FRSKY_D_RPM packets
 * \param buffer buffer pointer
 * \return RPM
 */
int FrskyD::decodeRpm (byte *buffer) {
    return (buffer[1] << 8 | buffer[0]) * 60;
}

/**
 * \todo
 */
uint16_t FrskyD::_fixForbiddenValues (uint16_t val) {
    uint8_t byte1 = val & 0x00ff;
    uint8_t byte2 = (val & 0xff00) >> 8;
    // TODO
}

/**
 * \brief SoftwareSerial.read() passthrough
 */
byte FrskyD::read () {
    return this->mySerial->read ();
}

/**
 * Send a value (D protocol)
 * In the D protocol, the hub sends frames periodically @ 9600 bds.
 *
 * \see http://www.rcgroups.com/forums/showthread.php?t=1874973
 * \param id sensor ID
 * \param val preformated value
 */
void FrskyD::sendData (uint8_t id, uint16_t val) {
    uint8_t byte1 = val & 0x00ff;
    uint8_t byte2 = (val & 0xff00) >> 8;
    this->mySerial->write (0x5E);
    this->mySerial->write (id);

    if      (byte1 == 0x5E) { this->mySerial->write (0x5D); this->mySerial->write (0x3E); }
    else if (byte1 == 0x5D) { this->mySerial->write (0x5D); this->mySerial->write (0x3D); }
    else                    { this->mySerial->write (byte1); }

    if      (byte2 == 0x5E) { this->mySerial->write (0x5D); this->mySerial->write (0x3E); }
    else if (byte2 == 0x5D) { this->mySerial->write (0x5D); this->mySerial->write (0x3D); }
    else                    { this->mySerial->write (byte2); }

    this->mySerial->write (0x5E);
}

/**
 * \brief Send accelerometer values
 * \param valX X value [g] - (\ref FRSKY_D_ACCX)
 * \param valY Y value [g] - (\ref FRSKY_D_ACCY)
 * \param valZ Z value [g] - (\ref FRSKY_D_ACCZ)
 */
void FrskyD::sendAcc (float valX, float valY, float valZ) {
    this->sendData (FRSKY_D_ACCX, int16_t (valX * 1000));
    this->sendData (FRSKY_D_ACCY, int16_t (valY * 1000));
    this->sendData (FRSKY_D_ACCZ, int16_t (valZ * 1000));
}

/**
 * \brief Send altitude
 * \param val altitude
 */
void FrskyD::sendAlt (float val) {
    int16_t  bp = int16_t (val);
    uint16_t ap = uint16_t ((abs(val) - abs(bp)) * 100);
    this->sendData (FRSKY_D_ALT_B, bp);
    this->sendData (FRSKY_D_ALT_A, ap);
}

/**
 * \brief Send lipo cell voltage
 * \param cellID cell ID
 * \param val voltage
 */
void FrskyD::sendCellVolt (uint8_t cellID, float val) {
    uint16_t voltage = val * 500;
    uint8_t v1 = (voltage & 0x0f00) >> 8 | (cellID << 4 & 0xf0);
    uint8_t v2 = (voltage & 0x00ff);
    uint16_t value = (v1 & 0x00ff) | (v2 << 8);
    this->sendData (FRSKY_D_CELL_VOLT, value);
}

/**
 * \brief Send fuel level (\ref FRSKY_D_FUEL)
 * \param val fuel level [%]
 */
void FrskyD::sendFuel (uint16_t val) {
    this->sendData (FRSKY_D_FUEL, val);
}

/**
 * \brief Send GPS altitude (\ref FRSKY_D_ALT_B)
 * \param val altitude [m]
 */
void FrskyD::sendGpsAlt (float val) {
    uint16_t bp = (int) val;
    uint16_t ap = (int) ((val-bp) * 100);
    this->sendData (FRSKY_D_GPS_ALT_B, bp);
    this->sendData (FRSKY_D_GPS_ALT_A, ap);
}

/**
 * \brief Send GPS course (\ref FRSKY_D_GPS_COURSE_B)
 * \param val course [°]
 * \todo
 */
void FrskyD::sendGpsCourse (float val) {
    
}

/**
 * \brief Send GPS lalitude (\ref FRSKY_D_GPS_LAT_B)
 * \todo
 */
void FrskyD::sendGpsLat (uint8_t d, uint8_t m, uint8_t mm1, uint8_t mm2) {
    uint16_t v1  = m << 8 | d;
    uint16_t v2 = mm1 << 8 | mm2;
    this->sendData (FRSKY_D_GPS_LAT_B, v1);
    this->sendData (FRSKY_D_GPS_LAT_A, v2);
}

/**
 * \brief Send GPS longitude (\ref FRSKY_D_GPS_LONG_B)
 * \todo
 */
void FrskyD::sendGpsLong (uint8_t d, uint8_t m, uint8_t mm1, uint8_t mm2) {
    uint16_t v1 = (m << 8) | d;
    uint16_t v2 = mm1 << 8 | mm2;
    this->sendData (FRSKY_D_GPS_LONG_B, v1);
    this->sendData (FRSKY_D_GPS_LONG_A, v2);
}

/**
 * \brief Send GPS day and month (\ref FRSKY_D_GPS_DM)
 * \param day day
 * \param month month
 * \todo
 */
void FrskyD::sendGpsDayMonth (uint8_t day, uint8_t month) {
    this->sendData (FRSKY_D_GPS_DM, (day << 8) | month);
}

/**
 * \brief Send GPS hour and minute (\ref FRSKY_D_GPS_HM)
 * \param hour hour
 * \param min minute
 * \todo
 */
void FrskyD::sendGpsHourMin (uint8_t hour, uint8_t min) {
    this->sendData (FRSKY_D_GPS_HM, (hour << 8) | min);
}

/**
 * \brief Send GPS seconds (\ref FRSKY_D_GPS_SEC)
 * \param val second
 * \todo
 */
void FrskyD::sendGpsSec (uint16_t val) {
    this->sendData (FRSKY_D_GPS_SEC, val);
}

/**
 * \brief Send GPS speed (\ref FRSKY_D_GPS_SPEED_B)
 * \param val knots
 */
void FrskyD::sendGpsSpeed (float val) {
    uint16_t v1 = (int) val;
    uint16_t v2 = (val - v1) * 1000;
    this->sendData (FRSKY_D_GPS_SPEED_B, v1);
    this->sendData (FRSKY_D_GPS_SPEED_A, v2);
}

/**
 * \brief Send GPS speed (\ref FRSKY_D_GPS_SPEED_B)
 * \param val kmph
 * \bug
 */
void FrskyD::sendGpsSpeedKmph (float val) {
  this->sendGpsSpeed (val / 1.852);
}

/**
 * \brief Send GPS speed (\ref FRSKY_D_GPS_SPEED_B)
 * \param val mph
 */
void FrskyD::sendGpsSpeedMph (float val) {
  this->sendGpsSpeed (val / 1.151);
}

/**
 * \brief Send GPS year (\ref FRSKY_D_GPS_YEAR)
 * \param val year
 */
void FrskyD::sendGpsYear (uint16_t val) {
    this->sendData (FRSKY_D_GPS_YEAR, val);
}

/**
 * \brief Send RPM (\ref FRSKY_D_RPM)
 * \param val rpm
 */
void FrskyD::sendRpm (uint16_t val) {
    this->sendData (FRSKY_D_RPM, val/60);
}

/**
 * \brief Send Temp1 (\ref FRSKY_D_TEMP1)
 * \param val temperature [°C]
 */
void FrskyD::sendTemp1 (int16_t val) {
    this->sendData (FRSKY_D_TEMP1, val);
}

/**
 * \brief Send Temp2 (\ref FRSKY_D_TEMP2)
 * \param val temperature [°C]
 */
void FrskyD::sendTemp2 (int16_t val) {
    this->sendData (FRSKY_D_TEMP2, val);
}
