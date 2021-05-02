#ifndef ROBOT_CONTROL_SRC_DIGITALPIN_HPP_
#define ROBOT_CONTROL_SRC_DIGITALPIN_HPP_

#include "GpioInterface.hpp"
#include <Arduino.h>

template<std::uint8_t pin>
class DigitalPin: public GpioInterface {
public:
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
};

#endif /* ROBOT_CONTROL_SRC_DIGITALPIN_HPP_ */
