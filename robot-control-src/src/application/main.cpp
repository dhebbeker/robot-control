#include "main.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "../utils/array.hpp"
#include "../utils/circular_buffer.hpp"
#include "../utils/arduino_helpers.hpp"
#include "../utils/Debug.hpp"
#include "Bearing.hpp"
#include "wifi_ap.hpp"
#include <ESP8266WiFi.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <type_traits>

void main::setup(const char * const programIdentificationString)
{
  Serial.begin(9600);
  delay(100);
  Serial.flush();
  delay(100);
  Serial.printf("\n begin program '%s'\n", programIdentificationString);
  board::setup(drives::stopDrives);


  Serial.printf("connect to wifi %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Serial.printf("webserver has IP %s\n", WiFi.localIP().toString().c_str());

#if DEBUG_VIA_WIFI
  DebugOutputStream::wiFiStream.begin(programIdentificationString, TELNET_PORT, DebugOutputStream::DebugLevel::PROFILER);
  DebugOutputStream::wiFiStream.setResetCmdEnabled(true); // Enable the reset command
  DebugOutputStream::wiFiStream.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
  DebugOutputStream::wiFiStream.showColors(true); // Colors
#endif
}

void main::loop()
{
  static Milliseconds mLastTime = 0;
  static Milliseconds mTimeSeconds = 0;

  static Bearing bearing;
  board::loop();
  board::setDebugLed(board::isBumperPressed(), board::DebugLeds::red);
  board::setDebugLed(!drives::isIdle(), board::DebugLeds::yellow);
  board::setDebugLed(drives::isIdle(), board::DebugLeds::green);

  // Each second
  if ((millis() - mLastTime) >= 1000)
  {

    mLastTime = millis();
    mTimeSeconds++;

    // Debug the time (verbose level)
    DEBUG_MSG_VERBOSE("* Time: %u seconds (VERBOSE)", mTimeSeconds);

    if (mTimeSeconds % 5 == 0)
    { // Each 5 seconds

      // Debug levels
      DEBUG_MSG_VERBOSE("* This is a message of debug level VERBOSE");
      DEBUG_MSG_DEBUG("* This is a message of debug level DEBUG");
      DEBUG_MSG_INFO("* This is a message of debug level INFO");
      DEBUG_MSG_WARNING("* This is a message of debug level WARNING");
      DEBUG_MSG_ERROR("* This is a message of debug level ERROR");
    }
  }

#if DEBUG_VIA_WIFI
  // RemoteDebug handle
  DebugOutputStream::wiFiStream.handle();
  // Give a time for ESP
  yield();
#endif

}
