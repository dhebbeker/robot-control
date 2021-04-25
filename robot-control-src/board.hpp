#if !defined(BOARD_HPP)
#define BOARD_HPP

#include <Arduino.h>
#include <cmath>

typedef decltype(millis()) Milliseconds;
constexpr auto pi = std::acos(-1);

namespace board
{
constexpr auto leftOdoSignal = D6; //!< blue
constexpr auto rightOdoSignal = D7; //!< green
constexpr auto rightBackwards = D5; //!< yellow
constexpr auto leftBackwards = D3; //!< white
constexpr auto leftMotor = D1; //!< brown
constexpr auto rightMotor = D2; //!< orange
constexpr auto debugLed = D4;

constexpr auto maxVelocity = 220; //!< [mm/s]
constexpr float odoIntervalLength = 12*pi; //!< [mm]
//! Minimum time with buffer factor [ms]
constexpr Milliseconds odoMinIntervalDuration = (odoIntervalLength * 1000.0) / (maxVelocity*2.0);
};

#endif // BOARD_HPP
