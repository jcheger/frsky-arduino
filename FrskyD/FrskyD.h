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
 * limits | ?
 * 
 * \brief GPS
 */
#define FRSKY_D_GPS_ALT_B    0x01    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x02
 * value  | unsigned int [°]
 * limits | -30~250°C
 */
#define FRSKY_D_TEMP1        0x02    // TEMS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x03
 * value  | unsigned int [rpm]
 * limits | 0 ~ 60000
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
 * \brief Barometric altitude
 */
#define FRSKY_D_ALT_B   0x10    // FVAS-01, FVAS-02

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x14, 0x14+8
 * value  | signed int [knots], unsigned in [dknots]
 * limits | ?
 * 
 * \brief GPS speed
 */
#define FRSKY_D_GPS_SPEED_B  0x11    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x12, 0x12+8
 * value  | ?
 * limits | ?
 *
 * \brief GPS longitude
 */
#define FRSKY_D_GPS_LONG_B   0x12    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x13, 0x13+8
 * value  | ?
 * limits | ?
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
 * \brief sensor: GPS course
 */
#define FRSKY_D_GPS_COURSE_B 0x14    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x15
 * value  | ?
 *
 */
#define FRSKY_D_GPS_DM       0x15    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x16
 * value  | ?
 *
 */
#define FRSKY_D_GPS_YEAR     0x16    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x17
 * value  | ?
 *
 */
#define FRSKY_D_GPS_HM       0x17    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x18
 * value  | ?
 *
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
 */
#define FRSKY_D_GPS_LONG_EW  0x22    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x23
 * value  | ?
 *
 */
#define FRSKY_D_GPS_LAT_NS   0x23    // GPS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x24
 * value  | signed float * 1000 [g]
 * limits | -8g ~ +8g
 *
 * \brief Accelerometer
 */
#define FRSKY_D_ACCX         0x24    // FBS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x25
 * value  | (signed int) float * 1000 [g]
 * limits | -8g ~ +8g
 *
 * \brief Accelerometer
 */
#define FRSKY_D_ACCY         0x25    // FBS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x26
 * value  | (signed int) float * 1000 [g]
 * limits | -8g ~ +8g
 *
 * \brief Accelerometer
 */
#define FRSKY_D_ACCZ         0x26    // FBS-01

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x28
 * value  | (signed int) float * 100
 * limits | ?
 *
 * \brief VFAS current
 */
#define FRSKY_D_CURRENT      0x28    // FAS40, FAS100

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x49
 * value  | ?
 * limits | ?
 *
 * \brief Home made current sensors
 */
#define FRSKY_D_VFAS         0x39    // home made current sensors

/**
 * info   | comment
 * ----   | -------
 * ID(s)  | 0x3A (before "."), 0x3B (after ".")
 * value  | ?
 * limits | ?
 *
 * \brief VFAS voltage
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

    float  decodeAcc        (byte *buffer);
    float  decodeAlt        (byte *buffer);
    float  decodeCellVolt   (byte *buffer);
    int    decodeCellVoltId (byte *buffer);
    float  decodeGpsAlt     (byte *buffer);
    float  decodeGpsCourse  (byte *buffer);
    int    decodeGpsDay     (byte *buffer);
    int    decodeGpsHour    (byte *buffer);
    String decodeGpsLat     (byte *buffer);
    int    decodeGpsLatNS   (byte *buffer);
    String decodeGpsLong    (byte *buffer);
    int    decodeGpsLongEW  (byte *buffer);
    int    decodeGpsMin     (byte *buffer);
    int    decodeGpsMonth   (byte *buffer);
    int    decodeGpsSec     (byte *buffer);
    float  decodeGpsSpeed   (byte *buffer);
    int    decodeGpsYear    (byte *buffer);
    int    decodeFuel       (byte *buffer);
    int    decodeRpm        (byte *buffer);
    int    decodeTemp       (byte *buffer);
    
    uint16_t _fixForbiddenValues (uint16_t val);
    byte  read ();
    
    void  sendAcc          (float valX, float valY, float valZ);
    void  sendAlt          (float val);
    void  sendCellVolt     (uint8_t cellID, float val);
    void  sendData         (uint8_t id, uint16_t val);
    void  sendFuel         (uint16_t val);
    void  sendGpsAlt       (float val);
    void  sendGpsCourse    (float val);
    void  sendGpsLat       (uint8_t d, uint8_t m, uint8_t mm1, uint8_t mm2);
    void  sendGpsLong      (uint8_t d, uint8_t m, uint8_t mm1, uint8_t mm2);
    void  sendGpsDayMonth  (uint8_t day,  uint8_t month);
    void  sendGpsHourMin   (uint8_t hour, uint8_t min);
    void  sendGpsSec       (uint16_t val);
    void  sendGpsSpeed     (float val);
    void  sendGpsSpeedKmph (float val);
    void  sendGpsSpeedMph  (float val);
    void  sendGpsYear      (uint16_t val);
    void  sendRpm          (uint16_t val);
    void  sendTemp1        (int16_t val);
    void  sendTemp2        (int16_t val);
};

/**
 * \example FrskyD_sensor_demo/FrskyD_sensor_demo.ino
 */

/**
 * \example FrskyD_sniffer/FrskyD_sniffer.ino
 */
