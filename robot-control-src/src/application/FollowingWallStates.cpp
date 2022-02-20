#include "FollowingWallStates.hpp"
#include "Bearing.hpp"
#include "board.hpp"
#include "../utils/numeric.hpp"
#include "../utils/lazy_creation.hpp"

#define PRINT_NUMBER( x ) DEBUG_MSG_VERBOSE("Number %s = %f", #x, static_cast<double>(x))

using FP = double;

static constexpr std::size_t maxNumberMeasuringAttempts = 10;
static constexpr Distance minDistanceBetweenPoints = 80; /*!< Distance between points P1 and P2 [mm] */
static constexpr Distance maxTravelDistance = FollowingWall::targetDistanceToWall * 2;

static void checkFront()
{
  const Distance distanceFront = board::getDistances()[board::DistanceSensorIndex::front_right];
  // TODO if too close to wall in front, "do LOST"
}

FollowingWallState1::FollowingWallState1(): distanceToNextPoint(minDistanceBetweenPoints) { PRINT_CHECKPOINT(); }

PollingStateMachine::State* FollowingWallState1::operation()
{
  Distance distanceRight = board::distanceErrorValue;
  const bool distanceMeasured = board::retrieveSensorStatusOrError(board::DistanceSensorIndex::right, distanceRight, maxNumberMeasuringAttempts);
  if(distanceMeasured)
  {
    PRINT_NUMBER(distanceRight);
    /* drive to P2 */
    const DriveOrders newOrders(
    {
    { .angle = 0, .length = distanceToNextPoint }, });
    return newDriver(newOrders, createCreatorForNewObject<FollowingWallState2>(std::move(distanceToNextPoint), std::move(distanceRight)));
  }
  else
  {
    return new Lost();
  }
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

  const FP h = g - FollowingWall::targetDistanceToWall;
  PRINT_NUMBER(h);

  const FP _minDistanceBetweenPoints = static_cast<FP>(minDistanceBetweenPoints);
  PRINT_NUMBER(_minDistanceBetweenPoints);
  const FP distanceToLine = std::abs(h);
  PRINT_NUMBER(distanceToLine);
  // if the distance to the wall (g) is too big or too small, that is the distance to line to big, the travel distance must be increased
  FP distanceToNextPoint = std::max(_minDistanceBetweenPoints, distanceToLine);
  PRINT_NUMBER(distanceToNextPoint);

  // if the distance the 

  const FP w3 = std::acos(h / distanceToNextPoint);
  PRINT_NUMBER(w3);
  constexpr FP rightAngle = numbers::pi/2.0;
  PRINT_NUMBER(rightAngle);
  const FP beta = radToDeg(rightAngle + alpha - w3);
  PRINT_NUMBER(beta);

  // limit the distance to travel
  distanceToNextPoint = std::min(distanceToNextPoint, static_cast<FP>(maxTravelDistance));
  PRINT_NUMBER(distanceToNextPoint);

  const PolarVector vectorToNextPoint = { .angle = beta, .length = distanceToNextPoint };
  PRINT_NUMBER(vectorToNextPoint.length);
  return vectorToNextPoint;
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
    PRINT_CHECKPOINT();
    PRINT_NUMBER(vectorToNextPoint.length);
    return newDriver(newOrders, createCreatorForNewObject<FollowingWallState1>(std::move(vectorToNextPoint.length)));
  }
  else
  {
    return new Lost();
  }
}
