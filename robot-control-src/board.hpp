#if !defined(BOARD_HPP)
#define BOARD_HPP
#include "DigitalPin.hpp"
#include "MCP23017Pin.hpp"
#include <Arduino.h>
#include <cmath>
#include <MCP23017.h>

typedef decltype(millis()) Milliseconds;
constexpr auto pi = std::acos(-1);

namespace board
{
constexpr std::uint8_t debugLed = D0;
constexpr std::uint8_t leftMotor = D1;
constexpr std::uint8_t rightMotor = D2;
constexpr std::uint8_t scl = D3;
constexpr std::uint8_t sda = D4;
constexpr std::uint8_t rightOdoSignal = D5;
constexpr std::uint8_t leftOdoSignal = D6;
constexpr std::uint8_t ioExpanderIntB = D7;
constexpr std::uint8_t ioExpanderIntAInv = D8;
extern MCP23017 ioExpander1;
extern MCP23017Pin leftBackwards;
extern MCP23017Pin rightBackwards;
extern MCP23017Pin leftBumper;
extern MCP23017Pin rightBumper;

constexpr auto maxVelocity = 220; //!< [mm/s]
constexpr float odoIntervalLength = 12*pi; //!< [mm]
//! Minimum time with buffer factor [ms]
constexpr Milliseconds odoMinIntervalDuration = (odoIntervalLength * 1000.0) / (maxVelocity*2.0);
};

#endif // BOARD_HPP
