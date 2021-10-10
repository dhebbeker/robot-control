#if !defined(BOARD_HPP)
#define BOARD_HPP
#include "MCP23017Pin.hpp"
#include "VL53L1GpioInterface.hpp"
#include <Arduino.h>
#include <cmath>
#include <MCP23017.h>

typedef decltype(millis()) Milliseconds;
constexpr auto pi = M_PI;

namespace board
{
const extern std::uint8_t debugLed;
const extern std::uint8_t leftMotor;
const extern std::uint8_t rightMotor;
const extern std::uint8_t scl;
const extern std::uint8_t sda;
const extern std::uint8_t rightOdoSignal;
const extern std::uint8_t leftOdoSignal;
const extern std::uint8_t ioExpanderIntB;
const extern std::uint8_t ioExpanderIntAInv;
extern MCP23017 ioExpander1;
extern MCP23017Pin VL53L1_1_INT;
extern MCP23017Pin VL53L1_1_XSHUT;
extern MCP23017Pin VL53L1_2_INT;
extern MCP23017Pin VL53L1_2_XSHUT;
extern MCP23017Pin VL53L1_3_INT;
extern MCP23017Pin VL53L1_3_XSHUT;
extern MCP23017Pin VL53L1_4_INT;
extern MCP23017Pin VL53L1_4_XSHUT;
extern MCP23017Pin leftBackwards;
extern MCP23017Pin rightBackwards;
extern MCP23017Pin leftBumper;
extern MCP23017Pin rightBumper;
extern VL53L1GpioInterface * const distanceSensors[4];

/**
 * Maximum robot velocity.
 *
 * According to [this wiki](https://rn-wissen.de/wiki/index.php/CCRP5#Technische_Daten).
 */
constexpr auto maxVelocity = 220; //!< [mm/s]
/**
 * distance of one outer wheel turn U = d*pi
 * diameter of outer wheel d = 50 mm
 * fraction of one odometry interval to outer wheel f = 6/25
 * distance of one odometry interval = f*U
 */
constexpr float odoIntervalLength = 12*pi; //!< [mm]
//! Minimum time with buffer factor [ms]
constexpr Milliseconds odoMinIntervalDuration = (odoIntervalLength * 1000.0) / (maxVelocity*2.0);
static_assert(odoMinIntervalDuration > 38, "threshold must be greater than longest pulse of photoelectric sensor (measured at slowest speed)");
static_assert(odoMinIntervalDuration < 142, "threshold must be smaller than shortest odometry interval between pulses (measured at highest speed)");
};

#endif // BOARD_HPP
