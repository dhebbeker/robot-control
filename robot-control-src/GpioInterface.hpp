#ifndef GPIOINTERFACE_HPP_
#define GPIOINTERFACE_HPP_

#include <cstdint>

class GpioInterface {
public:

	virtual void write(const bool high) = 0;
	virtual bool isHigh() = 0;

	enum class Mode {
		input, input_pullup, output
	};
	virtual void setMode(const Mode mode) = 0;

	enum class InterruptMode {
		low, change, rising, falling
	};
	virtual void attachInterrupt(const void (*const interruptServiceRoutine)(void), const InterruptMode interruptMode) = 0;

	virtual ~GpioInterface() = 0;
};

#endif /* GPIOINTERFACE_HPP_ */
