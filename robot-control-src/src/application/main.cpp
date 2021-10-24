#include "main.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "../utils/array.hpp"
#include <assert.h>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <type_traits>

using Distance = decltype(VL53L1_RangingMeasurementData_t::RangeMilliMeter);
static Distance distances[size(board::distanceSensors)] { };

void main::setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.flush();
  delay(100);
  Serial.printf("\n begin program '%s'\n", __FILE__);
  Wire.begin(board::sda, board::scl);
  board::ioExpander1.init();
  board::ioExpander1.interruptMode(MCP23017InterruptMode::Separated);
  board::ioExpander1.interrupt(MCP23017Port::B, FALLING);

  // initialize pins
  pinMode(board::debugLed, OUTPUT);
  digitalWrite(board::debugLed, LOW);
  pinMode(board::ioExpanderIntB, INPUT_PULLUP);
  pinMode(board::ioExpanderIntAInv, INPUT);
  pinMode(board::VL53L1_1_INT, INPUT_PULLUP);
  pinMode(board::VL53L1_2_INT, INPUT_PULLUP);
  pinMode(board::VL53L1_3_INT, INPUT_PULLUP);
  pinMode(board::VL53L1_4_INT, INPUT_PULLUP);
  pinMode(board::leftBumper, INPUT_PULLUP);
  pinMode(board::rightBumper, INPUT_PULLUP);

  drives::LeftDrive::init();
  drives::RightDrive::init();

  // initialize distance measurement sensors
  for(std::size_t i=0; i<size(board::distanceSensors); i++)
  {
    board::distanceSensors[i]->begin();
    board::distanceSensors[i]->VL53L1_Off();
  }
  for(std::size_t i=0; i<size(board::distanceSensors); i++)
  {
    Serial.printf("Initialize distance sensor %u...", i);
    assert(board::distanceSensors[i]->initSensorWithAddressValue(VL53L1GpioInterface::defaultAddressValue + 1 + i) == VL53L1_ERROR_NONE);
    Serial.printf("Done.\n");
  }
  for(std::size_t i=0; i<size(board::distanceSensors); i++)
  {
    board::distanceSensors[i]->VL53L1_SetPresetMode(VL53L1_PRESETMODE_RANGING);
    board::distanceSensors[i]->VL53L1_ClearInterruptAndStartMeasurement();
  }

  attachInterrupt(board::ioExpanderIntB, drives::stopDrives, FALLING);
}

static Distance retrieveSensorStatus(VL53L1GpioInterface* const sensor)
{
  uint8_t newDataReady = 0;
  const VL53L1_Error status_getReady = sensor->VL53L1_GetMeasurementDataReady(&newDataReady);
  Distance newDistance = -1;

  if ((status_getReady == VL53L1_ERROR_NONE) && (newDataReady != 0))
  {
    VL53L1_RangingMeasurementData_t rangingMeasurement = VL53L1_RangingMeasurementData_t();
    const VL53L1_Error status_GetData = sensor->VL53L1_GetRangingMeasurementData(&rangingMeasurement);
    sensor->VL53L1_ClearInterruptAndStartMeasurement();
    Serial.printf(
                  "VL53L1 Satellite @ %#hhx: Status=%3i\n",
                  sensor->VL53L1_GetDeviceAddressValue(),
                  status_GetData);
    if (status_GetData == VL53L1_ERROR_NONE)
    {
      Serial.printf(
                    "count=%3hhu, \t"
                    "status=%3hhu, \t"
                    "distance=%4hu mm, \t"
                    "sig. rate=%5.2f MCPS, \t"
                    "noise rate=%5.2f MCPS, \t"
                    "σ=%5.2f mm, \t"
                    "\n",
                    rangingMeasurement.StreamCount,
                    rangingMeasurement.RangeStatus,
                    rangingMeasurement.RangeMilliMeter,
                    rangingMeasurement.SignalRateRtnMegaCps / 65536.0,
                    rangingMeasurement.AmbientRateRtnMegaCps / 65536.0,
                    rangingMeasurement.SigmaMilliMeter / 65536.0);
      if (rangingMeasurement.RangeStatus == 0)
      {
        newDistance = rangingMeasurement.RangeMilliMeter;
      }
    }
  }
  return newDistance;
}

static bool isBumperPressed()
{
  return digitalRead(board::leftBumper) == LOW || digitalRead(board::rightBumper) == LOW;
}

static void followWall()
{
  const Distance wallSensor = distances[3]; // wall is to the right

  if (wallSensor > 0 && !isBumperPressed()) // sensor value is OK
  {
    constexpr Distance threshold = 100; /* mm */
    constexpr drives::Amplitude p = drives::maxAmplitude / 2 / 200;
    using Speed = std::make_signed<Distance>::type;
    const Speed deviation = threshold - wallSensor;
    const drives::Amplitude left = std::max( // @suppress("Invalid arguments")
                                            std::min( // @suppress("Invalid arguments")
                                                     static_cast<Speed>(drives::maxAmplitude),
                                                     static_cast<Speed>(drives::cruiseSpeed + deviation * p)),
                                            static_cast<Speed>(0));
    const drives::Amplitude right = std::max( // @suppress("Invalid arguments")
                                             std::min( // @suppress("Invalid arguments")
                                                      static_cast<Speed>(drives::maxAmplitude),
                                                      static_cast<Speed>(drives::cruiseSpeed - deviation * p)),
                                             static_cast<Speed>(0));
    drives::drive(50, left, right, false);
  }
  else
  {
    drives::stopDrives();
  }
}

void main::loop()
{
  for(std::size_t i=0; i<size(board::distanceSensors); ++i)
  {
    distances[i] = retrieveSensorStatus(board::distanceSensors[i]);
  }
  followWall();
}
