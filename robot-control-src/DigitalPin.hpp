#ifndef ROBOT_CONTROL_SRC_DIGITALPIN_HPP_
#define ROBOT_CONTROL_SRC_DIGITALPIN_HPP_

#include "GpioInterface.hpp"
#include <Arduino.h>

template<std::uint8_t PIN>
class DigitalPin: public GpioInterface {
public:
	static constexpr std::uint8_t pin = PIN;

	virtual int read() override
	{
		return readS();
	}
	virtual void setMode(const std::uint8_t mode) override
	{
		setModeS(mode);
	}
	virtual void write(const std::uint8_t value) override
	{
		writeS(value);
	}
	void setDutyCycle(const int dutyCycle) const
	{
		setDutyCycleS(dutyCycle);
	}
	static int readS()
	{
		return digitalRead(pin);
	}
	static void setModeS(const std::uint8_t mode)
	{
		pinMode(pin, mode);
	}
	static void writeS(const std::uint8_t value)
	{
		digitalWrite(pin, value);
	}
	static void setDutyCycleS(const int dutyCycle)
	{
		analogWrite(pin, dutyCycle);
	}
};

template<std::uint8_t PIN>
void analogWrite(const DigitalPin<PIN>& gpio, const int dutyCycle)
{
	analogWrite(gpio.pin, dutyCycle);
}

inline constexpr int digitalPinToInterruptWrapper(const int pinNumber)
{
	return digitalPinToInterrupt(pinNumber);
}

#undef digitalPinToInterrupt

inline constexpr int digitalPinToInterrupt(const int pinNumber)
{
	return digitalPinToInterruptWrapper(pinNumber);
}

template<std::uint8_t PIN>
int constexpr digitalPinToInterrupt(const DigitalPin<PIN>& gpio)
{
	return digitalPinToInterruptWrapper(gpio.pin);
}

template<std::uint8_t PIN>
void attachInterrupt(const DigitalPin<PIN>& gpio, void (* const interruptServiceRoutine)(void), const int mode)
{
	attachInterrupt(digitalPinToInterrupt(gpio.pin), interruptServiceRoutine, mode);
}


#endif /* ROBOT_CONTROL_SRC_DIGITALPIN_HPP_ */
