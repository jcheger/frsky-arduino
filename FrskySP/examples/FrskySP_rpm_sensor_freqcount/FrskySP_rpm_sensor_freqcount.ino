/*
 * RPM sensor for Frsky Smart Port protocol.
 *
 * Requirements
 * ------------
 * - FrskySP library: https://github.com/jcheger/frsky-arduino
 * - FreqCount library: https://www.pjrc.com/teensy/td_libs_FreqCount.html
 * 
 * The RPM pin is 5 and cannot be changed (defined by the FreqCount library). The RPMs are refreshed once per second.
 *
 * See the the images in the example folder to see the pinout.
 *
 * EagleTree sensors
 * -----------------
 * Be very cautious, the pinout is not standard ! Any mistake will make the optical sensor to blow up instantly !
 * - red: GND !!!
 * - black: VCC for optical sensor IR LED. Must have a 100 Ω @ 5V. No more, no less.
 * - white: Signal. Must have a pull-up resistor (see below)
 *
 * Tests with:
 * - EagleTree Brushless RPM Sensor V2 - pull-up: 1 kΩ
 * - EagleTree Optical RPM Sensor - pull-up: 10 kΩ
 * - EagleTree Hall RPM Sensor - pull-up: 1 kΩ or 10 kΩ
 * 
 * 
 * origin: https://github.com/jcheger/frsky-arduino
 * author: Jean-Christophe Heger <jcheger@ordinoscope.net>
 */

#include <FrskySP.h>
#include <SoftwareSerial.h>
#include <FreqCount.h>

#define DEBUG 0

FrskySP FrskySP (10, 11);

float rpm_ratio = 7;

void setup () {
  #if DEBUG
  Serial.begin (115200);
  Serial.println ("FrskySP rpm sensor freqcount");
  #endif
  FreqCount.begin(1000);
}

void loop () {
  static unsigned int i = 0;

  static unsigned long rpm_freq = 0;
  uint16_t             rpm_send = 0;

  while (FrskySP.available ()) {

    if (FrskySP.read () == 0x7E) {

      while (!FrskySP.available ());  // wait for the next byte

      switch (FrskySP.read ()) {

        case 0xE4:
          rpm_freq = FreqCount.read();
          // The brushless sensor triggers about 1 pulse per second when no pulse is detected. Erase them.
          rpm_send = (rpm_freq > 5) ? (float) rpm_freq * rpm_ratio : 0;
          #if DEBUG
          Serial.print ("rpm_freq: ");
          Serial.print (rpm_freq);
          Serial.print (", rpm_send: ");
          Serial.println (rpm_send);
          #endif
          FrskySP.sendData (FRSKY_SP_RPM, rpm_send);
          break;
      }
    }
  }
}

