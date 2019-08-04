/*
 * Test binary size for C++ templates
 * 
 */

#include <Arduino.h>

#include <arduinotick.h>

#define USE_THE_TEMPLATE

#ifdef USE_THE_TEMPLATE
#include <gatlbuffer.h>
#include <gatlformat.h>
#include <gatldisplay.h>
#endif

#define SERIAL_BAUD            115200

#define MATRIX_D1                   2
#define MATRIX_D2                1024

#define INTERVAL                 1000

#define TEXT_ID_1 "A: "
#define TEXT_ID_2 "B: "
#define TEXT_UNIT " C"

template<typename T> T Matrix[MATRIX_D1][MATRIX_D2];

template<typename T> T function(const T& input) {
  const int d1 = 2;
  const int d2 = 1024;
  T matrix[d1][d2];  /* 2k */
  for(int x = 0; x < d1; x++) {
    for(int y = 0; y < d2; y++) {
      matrix[x][y] = input * x + y;
    }
  }
  T result = T();
  for(int y = 0; y < d2; y++) {
    for (int x = 0; x < d1; x++) {
      result += matrix[x][y];
    }
  }
  return result;
}

typedef double TheType;

TheType output = TheType(), input = TheType();

::gos::atl::buffer::Holder<uint8_t> first;
::gos::atl::buffer::Holder<uint8_t> second;

::gos::atl::buffer::Holder<uint8_t> id1(TEXT_ID_1, sizeof(TEXT_ID_1));
::gos::atl::buffer::Holder<uint8_t> id2(TEXT_ID_2, sizeof(TEXT_ID_2));
::gos::atl::buffer::Holder<uint8_t> unit(TEXT_UNIT, sizeof(TEXT_UNIT));

::gos::atl::display::Oled<DISPLAY_DEFAULT> oled;
::gos::atl::display::line::Two<DISPLAY_DEFAULT, uint8_t> two(oled);

::gos::atl::display::line::One<DISPLAY_DEFAULT, uint8_t> one(oled);

::gos::atl::format::option::Number number;
::gos::atl::format::option::Number numberone(5);

bool istwo = true;

Tick tdisplay(INTERVAL);
Tick tchange(INTERVAL * 10);

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("Template Test Starting");
  Serial.println();
  oled.U8g2->begin();
}

void loop() {
#ifdef USE_THE_TEMPLATE
  //output = function(input);

  unsigned long tick = millis();

  int r = random(0, 1024);
  double d = (double)(r) / 2.0;  

  if(tdisplay.is(tick)) {
    Serial.println("Display");
    if(tchange.is(tick)) {
      Serial.println("Switch");
      istwo = istwo ? false : true;
    }

    if(istwo) {
      ::gos::atl::format::real(first, d, number, &id1, &unit);
      ::gos::atl::format::integer(second, r, &id2, &unit);
      two.display(first, second);
    } else {
      ::gos::atl::format::real<double, uint8_t>(first, d, numberone, nullptr, &unit);
      one.display(first);
    }
  }

  if(istwo) {
    two.loop();
  } else {
    one.loop();
  }
#else
  output = input += 1.0;
#endif
  Serial.print("Input is ");
  Serial.print(input, 1);
  Serial.print(" and output is ");
  Serial.println(output, 1);
  input += PI;
}
