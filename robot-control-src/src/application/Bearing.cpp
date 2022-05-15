#include "Bearing.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "FollowingWallStates.hpp"
#include "../utils/array.hpp"
#include "../utils/arduino_helpers.hpp"
#include "../utils/lazy_creation.hpp"
#include <cmath>
#include <limits>

using namespace drives;

float shortenAngle(const float& angle)
{
  const std::int8_t sign = angle < 0 ? -1 : 1;
  return (std::fmod(std::abs(angle) + 180, 360.0) - 180) * sign;
}

Bearing::Bearing() :
    PollingStateMachine(new StopBearing())
{

}

AligningToWall::AligningToWall(const PolarVector vectorToWall) : vectorToWall(vectorToWall)
{
  PRINT_CHECKPOINT();
  DEBUG_MSG_VERBOSE("Vector to wall points to %f degrees, %imm", vectorToWall.angle, vectorToWall.length);
}

PollingStateMachine::State* AligningToWall::operation()
{
if (vectorToWall.length > FollowingWall::targetDistanceToWall)
{
  /* drive towards the wall and turn left */
  const DriveOrders newOrders(
  {
  { .angle = vectorToWall.angle, .length = vectorToWall.length - FollowingWall::targetDistanceToWall },
  { .angle = -90 } });
  return newDriver(newOrders, createCreatorForNewObject<FollowingWall>());
}
else
{
  /* drive away from the wall and turn right */
  const DriveOrders newOrders(
  {
  { .angle = shortenAngle(vectorToWall.angle + 180), .length = FollowingWall::targetDistanceToWall - vectorToWall.length },
  { .angle = 90 } });
  return newDriver(newOrders, createCreatorForNewObject<FollowingWall>());

}
}

PollingStateMachine::State* Lost::operation()
{
  if (numberOfScan <= maxNumberOfScans)
  {
    if (isIdle() && !board::isBumperPressed())
    {
      board::Distance currentMinDistance = std::numeric_limits<board::Distance>::max();
      Counter currentMinOrientation = 0;
      const auto &distances = board::getDistances();

      // check the shortest distance of all sensors
      for (Counter i = 0; i < size(distances); i++)
      {
        if (distances[i] < currentMinDistance && distances[i] != board::distanceErrorValue)
        {
          currentMinOrientation = i;
          currentMinDistance = distances[i];
        }
      }

      // compare with previous scans
      if (currentMinDistance < minDistance)
      {
        minDistance = currentMinDistance;
        orientationToMinDistance = currentMinOrientation;
        rotationToMinDistance = numberOfScan;
      }
      rotateCounter(1, cruiseSpeed, true);
      numberOfScan++;
    }
  }
  else
  {
    float angleToSensor = 0;
    switch (orientationToMinDistance)
    {
    case 0:
    case 1:
      angleToSensor = 0;
      break;
    case 2:
      angleToSensor = -90;
      break;
    case 3:
      angleToSensor = 90;
      break;
    default:
      break;
    }
    const float longAngle = static_cast<float>(rotationToMinDistance) / stepsPerDeg + angleToSensor;
    const PolarVector vectorToBlip =
    { .angle = shortenAngle(longAngle), .length = minDistance };
    Serial.printf(
                  "Found blip at %imm, with sensor %u and scan %u/%u resulting in long angle %f\n",
                  minDistance,
                  orientationToMinDistance + 1,
                  rotationToMinDistance,
                  maxNumberOfScans,
                  longAngle);
    return new_s(AligningToWall(vectorToBlip));
  }
  return this;
}

FollowingWall::FollowingWall() : subStateMachine(new PollingStateMachine(new FollowingWallState1()))
{
  PRINT_CHECKPOINT();
}

FollowingWall::~FollowingWall()
{
  delete subStateMachine;
}

PollingStateMachine::State* FollowingWall::operation()
{
  subStateMachine->loop();
  return this;// TODO
}

void Bearing::begin()
{
  setForcedNextState(new Lost());
}

void Bearing::stop()
{
  setForcedNextState(new StopBearing());
}

StopBearing::StopBearing()
{
  PRINT_CHECKPOINT();
}

PollingStateMachine::State* StopBearing::operation()
{
  return this;
}
