#include "FollowingWallStates.hpp"
#include "Bearing.hpp"
#include "board.hpp"
#include "../utils/numeric.hpp"
#include "../utils/lazy_creation.hpp"

using FP = double;

static constexpr std::size_t maxNumberMeasuringAttempts = 10;
static constexpr Distance minDistanceBetweenPoints = 80; /*!< Distance between points P1 and P2 [mm] */
static constexpr Distance maxTravelDistance = FollowingWall::targetDistanceToWall * 2;

static void checkFront()
{
  const Distance distanceFront = board::getDistances()[board::DistanceSensorIndex::front_right];
  // TODO
}

FollowingWallState1::FollowingWallState1(): distanceToNextPoint(minDistanceBetweenPoints) { PRINT_CHECKPOINT(); }

PollingStateMachine::State* FollowingWallState1::operation()
{
  Distance distanceRight = board::distanceErrorValue;
  const bool distanceMeasured = board::retrieveSensorStatusOrError(board::DistanceSensorIndex::right, distanceRight, maxNumberMeasuringAttempts);
  if(distanceMeasured)
  {
    /* drive to P2 */
    const DriveOrders newOrders(
    {
    { .angle = 0, .length = distanceToNextPoint }, });
    return newDriver(newOrders, createCreatorForNewObject<FollowingWallState2>(distanceToNextPoint, distanceRight));
  }
  else
  {
    return new Lost();
  }
}

#define PRINT_NUMBER( x ) DEBUG_MSG_VERBOSE("Number %s = %f", #x, x)

PolarVector FollowingWallState2::calculateVectorToNextPoint(const Distance distanceToWallAtCurrentPoint,
    const Distance distanceToLastPoint, const Distance distanceToWallAtLastPoint)
{
  const FP alpha = std::atan(static_cast<FP>(distanceToWallAtCurrentPoint-distanceToWallAtLastPoint)/static_cast<FP>(distanceToLastPoint));
  PRINT_NUMBER(alpha);
  const FP g = distanceToWallAtCurrentPoint * std::cos(alpha);
  PRINT_NUMBER(g);
  // if the distance to the wall (g) is too big, the travel distance must be increased
  FP distanceToNextPoint = std::max(static_cast<FP>(minDistanceBetweenPoints), g - FollowingWall::targetDistanceToWall);
  PRINT_NUMBER(distanceToNextPoint);
  const FP h = g - FollowingWall::targetDistanceToWall;
  PRINT_NUMBER(h);
  const FP w3 = std::acos(h / distanceToNextPoint);
  PRINT_NUMBER(w3);
  constexpr FP rightAngle = numbers::pi/2.0;
  PRINT_NUMBER(rightAngle);
  const FP beta = radToDeg(rightAngle + alpha - w3);
  PRINT_NUMBER(beta);

  // limit the distance to travel
  distanceToNextPoint = std::min(distanceToNextPoint, static_cast<FP>(maxTravelDistance));
  PRINT_NUMBER(distanceToNextPoint);

  return { .angle = beta, .length = distanceToNextPoint };
}

PollingStateMachine::State* FollowingWallState2::operation()
{
  Distance distanceRight = board::distanceErrorValue;
  const bool distanceMeasured = board::retrieveSensorStatusOrError(board::DistanceSensorIndex::right, distanceRight, maxNumberMeasuringAttempts);
  if(distanceMeasured)
  {
    const PolarVector vectorToNextPoint = calculateVectorToNextPoint(distanceRight, distanceFromLastPoint, distanceToWallAtLastPoint);

    /* drive to next P1 */
    const DriveOrders newOrders(
    {
    { .angle = vectorToNextPoint.angle, .length = 0 /* use length 0 because we have to measure after turning and before proceeding */ }, });
    return newDriver(newOrders, createCreatorForNewObject<FollowingWallState1>(vectorToNextPoint.length));
  }
  else
  {
    return new Lost();
  }
}
