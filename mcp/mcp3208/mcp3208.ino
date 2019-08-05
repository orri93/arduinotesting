/*
 * MCP 3208 Single channel
 * 
 * Wiring
 * 
 * MCP 3208
 * 
 *  CH0  1  u 16   Pin 16  VDD
 *  CH1  2    15   Pin 15  VREF
 *  CH2  3    14   Pin 14  AGND
 *  CH3  4    13   Pin 13  CLK       CLK   Pin 13
 *  CH4  5    12   Pin 12  DOOUT     MISO  Pin 12
 *  CH5  6    11   Pin 11  DIN       MOSI  Pin 11
 *  CH6  7    10   Pin 10  CS/SHDN   D10 Pin 10
 *  CH7  8     9   Pin  9  DGNS
 * 
 */

#include <SPI.h>

#include <arduinotick.h>

#include <gosmcp3208.h>

#define SERIAL_BAUD   115200

#define PIN_SC            10

::gos::Mcp3208 mcp3208(PIN_SC);

uint16_t value;
uint16_t values[8];

Tick readsingle(2000);
Tick readmultiple(5000);

unsigned long tick;

void setup() {
  SPI.begin();
  mcp3208.begin();
  Serial.begin(SERIAL_BAUD);
  Serial.println("MCP 3208 Reading single channel example");
}

void loop() {
  tick = millis();

  if(readsingle.is(tick)) {
    value = mcp3208.read(0);
    Serial.print("Channel 0: ");
    Serial.println(value, DEC);
    value = mcp3208.read(7);
    Serial.print("Channel 7: ");
    Serial.println(value, DEC);
  }

  if(readmultiple.is(tick)) {
    mcp3208.read(values, 0, 7);
    for(uint8_t i = 0; i < 8; i++) {
      Serial.print("Multichannel no. ");
      Serial.print(i, DEC);
      Serial.print(": ");
      Serial.println(values[i]);
    }
  }
}
