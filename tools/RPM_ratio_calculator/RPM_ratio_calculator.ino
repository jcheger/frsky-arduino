/*
 * RPM ratio calculator
 * Use two RPM sensors and calculate the ratio.
 *
 * Those values are shown once per second
 * - frequency (Hz)
 * - RPM (Hz * 60)
 * - ratio between the bigger on divided by the lower one
 * 
 * Requirements
 * ------------
 * - PinChangeInt library - https://code.google.com/p/arduino-pinchangeint/
 *                          https://arduino-pinchangeint.googlecode.com/files/pinchangeint-v2.19beta.zip
 * 
 * - Streaming library - http://arduiniana.org/libraries/streaming/
 *                       http://arduiniana.org/Streaming/Streaming5.zip
 *
 * - LiquiCrystal_I2C library - http://www.dfrobot.com/wiki/index.php?title=I2C/TWI_LCD1602_Module_(SKU:_DFR0063)
 *                              http://www.dfrobot.com/image/data/DFR0154/LiquidCrystal_I2Cv1-1.rar
 * 
 * - a LCD with I2C (search on eBay for "LCD I2C Arduino")
 *
 * 
 * origin: https://github.com/jcheger/frsky-arduino
 * author: Jean-Christophe Heger <jcheger@ordinoscope.net>
 */
 
#include <PinChangeInt.h>
#include <Streaming.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define NO_PORTB_PINCHANGES  // disable PinChangeInt on pins 8-13
#define NO_PORTC_PINCHANGES  // disable PinChangeInt on pins A0-A7
//#define NO_PORTD_PINCHANGES  // disable PinChangeInt on pins 0-7

#define PIN_RPM1 2
#define PIN_RPM2 3

unsigned long rpm1_counter = 0;
unsigned long rpm2_counter = 0;

void rpm1ISR () {
  rpm1_counter++;
};

void rpm2ISR () {
  rpm2_counter++;
}

void setup() {
  lcd.init ();
  lcd.backlight ();
  lcd.setCursor (3, 0);
  lcd.print ("RPM  RATIO");
  lcd.setCursor (3, 1);
  lcd.print ("calculator");
  
  Serial.begin(115200);
  Serial.println("RPM ratio calculator");

  delay (2000);
  lcd.clear ();
  
  pinMode (PIN_RPM1, INPUT);
  PCintPort::attachInterrupt (PIN_RPM1, &rpm1ISR, FALLING);

  pinMode (PIN_RPM2, INPUT);
  PCintPort::attachInterrupt (PIN_RPM2, &rpm2ISR, FALLING);
}

void loop() {
  unsigned long rpm1;
  unsigned long rpm2;
  float ratio;
  
  rpm1_counter = 0;
  rpm2_counter = 0;
  delay (1000);
  rpm1 = rpm1_counter;
  rpm2 = rpm2_counter;
  if (rpm1 == 0 || rpm2 == 0) {
    ratio = 0;
  } else {
    ratio = (rpm1 > rpm2) ? rpm1 / rpm2 : rpm2 / rpm1;
  }
  
  lcd.setCursor (0, 0);
  lcd << rpm1 << "     ";
  lcd.setCursor (5, 0);
  lcd << rpm1 * 60 << "          ";
  lcd.setCursor (0, 1);
  lcd << rpm2 << "     ";
  lcd.setCursor (5, 1);
  lcd << rpm2 * 60 << "          ";
  lcd.setCursor (12, 1);
  lcd << ratio;
  
  Serial << "RPM1: " << rpm1 * 60 << " (" << rpm1 << " Hz) - ";
  Serial << "RPM2: " << rpm2 * 60 << " (" << rpm2 << " Hz) - ";
  Serial << "ratio: " << ratio << endl;
}

