#if !defined(DRIVES_HPP)
#define DRIVES_HPP

#include "board.hpp"
#include "../middleware/EnvironmentRecord.hpp"
#include <c_types.h>
#include <algorithm>
#include <cstdint>
#include <Arduino.h>

namespace drives
{

using Counter = std::uint16_t;
using Amplitude = std::uint16_t;
constexpr Amplitude maxAmplitude = 1023;
constexpr Amplitude cruiseSpeed = maxAmplitude / 2;
constexpr float stepsPerDeg = 100.0/(4.8*360.0);

template<typename MOTORCONTROL, MOTORCONTROL& motorControlpin, typename DIRECTIONPIN, DIRECTIONPIN& directionPin, typename ODOPIN, ODOPIN& odoPin>
class Drive
{
public:
	static Counter volatile counter;
	static Counter target;
	static bool isIdle;

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
		if(now - riseTime > board::odoMinIntervalDuration)
		{
			if (++counter >= target)
			{
				stop();
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
		digitalWrite(directionPin, !backwards ? LOW : HIGH);
		analogWrite(motorControlpin, std::min(maxAmplitude, amplitude));
	}
};

#define TYPEANDSYMBOL(t) decltype(t), t

using LeftDrive = Drive<TYPEANDSYMBOL(board::leftMotor), TYPEANDSYMBOL(board::leftBackwards), TYPEANDSYMBOL(board::leftOdoSignal)>;
using RightDrive = Drive<TYPEANDSYMBOL(board::rightMotor), TYPEANDSYMBOL(board::rightBackwards), TYPEANDSYMBOL(board::rightOdoSignal)>;

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

};

#endif // DRIVES_HPP
