/**
 * \page FollowWall Following a wall
 *
 * How this works:
 *
 * 1. FollowingWallState1() is called. The default distance to the next point is assumed.
 * 2. FollowingWallState1::operation() is called.
 *    The distance to the right is measured.
 *    The robot is ordered to drive straight the distance to the next point.
 * 3. FollowingWallState2(...) is called with the driven distance and the previously measured distance to the wall.
 * 4. FollowingWallState2::operation() is called.
 *    The distance to the right is measured.
 *    The vector to the next point is calculated using FollowingWallState2::calculateVectorToNextPoint().
 *    The robot is ordered to turn by the calculated vector.
 * 5. FollowingWallState1(...) is called with the distance of the calculated vector.
 * 6. go to 2.
 *
 */

#pragma once

#include "../utils/PollingStateMachine.hpp"
#include "Bearing.hpp"

/**
 * Measures the distance to the wall at the right side ("c") and moves to the next point.
 */
class FollowingWallState1 : public PollingStateMachine::State
{
public:
  FollowingWallState1();
  FollowingWallState1(const Distance distanceToNextPoint) : distanceToNextPoint(distanceToNextPoint) { PRINT_CHECKPOINT(); }
private:
  const Distance distanceToNextPoint;
  virtual PollingStateMachine::State* operation() override;
};

/**
 * Measures the distance to the wall at the right side ("f"), turns and moves to the next point.
 */
class FollowingWallState2 : public PollingStateMachine::State
{
public:
  FollowingWallState2(const Distance distanceFromLastPoint, const Distance distanceToWallAtLastPoint) :
      distanceFromLastPoint(distanceFromLastPoint), distanceToWallAtLastPoint(distanceToWallAtLastPoint)
  {
    PRINT_CHECKPOINT();
  }
private:
  const Distance distanceFromLastPoint;
  const Distance distanceToWallAtLastPoint;
  static PolarVector calculateVectorToNextPoint(const Distance distanceToWallAtCurrentPoint, const Distance distanceToLastPoint, const Distance distanceToWallAtLastPoint);
  virtual PollingStateMachine::State* operation() override;
};
