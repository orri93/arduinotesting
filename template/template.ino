/*
 * Test binary size for C++ templates
 * 
 */

#include <Arduino.h>

//#define USE_THE_TEMPLATE

#define SERIAL_BAUD            115200

#define MATRIX_D1                   2
#define MATRIX_D2                1024

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

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("Template Test Starting");
  Serial.println();
}

void loop() {
#ifdef USE_THE_TEMPLATE
  output = function(input);
  Matrix<double>[0][0] = output;
#else
  output = input += 1.0;
#endif
  Serial.print("Input is ");
  Serial.print(input, 1);
  Serial.print(" and output is ");
  Serial.println(output, 1);
  input += PI;
}
