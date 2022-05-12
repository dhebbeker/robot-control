#pragma once

#include "../device_drivers/MCP23017Pin.hpp"
#include "../device_drivers/VL53L1GpioInterface.hpp"
#include "../utils/arduino_helpers.hpp"
#include <MCP23017.h>
#include <limits>
#include <cstddef>

namespace board
{

enum class DebugLeds { built_in, green, yellow, red, blue };

using Distance = decltype(VL53L1_RangingMeasurementData_t::RangeMilliMeter);
constexpr Distance distanceErrorValue = std::numeric_limits<Distance>::max();
constexpr std::size_t numberOfDistanceSensors = 4;

extern MCP23017 ioExpander1;

void setup(const InterruptFunctionPointer interruptForBumper = nullptr);
void loop();
bool isBumperPressed();
void setDebugLed(const std::uint8_t value, const DebugLeds led = DebugLeds::built_in);
const Distance (&getDistances())[numberOfDistanceSensors];
}
