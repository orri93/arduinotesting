#include <SPI.h>

#include <gosmax31865.h>
#include <arduinosensor.h>

#include <arduinotick.h>
#include <arduinoformat.h>

#include "pinuno.h"

#define INTERVAL_MAX_6675     2000

#define DELAY_SENSOR_SETUP_END 500

#define SERIAL_BAUD          19200

#define DISPLAY_LENGTH          11
#define FORMAT_PRECISION         1
#define TEXT_UNIT             " C"

Tick timermax6675(INTERVAL_MAX_6675);

SPISettings spisettings6675(2000000, MSBFIRST, SPI_MODE1); 

unsigned long tick;

const char* error6675;
uint8_t status6675, length6675, first6675, second6675;
uint16_t raw6675;
double value6675;

void setup() {
  fds::format::setup();
  fds::format::set(
    '\0',
    TEXT_UNIT,
    sizeof(TEXT_UNIT),
    DISPLAY_LENGTH,
    DISPLAY_LENGTH - sizeof(TEXT_UNIT) - 2,
    FORMAT_PRECISION);

  fds::display::u8g2.begin();
  
  pinMode(PIN_MAX6675_CS, OUTPUT);
  digitalWrite(PIN_MAX6675_CS, HIGH);

  SPI.begin();

//  gos::max31865::setup(
//    PIN_SPI_MAX_31865_CS,
//    MAX_RTD_TYPE,
//    MAX_WIRES,
//    SPI_CLOCK_DIV16,
//    SPI_MODE3);

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
    SPI.beginTransaction(spisettings6675);
    digitalWrite(PIN_MAX6675_CS, LOW);
    // reading only, so data sent does not matter
    first6675 = SPI.transfer(0x00);
    second6675 = SPI.transfer(0x00);
    digitalWrite(PIN_MAX6675_CS, HIGH);
    SPI.endTransaction();
    raw6675 = (first6675 << 8) | second6675;
    Serial.print(raw6675);
    Serial.print(" (");
    Serial.print(first6675);
    Serial.print(",");
    Serial.print(second6675);
    Serial.println(")");
    fds::format::integer(raw6675, 0);
    /*
    value6675 = max6675.readCelsius();
    status6675 = gos::sensor::range::check(value6675);
    if(status6675 == GOS_SENSOR_STATUS_OK) {
      Serial.print("Max 6675: ");
      Serial.println(value6675, 1);
      ::fds::format::number(value6675, 0);
    } else {
      Serial.print("Max 6675 failure: ");
      error6675 = gos::sensor::error(status6675, length6675);
      Serial.println(error6675);
      ::fds::format::error(0, error6675, length6675);
    }
    */
    fds::display::lines(1);
  }

  fds::display::loop();
}
