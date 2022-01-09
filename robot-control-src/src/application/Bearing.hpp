#ifndef APPLICATION_BEARING_HPP_
#define APPLICATION_BEARING_HPP_

#include "Drives.hpp"
#include "board.hpp"
#include "../utils/PollingStateMachine.hpp"
#include "../utils/Debug.hpp"
#include <queue>

#define PRINT_CHECKPOINT() DEBUG_MSG_DEBUG("\nPassing at " __FILE__ ":%u: %s\n", __LINE__, __func__)

struct PolarVector
{
  float angle; //!< in degrees
  Distance length; //!< in mm
};

constexpr Distance targetDistanceToWall = 100; //!< [mm]
float shortenAngle(const float& angle);

class Bearing : public PollingStateMachine
{
public:
  Bearing();
};

class Lost: public PollingStateMachine::State
{
private:
  static constexpr drives::Counter maxNumberOfScans = 360 * drives::stepsPerDeg;
  drives::Counter numberOfScan = 0;
  board::Distance minDistance = std::numeric_limits<board::Distance>::max();
  drives::Counter rotationToMinDistance = 0; //!< relates to the rotation counter
  drives::Counter orientationToMinDistance = 0; //!< relates to the orientation of the sensor
  bool foundBlip = false;
public:

  Lost() { PRINT_CHECKPOINT(); }

  /**
   * Checks if one of the distance sensors currently senses a shorter distance than previously found.
   *
   * Orders to rotate in case the scan is not complete.
   *
   * If scan is complete, the vector to the blip is calculated and passed to AligningToWall::AligningToWall().
   */
  virtual PollingStateMachine::State* operation() override;
};

class AligningToWall: public PollingStateMachine::State
{
private:
  const PolarVector vectorToWall;
public:
  AligningToWall(const PolarVector vectorToWall);

  /**
   * Orders to drive to blip and then follow the wall.
   */
  virtual PollingStateMachine::State* operation() override;
};

using DriveOrders =std::queue<PolarVector>;

template<class OldState>
class Driving: public PollingStateMachine::State
{
private:
  using NextState = typename std::remove_reference<OldState>::type;
  DriveOrders driveOrders;
  bool rotated = false;
public:
  Driving(const DriveOrders &orders) : driveOrders(orders)
  {
    PRINT_CHECKPOINT();
    Serial.printf("Taking %u driving orders.\n", driveOrders.size());
  }

  /**
   * Each drive order is executed in two steps:
   *
   *  1. Rotate
   *  2. Drive
   *
   * Orders are only executed if bumpers are free and drive idle.
   *
   * If all orders have been executed, the next state is created.
   */
  virtual PollingStateMachine::State* operation() override
  {
    if (!driveOrders.empty())
    {
      if (drives::isIdle() && !board::isBumperPressed())
      {
        const auto& currentOrder = driveOrders.front();
        if (!rotated)
        {
          Serial.printf("Take order to rotate by %f\n", currentOrder.angle);
          drives::rotate(currentOrder.angle, drives::cruiseSpeed);
          rotated = true;
        }
        else
        {
          Serial.printf("Take order to drive by %i\n", currentOrder.length);
          drives::drive(currentOrder.length, drives::cruiseSpeed, false);
          driveOrders.pop();
          rotated = false;
        }
      }
      else
      {
        // wait for idle drives and free bumpers
      }
    }
    else
    {
      if (drives::isIdle())
      {
        return new NextState();
      }
      else
      {
        // wait for last command to be finished
      }
    }
    return this;
  }
};

class FollowingWall : public PollingStateMachine::State
{
public:
  FollowingWall();
  virtual ~FollowingWall();
  virtual PollingStateMachine::State* operation() override;
private:
  PollingStateMachine* subStateMachine;
};

#endif /* APPLICATION_BEARING_HPP_ */
