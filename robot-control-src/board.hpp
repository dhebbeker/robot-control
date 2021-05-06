#if !defined(BOARD_HPP)
#define BOARD_HPP
#include "DigitalPin.hpp"
#include "MCP23017Pin.hpp"
#include <Arduino.h>
#include <cmath>
#include <MCP23017.h>

typedef decltype(millis()) Milliseconds;
constexpr auto pi = M_PI;

namespace board
{
extern std::uint8_t debugLed;
extern std::uint8_t leftMotor;
extern std::uint8_t rightMotor;
extern std::uint8_t scl;
extern std::uint8_t sda;
extern std::uint8_t rightOdoSignal;
extern std::uint8_t leftOdoSignal;
extern std::uint8_t ioExpanderIntB;
extern std::uint8_t ioExpanderIntAInv;
extern MCP23017 ioExpander1;
extern MCP23017Pin leftBackwards;
extern MCP23017Pin rightBackwards;
extern MCP23017Pin leftBumper;
extern MCP23017Pin rightBumper;

constexpr auto maxVelocity = 220; //!< [mm/s]
constexpr float odoIntervalLength = 12*pi; //!< [mm]
};

#endif // BOARD_HPP
