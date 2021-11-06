#include "main.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "../utils/array.hpp"
#include "../utils/circular_buffer.hpp"
#include "../utils/arduino_helpers.hpp"
#include "Bearing.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <type_traits>

void main::setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.flush();
  delay(100);
  Serial.printf("\n begin program '%s'\n", __FILE__);
  board::setup(drives::stopDrives);
}

void main::loop()
{
  static Bearing bearing;
  board::loop();
  bearing.loop();
}
