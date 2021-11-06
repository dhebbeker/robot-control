#if !defined(BOARD_HPP)
#define BOARD_HPP
#include "../device_drivers/MCP23017Pin.hpp"
#include "../device_drivers/VL53L1GpioInterface.hpp"
#include "../utils/arduino_helpers.hpp"
#include <MCP23017.h>
#include <limits>

namespace board
{
using Distance = decltype(VL53L1_RangingMeasurementData_t::RangeMilliMeter);
constexpr Distance distanceErrorValue = std::numeric_limits<Distance>::max();
constexpr std::size_t numberOfDistanceSensors = 4;

extern MCP23017 ioExpander1;

void setup(const InterruptFunctionPointer interruptForBumper = nullptr);
void loop();
bool isBumperPressed();
void setDebugLed(const std::uint8_t value);
}

#endif // BOARD_HPP
