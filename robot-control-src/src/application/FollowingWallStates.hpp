#pragma once

#include "../utils/PollingStateMachine.hpp"
#include "../middleware/EnvironmentRecord.hpp"
#include <PID_v2.h>

class FollowingWallState1 : public PollingStateMachine::State
{
public:
  FollowingWallState1();
  FollowingWallState1(const Distance distanceAtLastPoint);
private:
  virtual PollingStateMachine::State* operation() override;
  Distance distanceAtLastPoint;
  const bool distanceValid;
  static PID_v2 controller;
  static bool controllerActive;
};
