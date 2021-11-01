#include "main.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "../utils/array.hpp"
#include "../utils/circular_buffer.hpp"
#include "../utils/arduino_helpers.hpp"
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <type_traits>

//#define DEBUG_SENSOR true
#define DEBUG_WALL

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
#if defined(DEBUG_SENSOR)
    Serial.printf(
                  "VL53L1 Satellite @ %#hhx: Status=%3i\n",
                  sensor->VL53L1_GetDeviceAddressValue(),
                  status_GetData);
#endif
    if (status_GetData == VL53L1_ERROR_NONE)
    {
#if defined(DEBUG_SENSOR)
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
#endif
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
  static circular_buffer<Distance, 2> wallDistances;
  const Distance wallSensor = distances[3]; // wall is to the right

  if (!isBumperPressed())
  {
    if (drives::LeftDrive::isIdle && drives::RightDrive::isIdle)
    {
      if (wallSensor > 0) // sensor value is OK
      {
        constexpr float path = 80;
        wallDistances.push_back(wallSensor);

        static bool lastMoveWasTurn = false;
        if (lastMoveWasTurn || wallDistances.size() != wallDistances.max_size())
        {
          drives::drive(path, drives::cruiseSpeed, false);
          lastMoveWasTurn = false;
#if defined(DEBUG_WALL)
        Serial.printf("Drive forward by %fmm\n", path);
#endif
        }
        else
        {
          const Distance oppositeSide = wallDistances.at(wallDistances.max_size() - 1) - wallDistances.at(0);
          const float angle = std::atan(static_cast<float>(oppositeSide) / static_cast<float>(path)) * 180.0
              / numbers::pi;
          drives::rotate(angle, drives::cruiseSpeed);
          lastMoveWasTurn = true;
#if defined(DEBUG_WALL)
        Serial.printf("Calculate angle with d=%fmm, b=%imm, angle=%f° and turn.\n", path, oppositeSide, angle);
#endif
        }
      }
      else
      {
#if defined(DEBUG_WALL)
        Serial.printf("Invalid wall sensor value: %i\n", wallSensor);
#endif
        wallDistances.clear();
        WAIT_AND_RETURN(1000);
        drives::rotateCounter(1, drives::cruiseSpeed, true);
      }
    }
    else
    {
      // wait until drives are idle
    }
  }
  else
  {
    wallDistances.clear();
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
