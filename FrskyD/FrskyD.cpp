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
 * * compared to SP, the D protocol has no slowness issue (no time based polling) - then sensor can take as much time
 *   it needs to send data. Anyhow, don't send too much data without delay. It could saturate the receiver buffer and
 *   corrupt data.
 * * in Frsky's white paper, packets are supposed to be sent as frames (sets of data), on a period base
 * * the frames are used by the D hub, be are not necessary to OpenTX - only one value can be sent at a time
 * * by the way, the VFAS sensor does not use frames, but send individual packets, and so for the Smart Port altimeters
 *   that can be connected on D series receivers as well.
 * 
 * Frsky's hub timing behavior:
 * frame | period | sensors
 * ----- | ------ | -------
 * 1 | 200ms | accelerometers, altitude, temperature, voltage, current, rpm
 * 2 | 1s    | GPS (except date and time), fuel level
 * 3 | 5s    | GPS date and time
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
 * \see https://github.com/jcheger/frsky-arduino/ - source of this library
 * \see http://www.frsky-rc.com/
 * \see http://www.open-tx.org/
 * \copyright 2014 - Jean-Christophe Heger - Released under the LGPL 3.0 license.
 * \ChangeLog 2014-06-27 - public devel release
 */

/**
 * \file FrskyD.cpp
 */
  
#include "Arduino.h"
#include "FrskyD.h"
#include "SoftwareSerial.h"

#define DEBUG

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
 * Return a float value, composed by 2 bytes as signed integer and 2 bytes as decimals.
 * Refered as \a before "." \a and  \a after "." \a values.
 * \param bp value before "."
 * \param ap value after "."
 */
float FrskyD::calcFloat (int16_t bp, uint16_t ap) {
    if (bp > 0) return (float) bp + (float) ap / 100.0;
    else        return (float) bp - (float) ap / 100.0;
}

/**
 * Return 2 bytes as signed integer
 * 
 * Those packets use this format:
 * * \ref FRSKY_D_ACCX (/1000)
 * * \ref FRSKY_D_ACCY (/1000)
 * * \ref FRSKY_D_ACCZ (/1000)
 * * \ref FRSKY_D_CURRENT (/100)
 * * \ref FRSKY_D_FUEL
 * * \ref FRSKY_D_GPS_SEC
 * * \ref FRSKY_D_GPS_YEAR
 * * \ref FRSKY_D_RPM
 * * \ref FRSKY_D_TEMP1
 * * \ref FRSKY_D_TEMP2
 * \param buffer buffer pointer
 */
int16_t FrskyD::decodeInt (byte *buffer) {
    return (buffer[1] << 8 | buffer[0]);
}

/**
 * Same as decodeInt(), but return only the first byte.
 * 
 * 2 type of packets use this format:
 * * \ref FRSKY_D_GPS_DM
 * * \ref FRSKY_D_GPS_HM
 * \param buffer buffer pointer
 */
uint8_t FrskyD::decode1Int (byte *buffer) {
    return buffer[0];
}

/**
 * \brief Decode the cell voltage of the \ref FRSKY_D_CELL_VOLT packets
 * \param buffer buffer pointer
 * \return voltage [V]
 */
float FrskyD::decodeCellVolt (byte *buffer) {
    return ((buffer[0] & 0x0F) << 8 | buffer[1]) / 500.0;
}

/**
 * \brief Decode cell ID of the \ref FRSKY_D_CELL_VOLT packets
 * \param buffer buffer pointer
 * \return cell ID
 */
int FrskyD::decodeCellVoltId (byte *buffer) {
    return (buffer[0] & 0xF0) >> 4;
}

/**
 * \brief Decode the \ref FRSKY_D_GPS_LAT_B packets
 * \param bp value B (before ".")
 * \param ap value A (after ".")
 * \return latitude
 */
String FrskyD::decodeGpsLat (int16_t bp, uint16_t ap) {
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
 * \brief Decode the \ref FRSKY_D_GPS_LONG_B packets
 * \param bp value B (before ".")
 * \param ap value A (after ".")
 * \return longitude
 */
String FrskyD::decodeGpsLong (int16_t bp, uint16_t ap) {
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
 * \todo forbidden values are not corrected yet
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
 * \brief Send lipo cell voltage (specific to \ref FRSKY_D_CELL_VOLT)
 * \param id cell ID
 * \param val voltage
 */
void FrskyD::sendCellVolt (uint8_t id, float val) {
    uint16_t voltage = val * 500;
    uint8_t v1 = (voltage & 0x0f00) >> 8 | (id << 4 & 0xf0);
    uint8_t v2 = (voltage & 0x00ff);
    uint16_t value = (v1 & 0x00ff) | (v2 << 8);
    this->sendData (FRSKY_D_CELL_VOLT, value);
}

/**
 * Send a value (D protocol)
 * In the D protocol, the hub sends frames periodically @ 9600 bds.
 *
 * frame | period | sensors
 * ----- | ------ | -------
 * 1 | 200ms | accelerometers, altitude, temperature, voltage, current, rpm
 * 2 | 1s    | GPS (except date and time), fuel level
 * 3 | 5s    | GPS date and time
 *
 * \see http://www.rcgroups.com/forums/showthread.php?t=1874973
 * \param id sensor ID
 * \param val preformated value
 */
void FrskyD::sendData (uint8_t id, int16_t val) {
    int8_t d[2];
    int i;
    
    d[0] =  val & 0x00ff;
    d[1] = (val & 0xff00) >> 8;

    this->mySerial->write (0x5E);
    this->mySerial->write (id);

    for (i=0; i<2; i++) {
        if      (d[i] == 0x5E) { this->mySerial->write (0x5D); this->mySerial->write (0x3E); }
        else if (d[i] == 0x5D) { this->mySerial->write (0x5D); this->mySerial->write (0x3D); }
        else                     this->mySerial->write (d[i]);
    }

    this->mySerial->write (0x5E);   // End of frame
}

/**
 * Send a float as 2 packets (before "." and after ".")
 * \param idb ID B (before ".")
 * \param ida ID A (after ".")
 * \param val value
 */
void FrskyD::sendFloat (uint8_t idb, uint8_t ida, float val) {
    int16_t  bp = (int16_t) val;
    uint16_t ap;
    if (val >= 0) ap = (val - bp) * 100;
    else          ap = (bp - val) * 100; 
    Serial.print ("float: ");
    Serial.print (val);
    Serial.print (", bp: ");
    Serial.print (bp);
    Serial.print (", ap: ");
    Serial.println (ap);
    this->sendData (idb, bp);
    this->sendData (ida, ap);
}
