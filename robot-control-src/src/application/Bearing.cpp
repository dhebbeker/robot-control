#include "Bearing.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "FollowingWallStates.hpp"
#include "../utils/array.hpp"
#include "../utils/arduino_helpers.hpp"
#include <cmath>

using namespace drives;

float shortenAngle(const float& angle)
{
  const std::int8_t sign = angle < 0 ? -1 : 1;
  return (std::fmod(std::abs(angle) + 180, 360.0) - 180) * sign;
}

static constexpr Distance targetDistanceToWall = 100; //!< [mm]

Bearing::Bearing() :
    PollingStateMachine(new Lost())
{

}

AligningToWall::AligningToWall(const PolarVector vectorToWall) : vectorToWall(vectorToWall)
{
  Serial.printf("Vector to wall points to %f°, %imm\n", vectorToWall.angle, vectorToWall.length);
}

PollingStateMachine::State* AligningToWall::operation()
{
if (vectorToWall.length > targetDistanceToWall)
{
  /* drive towards the wall and turn left */
  const DriveOrders newOrders(
  {
  { .angle = vectorToWall.angle, .length = vectorToWall.length - targetDistanceToWall },
  { .angle = -90 } });
  return new Driving<FollowingWall>(newOrders);
}
else
{
  /* drive away from the wall and turn right */
  const DriveOrders newOrders(
  {
  { .angle = shortenAngle(vectorToWall.angle + 180), .length = targetDistanceToWall - vectorToWall.length },
  { .angle = 90 } });
  return new Driving<FollowingWall>(newOrders);

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
    return new AligningToWall(vectorToBlip);
  }
  return this;
}

FollowingWall::FollowingWall() : subStateMachine(new PollingStateMachine(new FollowingWallState1()))
{
}

FollowingWall::~FollowingWall()
{
  delete subStateMachine;
}

PollingStateMachine::State* FollowingWall::operation()
{
  // TODO
}
