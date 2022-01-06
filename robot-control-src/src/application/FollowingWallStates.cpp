#include "FollowingWallStates.hpp"
#include "Bearing.hpp"
#include "board.hpp"

static constexpr Distance distanceBetweenPoints = 80; /*!< Distance between points P1 and P2 [mm] */
static Distance distanceAtP1 = 0;

PollingStateMachine::State* FollowingWallState1::operation()
{
  const auto& distances = board::getDistances();
  const auto distanceRight = distances[board::DistanceSensorIndex::right];
  if(distanceRight != board::distanceErrorValue)
  {
    distanceAtP1 = distanceRight;
    /* drive to P2 */
    const DriveOrders newOrders(
    {
    { .angle = 0, .length = distanceBetweenPoints }, });
    return new Driving<FollowingWallState2>(newOrders);
  }
  else
  {
    return this;
  }
}

PollingStateMachine::State* FollowingWallState2::operation()
{
  const auto& distances = board::getDistances();
  const auto distanceRight = distances[board::DistanceSensorIndex::right];
  if(distanceRight != board::distanceErrorValue)
  {
    const Distance distanceAtP2 = distanceRight;

    /* calculate new orientation */
    using FP = double;
    const FP alpha = std::atan(static_cast<FP>(distanceAtP2-distanceAtP1)/static_cast<FP>(distanceBetweenPoints));
    const FP g = distanceAtP2 * std::cos(alpha);
    const FP h = g - targetDistanceToWall;
    const FP w3 = std::acos(h / distanceBetweenPoints);
    constexpr FP rightAngle = std::asin(static_cast<FP>(1));
    const FP beta = rightAngle + alpha - w3;

    /* drive to next P1 */
    const DriveOrders newOrders(
    {
    { .angle = beta, .length = distanceBetweenPoints }, });
    return new Driving<FollowingWallState1>(newOrders);
  }
  else
  {
    return this;
  }
}
