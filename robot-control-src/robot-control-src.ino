#include "src/application/main.hpp"

const auto programIdentificationString = __FILE__ " compiled at " __DATE__ " " __TIME__;

void setup()
{
  main::setup(programIdentificationString);
}

void loop()
{
  main::loop();
}
