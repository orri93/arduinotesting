#include <SPI.h>

#include <gosmax31865.h>
#include <arduinosensor.h>

#include <arduinotick.h>
#include <arduinoformat.h>
#include <arduinodisplay.h>

#define INTERVAL_MAX_31865     500

#define DELAY_SENSOR_SETUP_END 500

#define SERIAL_BAUD          19200

#define PIN_SPI_MAX_31865_CS    10

/* For PT100 set type to 1 and for PT1000 set type to 2 */
#define MAX_RTD_TYPE             1

// set to 2WIRE or 4WIRE as necessary
#define MAX_WIRES       RTD_3_WIRE

#define NO_DISPLAY

Tick timermax31865(INTERVAL_MAX_31865);

unsigned long tick;

const char* error31865;
const char* error6675;
uint8_t status31865, status6675, fault31865, length31865,  length6675;
double value31865, value6675;

MAX6675 max6675(
  PIN_MAX6675_CLK,
  PIN_MAX6675_CS,
  PIN_MAX6675_DO);

void setup() {
  fds::format::setup();
  fds::format::set(
    ':',
    TEXT_UNIT,
    sizeof(TEXT_UNIT),
    DISPLAY_LENGTH,
    DISPLAY_LENGTH - sizeof(TEXT_UNIT) - 2,
    FORMAT_PRECISION);

  fds::format::ids(SENSOR_IDS, sizeof(SENSOR_IDS));

  fds::display::u8g2.begin();

  gos::max31865::setup(
    PIN_SPI_MAX_31865_CS,
    MAX_RTD_TYPE,
    MAX_WIRES,
    SPI_CLOCK_DIV16,
    SPI_MODE3);

#ifdef SERIAL_BAUD
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {
    delay(10);
  }
  Serial.println("MAX 31865 and 6675 testing");
#endif

#ifdef DELAY_SENSOR_SETUP_END
  delay(DELAY_SENSOR_SETUP_END);
#endif
}

void loop() {
  tick = millis();

  bool display = false;

  if(timermax31865.is(tick)) {
    value31865 = gos::max31865::read(status31865, fault31865);
    if(status31865 == GOS_SENSOR_STATUS_OK) {
      Serial.print("Max 31865: ");
      Serial.println(value31865, 1);
      ::fds::format::number(value31865, 0);
    } else {
      Serial.print("Max 31865 failure: ");
      error31865 = gos::max31865::error(status31865, fault31865, length31865);
      Serial.println(error31865);
      ::fds::format::error(0, error31865, length31865);
    }
    display = true;
  }
  
  if(timermax6675.is(tick)) {
    value6675 = max6675.readCelsius();
    status6675 = gos::sensor::range::check(value6675);
    if(status6675 == GOS_SENSOR_STATUS_OK) {
      Serial.print("Max 6675: ");
      Serial.println(value6675, 1);
      ::fds::format::number(value6675, 1);
    } else {
      Serial.print("Max 6675 failure: ");
      error6675 = gos::sensor::error(status6675, length6675);
      Serial.println(error6675);
      ::fds::format::error(1, error6675, length6675);
    }
    display = true;
  }

  if(display) {
    fds::display::lines(2);
  }

  fds::display::loop();
}
