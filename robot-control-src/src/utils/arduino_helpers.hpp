#ifndef UTILS_ARDUINO_HELPERS_HPP_
#define UTILS_ARDUINO_HELPERS_HPP_

#include <Arduino.h>

using Milliseconds = decltype(millis());

/**
 * Returns the current function for given duration.
 *
 * First time this is passed a time measurement is started.
 * As long as the measured time is not greater than the given duration
 * this does return the current function.
 *
 * When the duration is exceeded, the timer is reset and no return is executed.
 *
 * This macro can be used to implement a polling timer.
 */
#define WAIT_AND_RETURN( duration ) \
{ \
  static bool waitStarted = false; \
  static Milliseconds startTime; \
  if(!waitStarted) \
  { \
    startTime = millis(); \
    waitStarted = true; \
  } \
 \
  if(millis() > startTime + duration) \
  { \
    waitStarted = false; \
  } \
  else \
  { \
    return; \
  } \
}

#endif /* UTILS_ARDUINO_HELPERS_HPP_ */
