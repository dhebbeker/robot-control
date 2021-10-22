#ifndef MCP23017PIN_HPP_
#define MCP23017PIN_HPP_

#include "GpioInterface.hpp"

class MCP23017;

class MCP23017Pin: public GpioInterface {
public:
	MCP23017Pin(MCP23017& ioExpander, const std::uint8_t pin);
	virtual ~MCP23017Pin();
	virtual int read() override;
	virtual void setMode(const std::uint8_t mode) override;
	void setMode(const std::uint8_t mode, const bool inverted);
	virtual void write(const std::uint8_t value) override;
private:
	MCP23017& ioExpander;
	const std::uint8_t pin;
};

#endif /* MCP23017PIN_HPP_ */
