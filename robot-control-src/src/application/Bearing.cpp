#include "Bearing.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "../utils/array.hpp"
#include "../utils/arduino_helpers.hpp"
#include <cmath>

using namespace drives;

float shortenAngle(const float& angle)
{
  const std::int8_t sign = angle < 0 ? -1 : 1;
  return (std::fmod(std::abs(angle) + 180, 360.0) - 180) * sign;
}

Bearing::Bearing() :
    state(new Lost(*this))
{

}

Bearing::~Bearing()
{
  delete state;
}

void Bearing::loop()
{
  state->operation();
}

void Bearing::setState(State *const nextState)
{
  delete state;
  state = nextState;
}

AligningToWall::AligningToWall(Bearing &context, const PolarVector vectorToWall) :
    Bearing::State(context), vectorToWall(vectorToWall)
{
  Serial.printf("Vector to wall points to %f°, %imm\n", vectorToWall.angle, vectorToWall.length);
}

void AligningToWall::operation()
{
  const DriveOrders newOrders({ vectorToWall });
  context.setState(new Driving<FollowingWall>(context, newOrders));
}

Bearing::State::State(Bearing &context) :
    context(context)
{
}

Bearing::State::~State()
{
}

Lost::Lost(Bearing &context) : Bearing::State(context)
{
}

void Lost::operation()
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
    context.setState(new AligningToWall(context, vectorToBlip));
  }
}

FollowingWall::FollowingWall(Bearing &context) : Bearing::State(context)
{
}

void FollowingWall::operation()
{
  // TODO
}
