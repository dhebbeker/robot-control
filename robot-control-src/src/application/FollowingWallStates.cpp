#include "FollowingWallStates.hpp"
#include "Bearing.hpp"
#include "../utils/Debug.hpp"
#include "../utils/lazy_creation.hpp"
#include "board.hpp"

#define PRINT_NUMBER( x ) DEBUG_MSG_VERBOSE("Number %s = %f", #x, static_cast<double>(x))

FollowingWallState1::FollowingWallState1()
{
  PRINT_CHECKPOINT();
}

float FollowingWallState1::reactionToDegree(const ControllerOutput reaction)
{
  return reaction;
}

/**
 * Do not control the output while turning.
 * Turning will potentially increase the perceived distance even though the actual distance is not increased.
 * @return
 */
PollingStateMachine::State* FollowingWallState1::operation()
{
  Distance currentDistance;
  const bool distanceValid = board::retrieveSensorStatusOrError(board::DistanceSensorIndex::right, currentDistance);
  constexpr Distance distanceBetweenPoints = drives::odoIntervalLength;

  static bool controllerActive = false;
  // Specify the links and initial tuning parameters
  constexpr double Kp = 0.15, Ki = 0, Kd = 0;
  static PID_v2 controller(Kp, Ki, Kd, PID::Reverse);

  if(board::getDistances()[board::DistanceSensorIndex::front_left] < currentDistance)
  {
    const DriveOrders toNextStop({{ .angle = -90, .length = 0 }});
    controller.SetOutputLimits(0,0);
    controllerActive = false;
    return newDriver(toNextStop, createCreatorForNewObject<FollowingWallState1>());
  }

  if(!distanceValid) return new Lost();
  if(!controllerActive)
  {
    constexpr Distance targetDistance = 200; //< [mm]
    controller.Start(currentDistance, 0, targetDistance);
    controller.SetOutputLimits(-90, 90);
    controller.SetSampleTime(1); // force to compute every time Run() is called
    controllerActive = true;
  }

  PRINT_NUMBER(currentDistance);
  const ControllerOutput reaction = controller.Run(currentDistance);
  PRINT_NUMBER(reaction);
  const float angle = reactionToDegree(reaction);
  PRINT_NUMBER(angle);
  const DriveOrders toNextStop({{ .angle = angle, .length = distanceBetweenPoints }});
  return newDriver(toNextStop, createCreatorForNewObject<FollowingWallState1>());
}
