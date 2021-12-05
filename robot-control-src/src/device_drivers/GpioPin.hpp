#ifndef GPIOPIN_HPP_
#define GPIOPIN_HPP_

#include "GpioInterface.hpp"
#include "../utils/arduino_helpers.hpp"
#include <Arduino.h>

/**
 * Extension of GpioInterface for the processors general purpose input/output pins.
 *
 * Adds method to set PWM duty cycle.
 *
 * @tparam PIN is the Arduino pin number
 */
template<std::uint8_t PIN>
class GpioPin: public GpioInterface
{
public:
	static constexpr std::uint8_t pin = PIN;

	virtual int read() override
	{
		return digitalRead(pin);
	}

	virtual void setMode(const std::uint8_t mode) override
	{
		pinMode(pin, mode);
	}

	virtual void write(const std::uint8_t value) override
	{
		digitalWrite(pin, value);
	}

	/**
	 * Sets PWM duty cycle.
	 * @param dutyCycle between 0 and 255
	 */
	void setDutyCycle(const int dutyCycle) const
	{
		analogWrite(pin, dutyCycle);
	}
};

/* If digitalPinToInterrupt is defined as C preprocessor macro it needs to be
 * undefined. digitalPinToInterrupt must be overloaded for GpioPin. This is
 * not possible for macros.
 */
#if defined(digitalPinToInterrupt)

/**
 * Wrapper for digitalPinToInterrupt.
 *
 * Encapsulates the digitalPinToInterrupt functionality before it is undefined.
 * In contrast to the C preprocessor macro this function can be overridden.
 *
 * @param pinNumber is the Arduino pin number
 * @return interrupt number
 */
inline constexpr int digitalPinToInterruptWrapper(const int pinNumber)
{
	return digitalPinToInterrupt(pinNumber);
}

/* Undefine macro in order to enable function declaration with the same name. */
#undef digitalPinToInterrupt

/**
 * Getter for the interrupt number for a digital input pin.
 *
 * This is an adapter to digitalPinToInterruptWrapper().
 * @param pinNumber is the Arduino pin number
 * @return interrupt number
 */
inline constexpr int digitalPinToInterrupt(const int pinNumber)
{
	return digitalPinToInterruptWrapper(pinNumber);
}

#endif /* defined(digitalPinToInterrupt) */

/** @name Function overload for GpioPin objects.
 *
 *  These functions provide an interface to GpioPin objects compatible
 *  to the Arduino library functions.
 */
/** @{ */ /* Start group for function overloads */

/**
 * Getter for the interrupt number for a digital input pin.
 *
 * This is an overload to digitalPinToInterrupt().
 * @tparam PIN is the Arduino pin number
 * @return interrupt number
 */
template<std::uint8_t PIN>
int constexpr digitalPinToInterrupt(const GpioPin<PIN>& gpio)
{
	return digitalPinToInterrupt(gpio.pin);
}

/**
 * Attach function to be called on external interrupt.
 *
 * @param gpio is the input pin to attach interrupt to
 * @param interruptServiceRoutine is the function to be called when interrupt occurs
 * @param mode may be LOW, CHANGE, RISING, FALLING (see Arduino.h)
 * @tparam PIN is the Arduino pin number
 */
template<std::uint8_t PIN>
void attachInterrupt(const GpioPin<PIN>& gpio, const InterruptFunctionPointer interruptServiceRoutine, const int mode)
{
	attachInterrupt(digitalPinToInterrupt(gpio.pin), interruptServiceRoutine, mode);
}

/**
 * Sets PWM duty cycle to pin.
 * @tparam PIN is the Arduino pin number
 * @param gpio output pin to set duty cycle to.
 * @param dutyCycle between 0 and 255
 */
template<std::uint8_t PIN>
void analogWrite(const GpioPin<PIN>& gpio, const int dutyCycle)
{
	analogWrite(gpio.pin, dutyCycle);
}

/** @} */ /* End group for function overloads */

#endif /* GPIOPIN_HPP_ */
