#include "FollowingWallStates.hpp"
#include "Bearing.hpp"
#include "board.hpp"
#include "../utils/numeric.hpp"

using FP = double;

static constexpr std::size_t maxNumberMeasuringAttempts = 10;
static constexpr Distance minDistanceBetweenPoints = 80; /*!< Distance between points P1 and P2 [mm] */
static FP distanceToNextPoint = minDistanceBetweenPoints;
static Distance distanceAtP1 = 0;

PollingStateMachine::State* FollowingWallState1::operation()
{
  Distance distanceRight = board::distanceErrorValue;
  const bool distanceMeasured = board::retrieveSensorStatusOrError(board::DistanceSensorIndex::right, distanceRight, maxNumberMeasuringAttempts);
  if(distanceMeasured)
  {
    distanceAtP1 = distanceRight;
    /* drive to P2 */
    const DriveOrders newOrders(
    {
    { .angle = 0, .length = distanceToNextPoint }, });
    return new Driving<FollowingWallState2>(newOrders);
  }
  else
  {
    return new Lost();
  }
}

#define PRINT_NUMBER( x ) Serial.printf("\nNumber %s = %f\n", #x, x)

PollingStateMachine::State* FollowingWallState2::operation()
{
  Distance distanceRight = board::distanceErrorValue;
  const bool distanceMeasured = board::retrieveSensorStatusOrError(board::DistanceSensorIndex::right, distanceRight, maxNumberMeasuringAttempts);
  if(distanceMeasured)
  {
    const Distance distanceAtP2 = distanceRight;
    const Distance distanceToLastPoint = distanceToNextPoint;

    /* calculate new orientation */
    const FP alpha = std::atan(static_cast<FP>(distanceAtP2-distanceAtP1)/static_cast<FP>(distanceToLastPoint));
    PRINT_NUMBER(alpha);
    const FP g = distanceAtP2 * std::cos(alpha);
    PRINT_NUMBER(g);
    // if the distance to the wall (g) is too big, the travel distance must be increased
    distanceToNextPoint = std::max(static_cast<FP>(minDistanceBetweenPoints), g - targetDistanceToWall);
    PRINT_NUMBER(distanceToNextPoint);
    const FP h = g - targetDistanceToWall;
    PRINT_NUMBER(h);
    const FP w3 = std::acos(h / distanceToNextPoint);
    PRINT_NUMBER(w3);
    constexpr FP rightAngle = std::asin(static_cast<FP>(1));
    PRINT_NUMBER(rightAngle);
    const FP beta = radToDeg(rightAngle + alpha - w3);
    PRINT_NUMBER(beta);

    // limit the distance to travel
    distanceToNextPoint = std::min(distanceToNextPoint, static_cast<FP>(targetDistanceToWall * 2));
    PRINT_NUMBER(distanceToNextPoint);

    /* drive to next P1 */
    const DriveOrders newOrders(
    {
    { .angle = beta, .length = 0 }, });
    return new Driving<FollowingWallState1>(newOrders);
  }
  else
  {
    return new Lost();
  }
}
