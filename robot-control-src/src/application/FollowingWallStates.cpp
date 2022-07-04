#include "FollowingWallStates.hpp"
#include "Bearing.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "../utils/numeric.hpp"
#include "../utils/lazy_creation.hpp"
#include <cmath>

#define PRINT_NUMBER( x ) DEBUG_MSG_VERBOSE("Number %s = %f", #x, static_cast<double>(x))

using FP = double;

static constexpr std::size_t maxNumberMeasuringAttempts = 10;
/**
 * Distance between points P1 and P2 [mm].
 *
 * This distance is calculated as multiple of the odoInterval length.
 * As only this can be actually achieved by the robot.
 */
static constexpr Distance minDistanceBetweenPoints = drives::roundDownToOdoIntervalMultiple(100);
static constexpr Distance maxTravelDistance = drives::roundDownToOdoIntervalMultiple(FollowingWall::targetDistanceToWall * 2);

FollowingWallState1::FollowingWallState1(): distanceToNextPoint(minDistanceBetweenPoints) { PRINT_CHECKPOINT(); }

template<typename T>
static PollingStateMachine::State* operateOnRightWall(const T operatorFunction)
{
  PolarVector distanceRight = { .angle=0, .length=board::distanceErrorValue };
  const bool distanceRightMeasured = board::retrieveSensorStatusOrError(
                                                                        board::DistanceSensorIndex::right,
                                                                        distanceRight,
                                                                        maxNumberMeasuringAttempts);
  if(distanceRightMeasured) DEBUG_MSG_VERBOSE("distance at the right was found: %i", distanceRight.length);
  else DEBUG_MSG_VERBOSE("distance at the right NOT was found.");
  PolarVector distanceFront = { .angle=0, .length=board::distanceErrorValue };
  const bool distanceFrontMeasured = board::retrieveSensorStatusOrError(
                                                                        board::DistanceSensorIndex::front_left,
                                                                        distanceFront,
                                                                        maxNumberMeasuringAttempts);
  if(distanceFrontMeasured) DEBUG_MSG_VERBOSE("distance at the front was found: %i", distanceFront.length);
  else DEBUG_MSG_VERBOSE("distance at the front NOT was found.");

  // in order to arrive at the target distance with max 45°, the vector to the target point
  // defines an isosceles triangle, thus the shorter sides can be calculated as (Pythagorean Theorem)
  constexpr auto maxDistanceFor45Deg = std::sqrt(std::pow(minDistanceBetweenPoints, 2) / 2.0);

  if (distanceFrontMeasured
      && (distanceFront.length
          < FollowingWall::targetDistanceToWall + maxDistanceFor45Deg
          || (distanceRightMeasured && distanceFront.length < distanceRight.length)))
  {
    if (distanceFront.length < FollowingWall::targetDistanceToWall)
    {
      const DriveOrders newOrders(
      {
      { .angle = 180, .length = FollowingWall::targetDistanceToWall - distanceFront.length },
      { .angle = 90, .length = 0 }, });
      return newDriver(newOrders, createCreatorForNewObject<FollowingWallState1>());

    }
    else
    {
      const DriveOrders newOrders(
      {
      { .angle = 0, .length = distanceFront.length - FollowingWall::targetDistanceToWall },
      { .angle = -90, .length = 0 }, });
      return newDriver(newOrders, createCreatorForNewObject<FollowingWallState1>());
    }
  }
  else if (distanceRightMeasured)
  {
    DEBUG_MSG_VERBOSE("try to follow wall");
    return operatorFunction(distanceRight.length);
  }
  else
  {
    return new_s(Lost());
  }
}

PollingStateMachine::State* FollowingWallState1::operation()
{
  return operateOnRightWall(
    [&](const Distance distanceRight)
  {
    PRINT_NUMBER(distanceRight);
    /* drive to P2 */
    const DriveOrders newOrders(
        {
          { .angle = 0, .length = distanceToNextPoint},});
    return newDriver(newOrders, createCreatorForNewObject<FollowingWallState2>(std::move(distanceToNextPoint), std::move(distanceRight)));
  });
}


FollowingWallState1::FollowingWallState1(const Distance _distanceToNextPoint)
    : distanceToNextPoint(_distanceToNextPoint) {
  PRINT_CHECKPOINT();
  PRINT_NUMBER(distanceToNextPoint);
}

PolarVector FollowingWallState2::calculateVectorToNextPoint(const Distance distanceToWallAtCurrentPoint,
    const Distance distanceToLastPoint, const Distance distanceToWallAtLastPoint)
{
  PRINT_NUMBER(distanceToWallAtCurrentPoint);
  PRINT_NUMBER(distanceToLastPoint);
  PRINT_NUMBER(distanceToWallAtLastPoint);
  const FP alpha = std::atan(static_cast<FP>(distanceToWallAtCurrentPoint-distanceToWallAtLastPoint)/static_cast<FP>(distanceToLastPoint));
  PRINT_NUMBER(alpha);
  const FP g = distanceToWallAtCurrentPoint * std::cos(alpha);
  PRINT_NUMBER(g);
  yield();

  const FP h = g - FollowingWall::targetDistanceToWall;
  PRINT_NUMBER(h);

  const FP _minDistanceBetweenPoints = static_cast<FP>(minDistanceBetweenPoints);
  PRINT_NUMBER(_minDistanceBetweenPoints);
  const FP distanceToLine = std::abs(h);
  PRINT_NUMBER(distanceToLine);
  // if the distance to the wall (g) is too big or too small, that is the distance to line to big, the travel distance must be increased
  FP distanceToNextPoint = std::max(_minDistanceBetweenPoints, distanceToLine);
  distanceToNextPoint = drives::roundDownToOdoIntervalMultiple(distanceToNextPoint);
  PRINT_NUMBER(distanceToNextPoint);
  yield();

  // if the distance the 

  const FP w3 = std::acos(drives::roundDownToOdoIntervalMultiple(h) / distanceToNextPoint);
  PRINT_NUMBER(w3);
  constexpr FP rightAngle = numbers::pi/2.0;
  PRINT_NUMBER(rightAngle);
  const FP beta = radToDeg(rightAngle + alpha - w3);
  PRINT_NUMBER(beta);
  yield();

  // limit the distance to travel
  distanceToNextPoint = std::min(distanceToNextPoint, static_cast<FP>(maxTravelDistance));
  PRINT_NUMBER(distanceToNextPoint);

  const PolarVector vectorToNextPoint = { .angle = beta, .length = distanceToNextPoint };
  PRINT_NUMBER(vectorToNextPoint.length);
  yield();
  return vectorToNextPoint;
}

PollingStateMachine::State* FollowingWallState2::operation()
{
  return operateOnRightWall(
    [&](const Distance distanceRight)
  {
    const PolarVector vectorToNextPoint = calculateVectorToNextPoint(distanceRight, distanceFromLastPoint, distanceToWallAtLastPoint);

    if(!std::isnan(vectorToNextPoint.angle))
    {
      /* drive to next P1 */
      const DriveOrders newOrders(
          {
            { .angle = vectorToNextPoint.angle, .length = 0 /* use length 0 because we have to measure after turning and before proceeding */},});
      PRINT_CHECKPOINT();
      PRINT_NUMBER(vectorToNextPoint.length);
      return static_cast<PollingStateMachine::State*>(newDriver(newOrders, createCreatorForNewObject<FollowingWallState1>(std::move(vectorToNextPoint.length))));
    }
    else
    { return static_cast<PollingStateMachine::State*>(new Lost());}
  });
}
