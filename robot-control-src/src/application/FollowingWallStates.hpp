#pragma once

#include "../utils/PollingStateMachine.hpp"

class FollowingWallState1 : public PollingStateMachine::State
{
public:
  FollowingWallState1();
private:
  virtual PollingStateMachine::State* operation() override;
};
