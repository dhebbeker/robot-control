#include "Debug.hpp"

#if DEBUG_VIA_WIFI
RemoteDebug DebugOutputStream::wiFiStream;
#endif
#if defined(DEBUG_ESP_PORT)
DebugOutputStream::DebugLevel DebugOutputStream::debugLevelSerial = DebugOutputStream::DebugLevel::PROFILER;
#endif
