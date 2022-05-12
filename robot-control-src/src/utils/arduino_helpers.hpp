#ifndef UTILS_ARDUINO_HELPERS_HPP_
#define UTILS_ARDUINO_HELPERS_HPP_

#include <Arduino.h>
#include <cassert>
#include "Debug.hpp"
#include <cstddef>
#include <utility>

#if defined(round)
#undef round //see https://github.com/esp8266/Arduino/issues/5787#issuecomment-465852231
#endif
#if defined(abs)
#undef abs // else it conflicts with std::abs
#endif

using Milliseconds = decltype(millis());
using InterruptFunctionPointer = void (*)(void);
using SerialCharacter = decltype(std::declval<Stream>().read());

#if !defined(NDEBUG)
#include <Esp.h>
#define new_s_pre( T ) \
  { \
    const auto sizeAvailable = ESP.getMaxFreeBlockSize(); \
    const auto sizeNeeded = sizeof(T); \
    DEBUG_MSG_PROFILER("Try to allocate '%lu' bytes ('%lu' available).", sizeNeeded, sizeAvailable); \
    assert(sizeNeeded <= sizeAvailable); \
  }

#define new_s( T ) \
  [&](){ \
    new_s_pre( T ) \
    const auto p = new T; \
    assert(p != nullptr); \
    return p; \
  }();
#else
#define new_s_pre( T ) // empty on purpose
#define new_s( T ) new T
#endif

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
 *
 * \param duration time in milliseconds
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

/**
 * Prompts the user to enter characters and ENTER.
 * @param[out] buffer to write the read characters to
 * @param numberOfCharacters the number of characters to read, must be not greater than the size of the buffer
 * @param iostream the stream to write and read from
 * @return true as long as more characters must be read
 */
bool readFromSerial(SerialCharacter * const buffer = nullptr, const std::size_t numberOfCharacters = 0, Stream& iostream = Serial);

#endif /* UTILS_ARDUINO_HELPERS_HPP_ */
