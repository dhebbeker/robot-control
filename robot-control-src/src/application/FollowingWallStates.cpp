#include "FollowingWallStates.hpp"
#include "Bearing.hpp"
#include "../utils/Debug.hpp"

#define PRINT_NUMBER( x ) DEBUG_MSG_VERBOSE("Number %s = %f", #x, static_cast<double>(x))

FollowingWallState1::FollowingWallState1()
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
  return this;
}
