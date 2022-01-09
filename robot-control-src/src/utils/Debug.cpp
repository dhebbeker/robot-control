#include "Debug.hpp"

#if DEBUG_VIA_WIFI
RemoteDebug DebugOutputStream::wiFiStream;
#endif
#if DEBUG_VIA_SERIAL
DebugOutputStream::DebugLevel DebugOutputStream::debugLevelSerial = DebugOutputStream::DebugLevel::PROFILER;
#endif
