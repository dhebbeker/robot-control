#include "board.hpp"
#include <Arduino.h>

namespace board {
decltype(leftOdoSignal) leftOdoSignal;
decltype(rightOdoSignal) rightOdoSignal;
decltype(rightBackwards) rightBackwards;
decltype(leftBackwards) leftBackwards;
decltype(leftMotor) leftMotor = D1;
decltype(rightMotor) rightMotor = D2;
decltype(debugLed) debugLed = D0;
};
