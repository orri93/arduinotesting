/* MAX 485 tester using Modbus */

#include <ModbusSlave.h>

#define OPTION_USE_OLED

#include <arduinotick.h>

#ifdef OPTION_USE_OLED
#include <gatldisplay.h>
#include <gatlformat.h>
#endif

#define PIN_RS485_MODBUS_TE            8

//#define PIN_COIL_OUTPUT_1             14
//#define PIN_COIL_OUTPUT_2             15

//#define PIN_DISCRETE_INPUT_1           7
//#define PIN_DISCRETE_INPUT_2           8

//#define PIN_INPUT_REGISTRY_1          A0
//#define PIN_INPUT_REGISTRY_2          A1

//#define PIN_HOLDING_REGISTRY_OUTPUT_1  5
//#define PIN_HOLDING_REGISTRY_OUTPUT_2  6

#define MODBUS_SLAVE_ID               11
#define RS485_BAUD                  9600

#define DELAY_SETUP_COMPLETED         10

#define MAX_RAND                   0x400

#ifdef OPTION_USE_OLED
#define TEXT_ID_A "A: "
#define TEXT_ID_B "B: "
::gos::atl::format::option::Number option;
::gos::atl::buffer::Holder<> buffer1(11);
::gos::atl::buffer::Holder<> buffer2(11);
::gos::atl::buffer::Holder<> ida(TEXT_ID_A, sizeof(TEXT_ID_A));
::gos::atl::buffer::Holder<> idb(TEXT_ID_B, sizeof(TEXT_ID_B));
::gos::atl::display::Oled<> oled;
::gos::atl::display::line::Two<> twoline(oled);
enum class Display { Coil, Registry };
Display display = Display::Registry;
#endif

enum Coil {
  Coil1 = 0x0000,
  Coil2 = 0x0001,
};

enum DiscreteInput {
  DiscreteInput1 = 0x0000,
  DiscreteInput2 = 0x0001
};

enum HoldingRegistry {
  HoldingRegistry1 = 0x0000,
  HoldingRegistry2 = 0x0001
};

enum InputRegistry {
  InputRegistry1 = 0x0000,
  InputRegistry2 = 0x0001
};

bool coil1, coil2;
uint16_t address, holdingregistry1, holdingregistry2;

Modbus slave(MODBUS_SLAVE_ID, PIN_RS485_MODBUS_TE);

#ifdef OPTION_USE_OLED
namespace show {
void update();
}
#endif

/* 0x01 Read Coils */
uint8_t read_coils(uint8_t fc, uint16_t startaddress, uint16_t length) {
  for (address = startaddress; address < startaddress + length; address++) {
    switch (address) {
    case Coil1:
      slave.writeCoilToBuffer(address, coil1);
      break;
    case Coil2:
      slave.writeCoilToBuffer(address, coil2);
      break;
    default:
      return STATUS_ILLEGAL_DATA_ADDRESS;
    }
  }
  return STATUS_OK;
}

/* 0x02 Read Discrete Inputs */
uint8_t read_discrete_inputs(uint8_t fc, uint16_t startaddress, uint16_t length) {
  for (address = startaddress; address < startaddress + length; address++) {
    switch (address) {
    case DiscreteInput1:
#ifdef PIN_DISCRETE_INPUT_1
      slave.writeDiscreteInputToBuffer(address, digitalRead(PIN_DISCRETE_INPUT_1));
#else
      slave.writeDiscreteInputToBuffer(address, random(MAX_RAND));
#endif
      break;
    case DiscreteInput2:
#ifdef PIN_DISCRETE_INPUT_2
      slave.writeDiscreteInputToBuffer(address, digitalRead(PIN_DISCRETE_INPUT_2));
#else
      slave.writeDiscreteInputToBuffer(address, random(MAX_RAND));
#endif
      break;
    default:
      return STATUS_ILLEGAL_DATA_ADDRESS;
    }
  }
  return STATUS_OK;
}

/* 0x03 Read Multiple Holding Registers */
uint8_t read_holding_registers(uint8_t fc, uint16_t startaddress, uint16_t length) {
  for (address = startaddress; address < startaddress + length; address++) {
    switch (address) {
    case HoldingRegistry1:
      slave.writeRegisterToBuffer(address, holdingregistry1);
      break;
    case HoldingRegistry2:
      slave.writeRegisterToBuffer(address, holdingregistry2);
      break;
    default:
      return STATUS_ILLEGAL_DATA_ADDRESS;
    }
  }
  return STATUS_OK;
}

/* 0x04 Read Input Registers */
uint8_t read_input_registers(uint8_t fc, uint16_t startaddress, uint16_t length) {
  for (address = startaddress; address < startaddress + length; address++) {
    switch (address) {
    case InputRegistry1:
#ifdef PIN_DISCRETE_INPUT_1
      slave.writeRegisterToBuffer(address, analogRead(PIN_DISCRETE_INPUT_1));
#else
      slave.writeRegisterToBuffer(address, random(MAX_RAND));
#endif
      break;
    case InputRegistry2:
#ifdef PIN_DISCRETE_INPUT_2
      slave.writeRegisterToBuffer(address, analogRead(PIN_DISCRETE_INPUT_2));
#else
      slave.writeRegisterToBuffer(address, random(MAX_RAND));
#endif
      break;
    default:
      return STATUS_ILLEGAL_DATA_ADDRESS;
    }
  }
  return STATUS_OK;
}

/* 0x05 Write Single Coil and 0x0f Write Multiple Coils */
uint8_t write_coils(uint8_t fc, uint16_t startaddress, uint16_t length) {
  for (address = startaddress; address < startaddress + length; address++) {
    switch (address) {
    case Coil1:
      coil1 = slave.readCoilFromBuffer(address);
#ifdef OPTION_USE_OLED
      show::update();
#endif
#ifdef PIN_COIL_OUTPUT_1
      digitalWrite(PIN_COIL_OUTPUT_1, coil1 ? HIGH : LOW);
#endif
      break;
    case Coil2:
      coil2 = slave.readCoilFromBuffer(address);
#ifdef OPTION_USE_OLED
      show::update();
#endif
#ifdef PIN_COIL_OUTPUT_2
      digitalWrite(PIN_COIL_OUTPUT_2, coil2 ? HIGH : LOW);
#endif
      break;
    default:
      return STATUS_ILLEGAL_DATA_ADDRESS;
    }
  }
  return STATUS_OK;
}

/* 0x06 Write Single Holding Register and 0x10 Write Multiple Holding Registers */
uint8_t write_holding_registers(uint8_t fc, uint16_t startaddress, uint16_t length) {
  for (address = startaddress; address < startaddress + length; address++) {
    switch (address) {
    case HoldingRegistry1:
      holdingregistry1 = slave.readRegisterFromBuffer(address);
#ifdef OPTION_USE_OLED
      show::update();
#endif
#ifdef PIN_HOLDING_REGISTRY_OUTPUT_1
      if (holdingregistry1 >= 0 && holdingregistry1 <= 255) {
        analogWrite(PIN_HOLDING_REGISTRY_OUTPUT_1, holdingregistry1);
      }
      else {
        return STATUS_ILLEGAL_DATA_VALUE;
      }
#endif
      break;
    case HoldingRegistry2:
      holdingregistry2 = slave.readRegisterFromBuffer(address);
#ifdef OPTION_USE_OLED
      show::update();
#endif
#ifdef PIN_HOLDING_REGISTRY_OUTPUT_2
      if (holdingregistry2 >= 0 && holdingregistry2 <= 255) {
        analogWrite(PIN_HOLDING_REGISTRY_OUTPUT_2, holdingregistry2);
      }
      else {
        return STATUS_ILLEGAL_DATA_VALUE;
      }
#endif
      break;
    default:
      return STATUS_ILLEGAL_DATA_ADDRESS;
    }
  }
  return STATUS_OK;
}

void setup() {
#ifdef OPTION_USE_OLED
  oled.U8g2->begin();
  show::update();
#endif
#ifdef PIN_COIL_OUTPUT_1
  pinMode(PIN_COIL_OUTPUT_1, OUTPUT);
#endif
#ifdef PIN_COIL_OUTPUT_2
  pinMode(PIN_COIL_OUTPUT_2, OUTPUT);
#endif
#ifdef PIN_DISCRETE_INPUT_1
  pinMode(PIN_DISCRETE_INPUT_1, INPUT_PULLUP);
#endif
#ifdef PIN_DISCRETE_INPUT_2
  pinMode(PIN_DISCRETE_INPUT_2, INPUT_PULLUP);
#endif
#ifdef PIN_INPUT_REGISTRY_1
  pinMode(PIN_INPUT_REGISTRY_1, INPUT);
#endif
#ifdef PIN_INPUT_REGISTRY_2
  pinMode(PIN_INPUT_REGISTRY_2, INPUT);
#endif
#ifdef PIN_HOLDING_REGISTRY_OUTPUT_1
  pinMode(PIN_HOLDING_REGISTRY_OUTPUT_1, OUTPUT);
#endif
#ifdef PIN_HOLDING_REGISTRY_OUTPUT_2
  pinMode(PIN_HOLDING_REGISTRY_OUTPUT_2, OUTPUT);
#endif

  // RS485 control pin must be output
  pinMode(PIN_RS485_MODBUS_TE, OUTPUT);

  slave.cbVector[CB_READ_COILS] = read_coils;                             // Coils are R/W digital
  slave.cbVector[CB_READ_DISCRETE_INPUTS] = read_discrete_inputs;         // Discrete are RO digital Inputs
  slave.cbVector[CB_READ_INPUT_REGISTERS] = read_input_registers;         // Input Registry are RO analog Inputs
  slave.cbVector[CB_READ_HOLDING_REGISTERS] = read_holding_registers;     // Holding Registry are R/W analog registry
  slave.cbVector[CB_WRITE_COILS] = write_coils;
  slave.cbVector[CB_WRITE_HOLDING_REGISTERS] = write_holding_registers;

  Serial.begin(RS485_BAUD);
  slave.begin(RS485_BAUD);

#ifdef DELAY_SETUP_COMPLETED
  delay(DELAY_SETUP_COMPLETED);
#endif
}

void loop() {
  slave.poll();

#ifdef OPTION_USE_OLED
  twoline.loop();
#endif
}

#ifdef OPTION_USE_OLED
namespace show {
void update() {
  switch (display) {
    case Display::Coil:
      ::gos::atl::format::integer(buffer1, coil1 ? 1 : 0, &ida);
      ::gos::atl::format::integer(buffer2, coil2 ? 1 : 0, &idb);
      break;
    case Display::Registry:
      ::gos::atl::format::integer(buffer1, holdingregistry1, &ida);
      ::gos::atl::format::integer(buffer2, holdingregistry2, &idb);
      break;
  }
  twoline.display(buffer1, buffer2);
}
}
#endif
