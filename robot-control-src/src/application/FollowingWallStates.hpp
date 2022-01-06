#pragma once

#include "../utils/PollingStateMachine.hpp"

/**
 * Measures the distance to the wall at the right side ("c") and moves to the next point.
 */
class FollowingWallState1 : public PollingStateMachine::State
{
private:
  virtual PollingStateMachine::State* operation() override;
};

/**
 * Measures the distance to the wall at the right side ("f"), turns and moves to the next point.
 */
class FollowingWallState2 : public PollingStateMachine::State
{
private:
  virtual PollingStateMachine::State* operation() override;
};
