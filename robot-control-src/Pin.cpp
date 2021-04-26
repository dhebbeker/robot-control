#include "Pin.hpp"
#include <Arduino.h>
#include <assert.h>

Pin::~Pin() {
}

void Pin::write(const bool high) {
	const std::uint8_t value = (high ? HIGH : LOW);
	digitalWrite(pinNumber, value);
}

void Pin::setMode(const GpioInterface::Mode mode) {
	pinMode(pinNumber, modeToInt(mode));
}

Pin::Pin(std::uint8_t const pinNumber) :
		pinNumber(pinNumber) {
}

bool Pin::isHigh() {
	if (digitalRead(pinNumber) == LOW) {
		return false;
	} else {
		return true;
	}
}

void Pin::write(const bool high) {
}

void Pin::setMode(const GpioInterface::Mode mode) {
}

std::uint8_t Pin::modeToInt(Mode const mode) {
	switch (mode) {
	case Mode::input:
		return INPUT;
		break;
	case Mode::input_pullup:
		return INPUT_PULLUP;
		break;
	case Mode::output:
		return OUTPUT;
		break;
	default:
		assert(false);
		return -1;
		break;
	}
}

int Pin::interruptModeToInt(InterruptMode const interruptMode) {
	switch (interruptMode) {
	case InterruptMode::low:
		return ONLOW;
		break;
	case InterruptMode::change:
		return CHANGE;
		break;
	case InterruptMode::rising:
		return RISING;
		break;
	case InterruptMode::falling:
		return FALLING;
		break;
	default:
		assert(false);
		return -1;
		break;
	}
}

void Pin::attachInterrupt(void (*const interruptServiceRoutine)(), const GpioInterface::InterruptMode interruptMode) {
	::attachInterrupt(pinNumber, interruptServiceRoutine, interruptModeToInt(interruptMode));
}
