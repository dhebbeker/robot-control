#include "main.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "wifi_ap.hpp"
#include "WebserverHandle.hpp"
#include "../utils/array.hpp"
#include "Bearing.hpp"
#include <ESP8266WebServer.h>
#include <type_traits>
#include <functional>

static ActivityStates runningActivities { };
static EnvironmentRecord environmentRecord { };
static ESP8266WebServer server(80);
static WebserverHandle webserverHandle(server, environmentRecord, runningActivities);

void main::setup(const char * const programIdentificationString)
{
  Serial.begin(115200);
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

  server.begin();
  Serial.printf("webserver has IP %s\n", WiFi.localIP().toString().c_str());
  server.on("/", std::bind(&WebserverHandle::handleRoot, &webserverHandle)); // @suppress("Invalid arguments")
  webserverHandle.setup();
}

void main::loop()
{
  static Bearing bearing;
  board::loop();
  bearing.loop();
  board::setDebugLed(board::isBumperPressed(), board::DebugLeds::red);
  board::setDebugLed(!drives::isIdle(), board::DebugLeds::yellow);
  board::setDebugLed(drives::isIdle(), board::DebugLeds::green);

  server.handleClient();
  if(drives::isIdle())
  {
    const Position newPositionCandidate = drives::flushCurrentPosition();
    if(environmentRecord.positions[environmentRecord.positionIndex] != newPositionCandidate && !runningActivities.isBearingRunning)
    {
      environmentRecord.positions[++environmentRecord.positionIndex] = newPositionCandidate;
      environmentRecord.positionIndex %= environmentRecord.numberOfPositions;
    }
    const auto newTarget = webserverHandle.flushTargetRequest();
    if(newTarget.isTargetNew)
    {
      assert(!runningActivities.isBearingRunning);
      const bool bumperIsPressed = board::isBumperPressed();
      if(newTarget.newDrive!=0 && (!newTarget.forward || !bumperIsPressed))
      {
        drives::driveCounter(newTarget.newDrive, drives::cruiseSpeed, !newTarget.forward);
      }
      else if(newTarget.newRotate!=0 && !bumperIsPressed)
      {
        drives::rotateCounter(newTarget.newRotate, drives::cruiseSpeed, newTarget.clockwise);
      }
    }
    else
    {
      runningActivities.isManualRunning = false;
    }
    static bool bearingBefore = false;
    if (runningActivities.isBearingRunning)
    {
      assert(!newTarget.isTargetNew);
      if (!bearingBefore)
      {
        bearing.begin();
        bearingBefore = true;
      }
    }
    else
    {
      if (bearingBefore)
      {
        bearing.stop();
        bearingBefore = false;
      }
    }
  }
  webserverHandle.loop();

  yield(); // Give a time for ESP
}
