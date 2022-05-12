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
/**
 * Index to match distanceSensors[].
 * Does not necessarily match the numbers written on the devices.
 */
enum DistanceSensorIndex { front_left, right, count };

using Distance = decltype(VL53L1_RangingMeasurementData_t::RangeMilliMeter);
constexpr Distance distanceErrorValue = std::numeric_limits<Distance>::max();
constexpr std::size_t numberOfDistanceSensors = static_cast<std::size_t>(DistanceSensorIndex::count);
void testDistanceSensors();
extern MCP23017 ioExpander1;

void setup(const InterruptFunctionPointer interruptForBumper = nullptr);
void loop();
bool isBumperPressed();
void setDebugLed(const std::uint8_t value, const DebugLeds led = DebugLeds::built_in);
/**
 *
 * @param[out] returnDistance measured distance will be written to
 * @returns false on error
 */
bool retrieveSensorStatusOrError(const DistanceSensorIndex sensorIndex, Distance& returnDistance, const std::size_t maxNumberOfAttempts = 3);
const Distance (&getDistances())[numberOfDistanceSensors];
}
