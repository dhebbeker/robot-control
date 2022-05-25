#pragma once

#include "../utils/PollingStateMachine.hpp"
#include "../middleware/EnvironmentRecord.hpp"
#include <PID_v2.h>

class FollowingWallState1 : public PollingStateMachine::State
{
public:
  FollowingWallState1();
private:
  typedef decltype(((PID_v2*)nullptr)->Run(0)) ControllerOutput;
  inline static float reactionToDegree(const ControllerOutput reaction);
  virtual PollingStateMachine::State* operation() override;
};
