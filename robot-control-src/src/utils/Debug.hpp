#pragma once

#include <Arduino.h>
#include <cstddef>
#include <cstdint>
#include <utility>

#if defined(NDEBUG)
#if defined(DEBUG_ESP_PORT)
#undef DEBUG_ESP_PORT
#endif
#define DEBUG_VIA_WIFI false
#endif

/*
 * define DISABLE_DEBUG_VIA_WIFI or ENABLE_DEBUG_VIA_WIFI for all compilation units
 * in order to disable or enable the usage of RemoteDebug (via WiFi) for debugging
 */
#if !defined(DEBUG_VIA_WIFI)
#if   defined(DISABLE_DEBUG_VIA_WIFI) && !defined(ENABLE_DEBUG_VIA_WIFI)
#define DEBUG_VIA_WIFI false
#elif !defined(DISABLE_DEBUG_VIA_WIFI) && defined(ENABLE_DEBUG_VIA_WIFI)
#define DEBUG_VIA_WIFI true
#else
#define DEBUG_VIA_WIFI true
#endif
#endif

#if DEBUG_VIA_WIFI
#include <RemoteDebug.h> // https://github.com/jeroenst/RemoteDebug/
#endif

struct DebugOutputStream
{
  enum DebugLevel : std::uint8_t {
#if DEBUG_VIA_WIFI
    PROFILER = RemoteDebug::PROFILER,
    VERBOSE = RemoteDebug::VERBOSE,
    DEBUG = RemoteDebug::DEBUG,
    INFO = RemoteDebug::INFO,
    WARNING = RemoteDebug::WARNING,
    ERROR = RemoteDebug::ERROR,
    ANY = RemoteDebug::ANY
#else
    PROFILER, VERBOSE, DEBUG, INFO, WARNING, ERROR, ANY
#endif
  };
#if DEBUG_VIA_WIFI
  static RemoteDebug wiFiStream;
#endif
#if defined(DEBUG_ESP_PORT)
  DebugLevel static debugLevelSerial;
#endif
  template <class... Args>
  static std::size_t printf(const DebugLevel debugLevel, const char * format, Args&&... a)
  {
    std::size_t charPrinted = 0;
#if defined(DEBUG_ESP_PORT)
    if(debugLevel>=debugLevelSerial)
    {
      charPrinted += DEBUG_ESP_PORT.printf(format, std::forward<Args>(a)...);
    }
#endif /* defined(DEBUG_ESP_PORT) */
#if DEBUG_VIA_WIFI
    if(wiFiStream.isActive(debugLevel))
    {
      charPrinted += wiFiStream.printf(format, std::forward<Args>(a)...);
    }
#endif /* #if DEBUG_VIA_WIFI */

    return charPrinted;
  }
};

#define DEBUG_MSG_PROFILER(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::PROFILER, "(%s) " fmt "\n", __func__, ##__VA_ARGS__)
#define DEBUG_MSG_VERBOSE(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::VERBOSE, "(%s) " fmt "\n", __func__, ##__VA_ARGS__)
#define DEBUG_MSG_DEBUG(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::DEBUG, "(%s) " fmt "\n", __func__, ##__VA_ARGS__)
#define DEBUG_MSG_INFO(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::INFO, "(%s) " fmt "\n", __func__, ##__VA_ARGS__)
#define DEBUG_MSG_WARNING(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::WARNING, "(%s) " fmt "\n", __func__, ##__VA_ARGS__)
#define DEBUG_MSG_ERROR(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::ERROR, "(%s) " fmt "\n", __func__, ##__VA_ARGS__)
#define DEBUG_MSG_ANY(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::ANY, "(%s) " fmt "\n", __func__, ##__VA_ARGS__)
#define DEBUG_MSG(fmt, ...) DEBUG_MSG_ANY(fmt, ##__VA_ARGS__)

