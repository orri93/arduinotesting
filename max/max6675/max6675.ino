/*
 * SPI
 *                      UNO
 * 1 SLK <  SCK      Pin 13
 * 2 CS  <  D10      Pin 10
 * 3 DO  > MISO      Pin 12
 * 4 VCC    VCC      3.3/5V
 * 5 GND    GND         GND
 */ 

/* Build Options */
//#define NO_DISPLAY

#include <SPI.h>

#include <gosmax6675.h>
#include <arduinosensor.h>
#include <arduinotick.h>
#include <gos/atl/median.h>

#ifndef NO_DISPLAY
#include <arduinoformat.h>
#include <arduinodisplay.h>
#endif

#define PIN_MAX6675_CS          10

#define INTERVAL_MAX_6675     2000

#define DELAY_SENSOR_SETUP_END 500

#define SERIAL_BAUD          19200

#ifndef NO_DISPLAY
#define DISPLAY_LENGTH          11
#define FORMAT_PRECISION         1
#define TEXT_UNIT             "C"
#endif

Tick timermax6675(INTERVAL_MAX_6675);

::gos::atl::Median<double> median;

::gos::Max6675 max6675(PIN_MAX6675_CS);

unsigned long tick;

const char* error6675;
uint8_t status6675, length6675;
double value6675;

void setup() {
#ifndef NO_DISPLAY
  fds::format::setup();
  fds::format::set(
    '\0',
    TEXT_UNIT,
    sizeof(TEXT_UNIT),
    DISPLAY_LENGTH,
    DISPLAY_LENGTH - sizeof(TEXT_UNIT) - 2,
    FORMAT_PRECISION);
  fds::display::u8g2.begin();
#endif

  SPI.begin();

  max6675.initialize();

#ifdef SERIAL_BAUD
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {
    delay(10);
  }
  Serial.println("MAX 6675 testing");
#endif

#ifdef DELAY_SENSOR_SETUP_END
  delay(DELAY_SENSOR_SETUP_END);
#endif
}

void loop() {
  tick = millis();
  
  if(timermax6675.is(tick)) {
    error6675 = nullptr;
    if(max6675.read(value6675)) {
      if(::gos::sensor::range::check(value6675) == GOS_SENSOR_STATUS_OK) {
        median.add(value6675);
        } else {
        error6675 = ::gos::sensor::error(length6675);
      }
    } else {
      error6675 = max6675.error(length6675);
    }
    if(error6675 == nullptr) {
      Serial.println(value6675, 1);
#ifndef NO_DISPLAY
      ::fds::format::number(value6675, 0, 6);
#endif
    } else {
      Serial.println(error6675);
#ifndef NO_DISPLAY
      ::fds::format::error(error6675, length6675);
#endif
    }
#ifndef NO_DISPLAY
    fds::display::lines(1);
#endif
  }

#ifndef NO_DISPLAY
  fds::display::loop();
#endif
}
