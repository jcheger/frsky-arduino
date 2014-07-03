/**
 * \file FrskyD.h
 */

#include "Arduino.h"
#include "SoftwareSerial.h"

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x01 (before "."), 0x01+8 (after ".")
 * value  | signed int [m], unsigned int [dm]
 * 
 * N.B. OpenTX:
 * * must have a GpsFix (FRSKY_SP_GPS_LAT_B or FRSKY_SP_GPS_LONG_B must be non null)
 * * use the first non-zero value and set it as offset reference.
 * 
 * \brief GPS altitude
 */
#define FRSKY_D_GPS_ALT_B    0x01    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x02
 * value  | unsigned int [°]
 * limits | TEMS-01: -30~250°C
 * NIL    | -20
 * 
 * \brief Temperature
 */
#define FRSKY_D_TEMP1        0x02    // TEMS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x03
 * value  | unsigned int [rpm]
 * limits | RPMS-01: 0 ~ 60000
 * 
 * \brief RPM
 */
#define FRSKY_D_RPM          0x03    // RPMS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x04
 * value  | unsigned int [%]
 * limits | 0 ~ 100
 * 
 * \brief Fuel level
 */
#define FRSKY_D_FUEL         0x04    // FGS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x05
 * value  | unsigned int [°C]
 * limits | -30~250°C
 * NIL    | -20
 * 
 * \brief Analog temperature
 */
#define FRSKY_D_TEMP2        0x05    // TEMS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x06
 * value  | (unsigned int) float * 500 [V]
 * limits | 0 ~ 8.18V
 * voltage mask | 0x0fff
 * cell ID mask | 0xf000 >> 4
 * 
 * Some methods a specific to this sensor:
 * * FrskyD::decodeCellVolt
 * * FrskyD::decodeCellVoltId
 * * FrskyD::sendCellVolt
 * 
 * \brief LiPo cell voltage
 */
#define FRSKY_D_CELL_VOLT    0x06    // FLVS-01

/**
 * see \ref FRSKY_D_GPS_ALT_B
 */
#define FRSKY_D_GPS_ALT_A    0x09    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x10 (before "."), 0x21 (after ".")
 * value  | signed int [m], unsigned int [m]
 * limits | -500 ~ 9000m
 * 
 * N.B. OpenTX use the first non-zero value and set it as offset reference.
 * 
 * \brief Barometric altitude
 */
#define FRSKY_D_ALT_B   0x10    // FVAS-01, FVAS-02

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x14, 0x14+8
 * value  | signed int [knots], unsigned in [dknots]
 * 
 * * 1 km   = 1.852 knots
 * * 1 mile = 1.151 knots
 * 
 * \brief GPS speed
 * \warning The speed shown on OpenTX has a little drift, because the knots to shown value conversion is simplified.
 * Allthough, raw knots will be recorded in the logs, and the conversion will be correctly in Companion.
 * This was discussed in this issue: https://github.com/opentx/opentx/issues/1422
 */
#define FRSKY_D_GPS_SPEED_B  0x11    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x12, 0x12+8
 * value  | (signed int) ddmm, (unisgned int) mmmm (ddmm.mmmm)
 *
 * 0.006s resolution step
 * 
 * \brief GPS longitude
 */
#define FRSKY_D_GPS_LONG_B   0x12    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x13, 0x13+8
 * value  | (signed int) ddmm, (unisgned int) mmmm (ddmm.mmmm)
 * 
 * 0.006s resolution step
 * 
 * \brief GPS latitude
 */
#define FRSKY_D_GPS_LAT_B    0x13    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x14, 0x14+8
 * value  | signed int [°], unsigned int [d°]
 * limits | 0 ~ 359.99°
 * 
 * \brief GPS course (heading)
 */
#define FRSKY_D_GPS_COURSE_B 0x14    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x15
 * value  | (uint8) day, (uint8) month
 *
 * \brief GPS GMT day and month
 */
#define FRSKY_D_GPS_DM       0x15    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x16
 * value  | (uint8) null, (uint8) year
 *
 * \brief GPS GMT year
 */
#define FRSKY_D_GPS_YEAR     0x16    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x17
 * value  | (uint8) hour, (uint8) minute
 * 
 * \brief GPS GMT hour and minute
 */
#define FRSKY_D_GPS_HM       0x17    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x18
 * value  | (uint8) null, (uint8) second
 *
 * \brief GPS GMT second
 */
#define FRSKY_D_GPS_SEC      0x18    // GPS-01

/**
 * see \ref FRSKY_D_GPS_SPEED_B
 */
#define FRSKY_D_GPS_SPEED_A  0x19    // GPS-01

/**
 * see \ref FRSKY_D_GPS_LONG_B
 */
#define FRSKY_D_GPS_LONG_A   0x1A    // GPS-01

/**
 * see \ref FRSKY_D_GPS_LAT_B
 */
#define FRSKY_D_GPS_LAT_A    0x1B    // GPS-01

/**
 * see \ref FRSKY_D_GPS_COURSE_B
 */
#define FRSKY_D_GPS_COURSE_A 0x1C    // GPS-01

/**
 * see \ref FRSKY_D_ALT_B
 */
#define FRSKY_D_ALT_A   0x21    // FVAS01, FVAS-02

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x22
 * value  | ?
 *
 * \todo GPS only works for East at now
 */
#define FRSKY_D_GPS_LONG_EW  0x22    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x23
 * value  | ?
 *
 * \todo GPS only works for North at now
 */
#define FRSKY_D_GPS_LAT_NS   0x23    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x24
 * value  | (signed int) float * 1000 [g]
 * limits | FGS-01: -8g ~ +8g
 *
 * \brief Accelerometer
 */
#define FRSKY_D_ACCX         0x24    // FBS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x25
 * value  | (signed int) float * 1000 [g]
 * limits | FGS-01: -8g ~ +8g
 *
 * \brief Accelerometer
 */
#define FRSKY_D_ACCY         0x25    // FBS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x26
 * value  | (signed int) float * 1000 [g]
 * limits | FGS-01: -8g ~ +8g
 *
 * \brief Accelerometer
 */
#define FRSKY_D_ACCZ         0x26    // FBS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x28
 * value  | (signed int) float * 10 [A]
 *
 * \brief FAS current
 */
#define FRSKY_D_CURRENT      0x28    // FAS40, FAS100

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x39
 * value  | (signed int) float * 10 [V]
 *
 * \brief Home made current sensors (aka VFAS)
 */
#define FRSKY_D_VFAS         0x39    // home made current sensors

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x3A (before "."), 0x3B (after ".")
 * value  | bugged formula (see below)
 * limits | FAS: 0.5 ~ 48.0V
 *
 * The formula is bugged, with a pitiful resolution of 0.19V for a 4 bytes value (which should have a 0.01V resolution).
 * There is oviously a mistake in the design. If you want to make a voltage sensor, use the \ref FRSKY_D_VFAS ID. It's
 * much more simple.
 * 
 * Encoding formula
 * ----------------
 * ~~~~~~~~~~~~~~~~~~~~~
 * float volts = 12.3;
 * volts = volts * 110 / 21;
 * uint16_t volts_b = (int) volts / 10;
 * uint16_t volts_a = (int) volts - (volts_b * 10);
 * FrskyD.sendData (FRSKY_D_VOLTAGE_B, volts_b);
 * FrskyD.sendData (FRSKY_D_VOLTAGE_A, volts_a);
 * ~~~~~~~~~~~~~~~~~~~~~
 * 
 * Decoding formula
 * ----------------
 * ~~~~~~~~~~~~~~~~~~~~~
 * (float) (volts_b * 10 + vols_a) * 21 / 110 
 * ~~~~~~~~~~~~~~~~~~~~~
 * 
 * \brief FAS voltage
 */
#define FRSKY_D_VOLTAGE_B    0x3A    // FAS40, FAS100

/**
 * see \ref FRSKY_D_VOLTAGE_B
 */
#define FRSKY_D_VOLTAGE_A    0x3B    // FAS40, FAS100

/**
 * Frsky D class
 */
class FrskyD {
  public:
    // objetcs
    FrskyD (int pinRx, int pinTx);
    SoftwareSerial *mySerial;         //!<SoftwareSerial object

    // methods
    bool   available ();

    float   calcFloat (int16_t bp, uint16_t ap);
    int16_t decodeInt   (byte *buffer);
    uint8_t decode1Int  (byte *buffer);
        
    float  decodeCellVolt   (byte *buffer);
    int    decodeCellVoltId (byte *buffer);
    String decodeGpsLat     (int16_t bp, uint16_t ap);
    String decodeGpsLong    (int16_t bp, uint16_t ap);
    
    uint16_t _fixForbiddenValues (uint16_t val);
    byte   read ();
    
    void   sendCellVolt (uint8_t id, float val);
    void   sendData  (uint8_t id, int16_t val);
    void   sendFloat (uint8_t idb, uint8_t ida, float val);
};

/**
 * \example FrskyD_sensor_demo/FrskyD_sensor_demo.ino
 */

/**
 * \example FrskyD_sniffer/FrskyD_sniffer.ino
 */
