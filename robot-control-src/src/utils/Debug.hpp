#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

#include <Arduino.h>

#if defined(DEBUG_ESP_PORT)
#define SERIAL_DEBUG_PORT DEBUG_ESP_PORT
#else
#define SERIAL_DEBUG_PORT Serial
#endif

namespace DebugOutputStream
{
  enum DebugLevel : std::uint8_t {
    PROFILER, VERBOSE, DEBUG, INFO, WARNING, ERROR, ANY
  };

  extern DebugLevel debugLevelSerial;

  /**
   * @tparam Args further parameter types for arguments passed to printf()
   * @param debugLevel will be compared to current active debug level
   * @param format pointer to a null-terminated multibyte string specifying how to interpret the data
   * @param a arguments specifying data to print.
   * @returns number of characters written
   */
  template <class... Args>
  static std::size_t printf(const DebugLevel debugLevel, const char * format, Args&&... a)
  {
    std::size_t charPrinted = 0;
    if(debugLevel>=debugLevelSerial)
    {
      charPrinted += SERIAL_DEBUG_PORT.printf(format, std::forward<Args>(a)...);
    }
    return charPrinted;
  }
};

#define DEBUG_MSG_PROFILER(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::PROFILER, "[" __FILE__ ":%u %s()] " fmt "\n", __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG_MSG_VERBOSE(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::VERBOSE, "[" __FILE__ ":%u %s()] " fmt "\n", __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG_MSG_DEBUG(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::DEBUG, "[" __FILE__ ":%u %s()] " fmt "\n", __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG_MSG_INFO(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::INFO, "[" __FILE__ ":%u %s()] " fmt "\n", __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG_MSG_WARNING(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::WARNING, "[" __FILE__ ":%u %s()] " fmt "\n", __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG_MSG_ERROR(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::ERROR, "[" __FILE__ ":%u %s()] " fmt "\n", __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG_MSG_ANY(fmt, ...) DebugOutputStream::printf(DebugOutputStream::DebugLevel::ANY, "[" __FILE__ ":%u %s()] " fmt "\n", __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG_MSG(fmt, ...) DEBUG_MSG_ANY(fmt, ##__VA_ARGS__)
