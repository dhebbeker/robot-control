#if !defined(BOARD_HPP)
#define BOARD_HPP
#include "DigitalPin.hpp"
#include <Arduino.h>
#include <cmath>

typedef decltype(millis()) Milliseconds;
constexpr auto pi = std::acos(-1);

namespace board
{
extern DigitalPin<D6> leftOdoSignal;
extern DigitalPin<D5> rightOdoSignal;
extern DigitalPin<D5> rightBackwards;
extern DigitalPin<D5> leftBackwards;
extern const std::uint8_t leftMotor;
extern const std::uint8_t rightMotor;
extern const std::uint8_t debugLed;

constexpr auto maxVelocity = 220; //!< [mm/s]
constexpr float odoIntervalLength = 12*pi; //!< [mm]
//! Minimum time with buffer factor [ms]
constexpr Milliseconds odoMinIntervalDuration = (odoIntervalLength * 1000.0) / (maxVelocity*2.0);
};

#endif // BOARD_HPP
