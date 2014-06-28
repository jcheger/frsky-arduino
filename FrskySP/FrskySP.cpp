/** \mainpage
 * Arduino library for Frsky Smart Port protocol.
 * 
 * This library is not designed to decode data on the transmitter, but on the receiver side. OpenTX makes the rest of
 *  the job on the transmitter.
 *
 * This development is completely independent of Frsky or OpenTX.
 * 
 * What to know about Smart Port
 * -----------------------------
 * * use an inverted serial communication at 57600bds muxed on one port
 * * the receiver polls 27 physical sensors periodically
 * * compare to D, the SP protocol seems much faster, but maybe not - it's easier to use for data encoding, and is
 *   nicer to have a hub than a bus, but the polling of 27 IDs take much time - once a sensor is detected, the receiver
 *   may accelerate the polling, typically for the GPS.
 * 
 * Slowness considerations
 * -----------------------
 * Some other projects did use some home made protocol decoder. This library does use SoftwareSerial, interrupted based,
 *  included in Arduino 1.0+. Until now, no slowness issues were observed. Although, you must be careful
 *  around those issues:
 * * only one answer per poll cycle (the [FrskySP_sensor_demo.ino](\ref FrskySP_sensor_demo/FrskySP_sensor_demo.ino) example shows how to
 *  handle multiple answers for one physical ID)
 * * the answer must be fast - some sensor polling can take much time (i.e. analog reading)
 * * only one sensor per physical ID (i.e. GPS a normal precision altimeter share the physical ID 3)
 * 
 * Connection draft
 * ----------------
 * \image html Smart_Port_bb.png
 * 
 * * pull down resistor on TX line (100k)
 * * diode between TX and RX (i.e. 1N4108)
 * 
 * On this circuit, RX will hang after serial begin. There is a workaround that inverts the TX pinMode to INPUT and back
 *  to OUTPUT.
 *
 * \version devel
 * \author Jean-Christophe Heger
 * \see https://github.com/jcheger/frsky-arduino/ - source of this library
 * \see http://www.frsky-rc.com/
 * \see http://www.open-tx.org/
 * \copyright 2014 - Jean-Christophe Heger - Released under the LGPL 3.0 license.
 * \todo write an example for asynchronous polling
 * \todo write an example to simulate an X8R receiver
 * \ChangeLog 2014-06-27 - public devel release
 */
 
#include "Arduino.h"
#include "FrskySP.h"
#include "SoftwareSerial.h"

/**
 * Smart Port protocol uses 8 bytes packets.
 * 
 * Packet format (byte): tiivvvvc
 * - t: type (1 byte)
 * - i: sensor ID (2 bytes)
 * - v: value (4 bytes - int32)
 * - c: crc
 * 
 * The uint64 presentation is much easier to use for data shifting.
 */
union FrskySP::packet {
    //! byte[8] presentation
    uint8_t byte[8];
    //! uint64 presentation
    uint64_t uint64;
};

/**
 * Open a SoftwareSerial connection
 * \param pinRx RX pin
 * \param pinTx TX pin
 * \brief Class constructor
 * \warning after opening the ports with SoftwareSerial, and because of the mux between TX and RX, RX will hang.
 *   The workaround is to revert the TX port as INPUT, and put it back again as OUTPUT at the first available()
 *   call.
 */
FrskySP::FrskySP (int pinRx, int pinTx) {
    this->_pinRx = pinRx;
    this->_pinTx = pinTx;
    this->mySerial = new SoftwareSerial (pinRx, pinTx, true);
    this->mySerial->begin (57600);
    pinMode (pinTx, INPUT); // RX freeze workaround
}

/**
 * Check if a byte is available on Smart Port
 * 
 * Reverts the TX pin to OUTPUT mode after the first available byte
 * \brief SoftwareSerial.available() passthrough
 */
bool FrskySP::available () {
    static bool mode = 1;                   // at first call, mode is INPUT
    bool r = this->mySerial->available ();
    if (mode && r) {                        // RX freeze workaround
        pinMode (this->_pinTx, OUTPUT);     
        mode = 0;                           // mode is OUPUT from now
    }
    return r;
}

/**
 * \brief Calculate the CRC of a packet
 * \see https://github.com/opentx/opentx/blob/next/radio/src/telemetry/frsky_sport.cpp
 * \param packet packet pointer (in byte[8] presentation)
 */
uint8_t FrskySP::CRC (uint8_t *packet) {
    short crc = 0;
    for (int i=0; i<8; i++) {
        crc += packet[i]; //0-1FF
        crc += crc >> 8;  //0-100
        crc &= 0x00ff;
        crc += crc >> 8;  //0-0FF
        crc &= 0x00ff;
    }
    return ~crc;
}

/**
 * \brief Check the CRC of a packet
 * \see https://github.com/opentx/opentx/blob/next/radio/src/telemetry/frsky_sport.cpp
 * \param packet packet pointer
 */
bool FrskySP::CRCcheck (uint8_t *packet) {
    short crc = 0;
    for (int i=1; i<8; i++) {
        crc += packet[i]; //0-1FF
        crc += crc >> 8;  //0-100
        crc &= 0x00ff;
        crc += crc >> 8;  //0-0FF
        crc &= 0x00ff;
    }
    return (crc == 0x00ff);
}

/**
 * \brief Same as lipoCell(uint8_t id, float val1, float val2), but with only one cell.
 * \param id cell ID (0~11)
 * \param val cell voltage
 * \return formated data for cell voltage (1 cell)
 */
uint32_t FrskySP::lipoCell (uint8_t id, float val) {
    val *= 500;
    return (uint32_t) val << 8 | id;
}

/**
 * A cell packet is formated this way:
 * content    | length
 * ---------- | ------
 * volt[id+1] | 12 bits
 * volt[id]   | 12-bits
 * celltotal  | 4 bits
 * cellid     | 4 bits
 * 
 * The cell total is not used on OpenTX. The cell count is modified by the highest id,
 * but 12 at a maximum.
 *
 * \brief Lipo voltage data format for 2 cells
 * \param id cell ID (0~11)
 * \param val1 cell voltage (for cell ID)
 * \param val2 cell voltage (for cell ID+1)
 * \return formated data for cell voltage (2 cells)
 */
uint32_t FrskySP::lipoCell (uint8_t id, float val1, float val2) {
    val1 *= 500;
    val2 *= 500;
    return ((uint32_t) val2 & 0x0fff) << 20 | ((uint32_t) val1 & 0x0fff) << 8 | 0;
}

/**
 * \brief SoftwareSerial.read() passthrough
 */
byte FrskySP::read () {
    return this->mySerial->read ();
}

/**
 * Sensors logical IDs and value formats are documented in FrskySP.h.
 * 
 * \brief Simplified version of sendData(), while the type is only 0x10 at now.
 * \param id sensor ID
 * \param val value
 */
void FrskySP::sendData (uint16_t id, int32_t val) {
    this->sendData (0x10, id, (uint32_t) val);
}

/**
 * Sensors logical IDs and value formats are documented in FrskySP.h.
 * 
 * Packet format:
 * content   | length | remark
 * --------- | ------ | ------
 * type      | 8 bit  | always 0x10 at now
 * sensor ID | 16 bit | sensor's logical ID (see FrskySP.h for values)
 * data      | 32 bit | preformated data
 * crc       | 8 bit  | calculated by CRC()
 * 
 * \brief Prepare the packet and send it.
 * \param type value type
 * \param id sensor ID
 * \param val value
 */
void FrskySP::sendData (uint8_t type, uint16_t id, int32_t val) {
    int i = 0;
    union packet packet;

    packet.uint64  = (uint64_t) type | (uint64_t) id << 8 | (int64_t) val << 24;
    packet.byte[7] = this->CRC (packet.byte);

    for (i=0; i<8; i++) this->mySerial->write (packet.byte[i]);
}
