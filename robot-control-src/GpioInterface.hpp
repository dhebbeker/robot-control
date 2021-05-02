#ifndef GPIOINTERFACE_HPP_
#define GPIOINTERFACE_HPP_

#include <cstdint>

class GpioInterface {
public:
	virtual void write(const std::uint8_t value) = 0;
	virtual int read() = 0;
	virtual void setMode(const std::uint8_t mode) = 0;

	virtual ~GpioInterface() = 0;
};

void digitalWrite(GpioInterface& gpio, const std::uint8_t value);
int digitalRead(GpioInterface& gpio);
void pinMode(GpioInterface& gpio, const std::uint8_t mode);

#endif /* GPIOINTERFACE_HPP_ */
