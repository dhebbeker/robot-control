#include "MCP23017Pin.hpp"
#include <MCP23017.h>

MCP23017Pin::MCP23017Pin(MCP23017 &ioExpander, std::uint8_t const pin) :ioExpander(ioExpander), pin(pin) {
}

MCP23017Pin::~MCP23017Pin() {
}

int MCP23017Pin::read() {
	return ioExpander.digitalRead(pin);
}

void MCP23017Pin::setMode(std::uint8_t const mode) {
	setMode(mode, false);
}

void MCP23017Pin::setMode(std::uint8_t const mode, bool const inverted) {
	ioExpander.pinMode(pin, mode, inverted);
}

void MCP23017Pin::write(std::uint8_t const value) {
	ioExpander.digitalWrite(pin, value);
}
