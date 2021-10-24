#include "main.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "wifi_ap.hpp"
#include "WebserverHandle.hpp"
#include "../utils/array.hpp"
#include <assert.h>
#include <ESP8266WebServer.h>
#include <algorithm>
#include <cstddef>
#include <functional>

using Distance = decltype(VL53L1_RangingMeasurementData_t::RangeMilliMeter);

static EnvironmentRecord environmentRecord { };
static ESP8266WebServer server(80);
static WebserverHandle webserverHandle(server, environmentRecord);

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

  Serial.printf("connect to wifi %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  server.begin();
  Serial.printf("webserver has IP %s\n", WiFi.localIP().toString().c_str());
  server.on("/", std::bind(&WebserverHandle::handleRoot, &webserverHandle)); // @suppress("Invalid arguments")
  webserverHandle.setup();
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

void main::loop()
{
  for(std::size_t i=0; i<size(board::distanceSensors); ++i)
  {
    distances[i] = retrieveSensorStatus(board::distanceSensors[i]);
  }
  server.handleClient();
  //Serial.printf("left: \t%3u, right: \t%3u\n", drives::LeftDrive::counter, drives::RightDrive::counter);
  if(drives::LeftDrive::isIdle && drives::RightDrive::isIdle)
  {
    const Position newPositionCandidate = drives::flushCurrentPosition();
    if(environmentRecord.positions[environmentRecord.positionIndex] != newPositionCandidate)
    {
      environmentRecord.positions[++environmentRecord.positionIndex] = newPositionCandidate;
      environmentRecord.positionIndex %= environmentRecord.numberOfPositions;
      webserverHandle.loop();
    }
    const auto newTarget = webserverHandle.flushTargetRequest();
    if(newTarget.isTargetNew)
    {
      const bool bumperIsPressed = digitalRead(board::leftBumper) == LOW || digitalRead(board::rightBumper) == LOW;
      if(newTarget.newDrive!=0 && (!newTarget.forward || !bumperIsPressed))
      {
        drives::driveCounter(newTarget.newDrive, drives::cruiseSpeed, !newTarget.forward);
      }
      else if(newTarget.newRotate!=0 && !bumperIsPressed)
      {
        drives::rotateCounter(newTarget.newRotate, drives::cruiseSpeed, newTarget.clockwise);
      }
    }
  }
}
