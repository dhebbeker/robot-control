#include "Bearing.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "../utils/array.hpp"
#include "../utils/arduino_helpers.hpp"
#include <cmath>

using namespace drives;

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
  Serial.printf("Vector to wall points to %f°, %imm", vectorToWall.angle, vectorToWall.length);
  const DriveOrders newOrders({ vectorToWall });
  context.setState(new Driving<FollowingWall>(context, newOrders));
}

void AligningToWall::operation()
{
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
    const Distance distanceToBlip = minDistance;
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
    const int angle = std::round(static_cast<float>(numberOfScan) / stepsPerDeg + angleToSensor);
    const std::int8_t sign = angle < 0 ? -1 : 1;
    const PolarVector vectorToBlip = { .angle = static_cast<float>((std::abs(angle) % 360)*sign), .length = minDistance};
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
