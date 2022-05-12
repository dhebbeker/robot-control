#pragma once

#include "../utils/numbers.hpp"
#include "../utils/arduino_helpers.hpp"
#include "../middleware/EnvironmentRecord.hpp"
#include <c_types.h>
#include <cstdint>
#include <Arduino.h>
#include <algorithm>

namespace drives
{

using Counter = std::uint16_t;
using Amplitude = std::uint16_t;
constexpr Amplitude maxAmplitude = 1023;
constexpr Amplitude cruiseSpeed = maxAmplitude / 2;

/**
 * This was measured by rotating by 100 steps and then measuring the rotation.
 */
constexpr float stepsPerDeg = 100.0/(5.0*360.0 + 345.0);

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
constexpr float odoIntervalLength = (6.0 / 25.0) * 50 * numbers::pi; //!< [mm]
//! Minimum time with buffer factor [ms]
constexpr Milliseconds odoMinIntervalDuration = (odoIntervalLength * 1000.0) / (maxVelocity*2.0);
static_assert(odoMinIntervalDuration > 38, "threshold must be greater than longest pulse of photoelectric sensor (measured at slowest speed)");
static_assert(odoMinIntervalDuration < 142, "threshold must be smaller than shortest odometry interval between pulses (measured at highest speed)");

template<typename MOTORCONTROL, MOTORCONTROL& motorControlpin, typename DIRECTIONPIN, DIRECTIONPIN& directionPin, typename ODOPIN, ODOPIN& odoPin>
class Drive
{
public:
	static Counter volatile counter;
	static Counter target;
	static bool isIdle;
	static Milliseconds lastDuration;

	static void init()
	{
		target = counter = 0;
		pinMode(odoPin, INPUT);
		attachInterrupt(digitalPinToInterrupt(odoPin), evaluateInterval, RISING);

		analogWrite(motorControlpin, 0);
		pinMode(directionPin, OUTPUT);
	}

	IRAM_ATTR static void evaluateInterval()
	{
		const Milliseconds now = millis();
		static Milliseconds riseTime = now;
		if(now - riseTime > odoMinIntervalDuration)
		{
			if (++counter >= target)
			{
				stop();
				lastDuration = now - lastDuration;
			}
			riseTime = now;
		}
	}

	IRAM_ATTR static void stop()
	{
		analogWrite(motorControlpin, 0);
		isIdle = true;
	}

	static void drive(const Counter distance, const Amplitude amplitude, const bool backwards)
	{
		counter = 0;
		target = distance;
		isIdle = false;
		lastDuration = millis();
		digitalWrite(directionPin, !backwards ? LOW : HIGH);
		analogWrite(motorControlpin, std::min(maxAmplitude, amplitude));
	}
};

void initDrives();
void rotateCounter(const Counter deg, const Amplitude amplitude, bool const clockwise);

/**
 *
 * @param deg if negative, then counterclockwise
 * @param amplitude speed
 */
void rotate(const float deg, const Amplitude amplitude);

void driveCounter(const Counter distance, const Amplitude amplitude, const bool backwards);
void drive(const float distance, const Amplitude amplitude, const bool backwards);

Position flushCurrentPosition();

void stopDrives();
bool isIdle();

void calibrate(const float testDistance = 500);

}
