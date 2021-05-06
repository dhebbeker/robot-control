#if !defined(DRIVES_HPP)
#define DRIVES_HPP

#include "board.hpp"
#include <algorithm>
#include <cstdint>
#include <Arduino.h>

namespace drives
{

using Counter = std::uint16_t;
using Amplitude = std::uint16_t;
constexpr Amplitude maxAmplitude = 1023;
constexpr Amplitude cruiseSpeed = maxAmplitude / 2;

void odometryCounter();

class DriveInterface
{
public:
	Counter volatile counter;
	Counter target;
	bool isIdle;

	DriveInterface();
	virtual void init() = 0;
	virtual void evaluateInterval() = 0;
	virtual void stop() = 0;
	virtual void drive(const Counter distance, const Amplitude amplitude, const bool backwards) = 0;

	virtual ~DriveInterface();
};

template<typename MOTORCONTROL, typename DIRECTIONPIN, typename ODOPIN>
class Drive : public DriveInterface
{
public:

	Drive(MOTORCONTROL& motorControlPin, DIRECTIONPIN& directionPin, ODOPIN& odoPin) :
			motorControlpin(motorControlPin), directionPin(directionPin), odoPin(odoPin) {

	}

	virtual void init() override
	{
		target = counter = 0;
		pinMode(odoPin, INPUT);
		attachInterrupt(digitalPinToInterrupt(odoPin), odometryCounter, CHANGE);

		analogWrite(motorControlpin, 0);
		pinMode(directionPin, OUTPUT);
	}

	virtual void evaluateInterval() override
	{
		const bool isContact = digitalRead(odoPin) == LOW;
		static bool wasContact = isContact;
		static Milliseconds intervalStart = millis();

		if (wasContact && !isContact) // start interval
				{
			intervalStart = millis();
		} else if (!wasContact && isContact && (millis() - intervalStart) > board::odoMinIntervalDuration) {
			// valid interval end
			counter++;
			if (counter >= target)
				stop();
		}

		wasContact = isContact;
	}

	virtual void stop() override
	{
		analogWrite(motorControlpin, 0);
		isIdle = true;
	}

	virtual void drive(const Counter distance, const Amplitude amplitude, const bool backwards) override
	{
		counter = 0;
		target = distance;
		isIdle = false;
		digitalWrite(directionPin, backwards ? LOW : HIGH);
		analogWrite(motorControlpin, std::min(maxAmplitude, amplitude));
	}

	virtual ~Drive() {
	}

private:
	MOTORCONTROL &motorControlpin;
	DIRECTIONPIN &directionPin;
	ODOPIN &odoPin;
};

extern DriveInterface& leftDrive;
extern DriveInterface& rightDrive;

void rotateCounter(const Counter deg, const Amplitude amplitude, bool const clockwise);
void rotate(const float deg, const Amplitude amplitude, bool const clockwise);

void driveCounter(const Counter distance, const Amplitude amplitude, const bool backwards);
void drive(const float distance, const Amplitude amplitude, const bool backwards);

void stopDrives();

};

#endif // DRIVES_HPP
