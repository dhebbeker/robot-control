#ifndef ROBOT_CONTROL_SRC_PIN_HPP_
#define ROBOT_CONTROL_SRC_PIN_HPP_

#include "GpioInterface.hpp"
#include <cstdint>

class Pin: public GpioInterface {
private:
	const std::uint8_t pinNumber;
	static std::uint8_t modeToInt(const Mode);
	static int interruptModeToInt(const InterruptMode);
public:
	Pin(const std::uint8_t pinNumber);
	virtual ~Pin();
	virtual void write(const bool high) override;
	virtual void setMode(const GpioInterface::Mode) override;
	virtual bool isHigh() override;
	virtual void attachInterrupt(void (*const interruptServiceRoutine)(void), const InterruptMode interruptMode) override;
};

#endif /* ROBOT_CONTROL_SRC_PIN_HPP_ */
