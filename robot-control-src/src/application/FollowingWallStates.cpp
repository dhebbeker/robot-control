#include "FollowingWallStates.hpp"
#include "Bearing.hpp"
#include "../utils/Debug.hpp"
#include "board.hpp"

#define PRINT_NUMBER( x ) DEBUG_MSG_VERBOSE("Number %s = %f", #x, static_cast<double>(x))

// Specify the links and initial tuning parameters
constexpr double Kp = 2, Ki = 5, Kd = 1;
static PID_v2 FollowingWallState1::controller(Kp, Ki, Kd, PID::Direct);
static bool FollowingWallState1::controllerActive = false;

FollowingWallState1::FollowingWallState1() : distanceValid(board::retrieveSensorStatusOrError(DistanceSensorIndex::right, this->distanceAtLastPoint))
{
  PRINT_CHECKPOINT();

}

FollowingWallState1::FollowingWallState1(const Distance distanceAtLastPoint) : distanceAtLastPoint(distanceAtLastPoint)
{
  PRINT_CHECKPOINT();
}

/**
 * Do not control the output while turning.
 * Turning will potentially increase the perceived distance even though the actual distance is not increased.
 * @return
 */
PollingStateMachine::State* FollowingWallState1::operation()
{
  Distance currentDistance;
  const bool distanceValid = board::retrieveSensorStatusOrError(DistanceSensorIndex::right, currentDistance);

  if(!distanceValid) return new Lost();

  if(!controllerActive)
  {
    constexpr Distance targetDistance = 100; //< [mm]
    controller.Start(currentDistance, 0, targetDistance);
    controllerActive = true;
  }

  const Distance error = targetDistance - currentDistance;
  return this;
}
