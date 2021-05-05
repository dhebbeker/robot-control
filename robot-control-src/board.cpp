#include "board.hpp"
#include <Arduino.h>

namespace board {
constexpr std::uint8_t ioExpander1Address = 0x20;
MCP23017 ioExpander1(ioExpander1Address);
MCP23017Pin leftBackwards(ioExpander1, 8+0);
MCP23017Pin rightBackwards(ioExpander1, 8+1);
MCP23017Pin leftBumper(ioExpander1, 8+6);
MCP23017Pin rightBumper(ioExpander1, 8+7);
};
