#ifndef GPIOINTERFACE_HPP_
#define GPIOINTERFACE_HPP_

#include <cstdint>

/**
 * Interface for digital input and output pins.
 *
 * Implementations may provide access to the pins of the microprocessor or
 * external pins of IO-expanders.
 *
 * Parameters and return values are compatible to the Arduino library.
 */
class GpioInterface
{
public:
	/**
	 * Sets output value of the pin.
	 * @param value may be HIGH or LOW (see Arduino.h)
	 * @pre Configure the mode to OUTPUT using setMode().
	 */
	virtual void write(const std::uint8_t value) = 0;

	/**
	 * Reads the value of the pin.
	 * @return Either HIGH or LOW (see Arduino.h).
	 * @pre Configure the mode to INPUT or INPUT_PULLUP using setMode().
	 */
	virtual int read() = 0;

	/**
	 * Configures the pin to be either input or output.
	 * @param mode INPUT, OUTPUT, or INPUT_PULLUP (see Arduino.h).
	 */
	virtual void setMode(const std::uint8_t mode) = 0;

	virtual ~GpioInterface() = default;
};

/** @name Digital I/O function overload for GpioInterface objects.
 *
 *  These functions provide an interface to GpioInterface objects compatible
 *  to the Arduino library digital I/O functions.
 */
/** @{ */ /* Start group for digital I/O */

/**
 * Sets output value of the pin.
 * @param gpio digital output pin
 * @param value may be HIGH or LOW (see Arduino.h)
 * @pre Configure the mode to OUTPUT using pinMode().
 */
void digitalWrite(GpioInterface& gpio, const std::uint8_t value);

/**
 * Reads the value of the pin.
 * @param gpio digital input pin
 * @return Either HIGH or LOW (see Arduino.h).
 * @pre Configure the mode to INPUT or INPUT_PULLUP using pinMode().
 */
int digitalRead(GpioInterface& gpio);

/**
 * Configures the pin to be either input or output.
 * @param gpio digital pin
 * @param mode INPUT, OUTPUT, or INPUT_PULLUP (see Arduino.h).
 */
void pinMode(GpioInterface& gpio, const std::uint8_t mode);

/** @} */ /* End group for digital I/O */

#endif /* GPIOINTERFACE_HPP_ */
