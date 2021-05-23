#include "board.hpp"
#include <Arduino.h>

namespace board {
std::uint8_t debugLed = D0;
std::uint8_t leftMotor = D1;
std::uint8_t rightMotor = D2;
std::uint8_t scl = D3;
std::uint8_t sda = D4;
std::uint8_t rightOdoSignal = D5;
std::uint8_t leftOdoSignal = D6;
std::uint8_t ioExpanderIntB = D7;
std::uint8_t ioExpanderIntAInv = D8;
constexpr std::uint8_t ioExpander1Address = 0x20;
MCP23017 ioExpander1(ioExpander1Address);
MCP23017Pin VL53L1_1_INT(ioExpander1, 0);
MCP23017Pin VL53L1_1_XSHUT(ioExpander1, 1);
MCP23017Pin VL53L1_2_INT(ioExpander1, 2);
MCP23017Pin VL53L1_2_XSHUT(ioExpander1, 3);
MCP23017Pin VL53L1_3_INT(ioExpander1, 4);
MCP23017Pin VL53L1_3_XSHUT(ioExpander1, 5);
MCP23017Pin VL53L1_4_INT(ioExpander1, 6);
MCP23017Pin VL53L1_4_XSHUT(ioExpander1, 7);
MCP23017Pin leftBackwards(ioExpander1, 8+0);
MCP23017Pin rightBackwards(ioExpander1, 8+1);
MCP23017Pin leftBumper(ioExpander1, 8+6);
MCP23017Pin rightBumper(ioExpander1, 8+7);
static VL53L1GpioInterface distanceSensor1(&Wire, VL53L1_1_XSHUT);
static VL53L1GpioInterface distanceSensor2(&Wire, VL53L1_2_XSHUT);
static VL53L1GpioInterface distanceSensor3(&Wire, VL53L1_3_XSHUT);
static VL53L1GpioInterface distanceSensor4(&Wire, VL53L1_4_XSHUT);
VL53L1GpioInterface * const distanceSensors[] = {
		&distanceSensor1,
		&distanceSensor2,
		&distanceSensor3,
		&distanceSensor4,
	};
};
