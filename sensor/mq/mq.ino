/*
 * Read MQ-2 and MQ-3 with MCP 3208
 * 
 * Wiring
 * 
 * MCP 3208
 * 
 *       CH0  1  u 16   Pin 16  VDD
 * MQ-2  CH1  2    15   Pin 15  VREF
 * MQ-2  CH2  3    14   Pin 14  AGND
 *       CH3  4    13   Pin 13  CLK       CLK   Pin 13
 * MQ-3  CH4  5    12   Pin 12  DOOUT     MISO  Pin 12
 * MQ-3  CH5  6    11   Pin 11  DIN       MOSI  Pin 11
 *       CH6  7    10   Pin 10  CS/SHDN   D10 Pin 10
 * 5V    CH7  8     9   Pin  9  DGNS
 * 
 */

#include <SPI.h>

#include <arduinotick.h>

#include <gosmcp3208.h>

#include <gatlmedian.h>

#define SERIAL_BAUD      115200

#define PIN_SC               10

#define MCP_3208_CH_REF_GND   0
#define MCP_3208_CH_MQ2A      1
#define MCP_3208_CH_MQ2B      2
#define MCP_3208_CH_NOISE_A   3
#define MCP_3208_CH_MQ3A      4
#define MCP_3208_CH_MQ3B      5
#define MCP_3208_CH_NOISE_B   6
#define MCP_3208_CH_REF_VDD   7

#define MQ_TWO_CHANNEL_AVRAGE(x,y) (static_cast<uint32_t>(values[x]) + static_cast<uint32_t>(values[y])) / 2

#define MQ_UNDEFINED 0xffffffff
#define MQ_MEDIAN_COUNT      20
#define MQ_INTERVAL_READ     25

namespace gatl = ::gos::atl;

::gos::Mcp3208 mcp3208(PIN_SC);

uint16_t values[GOS_MCP_3208_CHANNEL_COUNT];

uint32_t mq2, mq3, noise;

Tick timetoread(MQ_INTERVAL_READ);
Tick timetodisplay(MQ_INTERVAL_READ * MQ_MEDIAN_COUNT);

gatl::statistics::Set<uint32_t, uint8_t> mq2set(MQ_UNDEFINED, MQ_MEDIAN_COUNT);
gatl::statistics::Set<uint32_t, uint8_t> mq3set(MQ_UNDEFINED, MQ_MEDIAN_COUNT);

gatl::statistics::Median<uint32_t, uint8_t> mq2median(mq2set);
gatl::statistics::Median<uint32_t, uint8_t> mq3median(mq3set);

uint32_t mmq2, mmq3;

unsigned long tick;

void setup() {
  SPI.begin();
  mcp3208.begin();
  Serial.begin(SERIAL_BAUD);
  Serial.println("MQ-2 and MQ-3 through MCP 3208");
}

void loop() {
  tick = millis();

  if(timetoread.is(tick)) {
    mcp3208.read(values, 0, GOS_MCP_3208_CHANNEL_MAX);
    for(uint8_t i = 0; i < GOS_MCP_3208_CHANNEL_COUNT; i++) {
      mq2 = MQ_TWO_CHANNEL_AVRAGE(MCP_3208_CH_MQ2A, MCP_3208_CH_MQ2B);
      mq3 = MQ_TWO_CHANNEL_AVRAGE(MCP_3208_CH_MQ3A, MCP_3208_CH_MQ3B);
      mq2set.add(mq2);
      mq3set.add(mq3);
      //noise = MQ_TWO_CHANNEL_AVRAGE(MCP_3208_CH_NOISE_A, MCP_3208_CH_NOISE_B);
    }
  }

  if(timetodisplay.is(tick)) {
    mmq2 = mq2median.get();
    mmq3 = mq3median.get();
    Serial.print(mmq2, DEC);
    Serial.print(",");
    Serial.println(mmq3, DEC);
    //Serial.print("MQ-2: ");
    //Serial.println(mmq2, DEC);
    //Serial.print("MQ-3: ");
    //Serial.println(mmq3, DEC);
  }
}
