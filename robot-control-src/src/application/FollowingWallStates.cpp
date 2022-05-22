#include "FollowingWallStates.hpp"
#include "Bearing.hpp"
#include "../utils/Debug.hpp"

#define PRINT_NUMBER( x ) DEBUG_MSG_VERBOSE("Number %s = %f", #x, static_cast<double>(x))

FollowingWallState1::FollowingWallState1()
{
  PRINT_CHECKPOINT();
}

PollingStateMachine::State* FollowingWallState1::operation()
{
  return this;
}
