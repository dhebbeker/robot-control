#include "GpioInterface.hpp"

void digitalWrite(GpioInterface& gpio, const std::uint8_t value)
{
	gpio.write(value);
}

int digitalRead(GpioInterface& gpio)
{
	return gpio.read();
}

void pinMode(GpioInterface& gpio, const std::uint8_t mode)
{
	gpio.setMode(mode);
}
