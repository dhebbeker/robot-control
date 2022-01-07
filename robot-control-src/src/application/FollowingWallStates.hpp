#pragma once

#include "../utils/PollingStateMachine.hpp"
#include "Bearing.hpp"

/**
 * Measures the distance to the wall at the right side ("c") and moves to the next point.
 */
class FollowingWallState1 : public PollingStateMachine::State
{
public:
  FollowingWallState1() { PRINT_CHECKPOINT(); }
private:
  virtual PollingStateMachine::State* operation() override;
};

/**
 * Measures the distance to the wall at the right side ("f"), turns and moves to the next point.
 */
class FollowingWallState2 : public PollingStateMachine::State
{
public:
  FollowingWallState2() { PRINT_CHECKPOINT(); }
private:
  virtual PollingStateMachine::State* operation() override;
};
