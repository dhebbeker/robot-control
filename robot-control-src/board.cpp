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
MCP23017Pin leftBackwards(ioExpander1, 8+0);
MCP23017Pin rightBackwards(ioExpander1, 8+1);
MCP23017Pin leftBumper(ioExpander1, 8+6);
MCP23017Pin rightBumper(ioExpander1, 8+7);
};
