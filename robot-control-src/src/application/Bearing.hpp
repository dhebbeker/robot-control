#ifndef APPLICATION_BEARING_HPP_
#define APPLICATION_BEARING_HPP_

#include "Drives.hpp"
#include "board.hpp"
#include "../utils/PollingStateMachine.hpp"
#include "../utils/Debug.hpp"
#include <queue>
#include <limits>

#define PRINT_CHECKPOINT() DEBUG_MSG_DEBUG("Passing at " __FILE__ ":%u", __LINE__)

struct PolarVector
{
  float angle; //!< in degrees
  Distance length; //!< in mm
};

float shortenAngle(const float& angle);

class Bearing : public PollingStateMachine
{
public:
  Bearing();
  void begin();
  void stop();
};

class StopBearing : public PollingStateMachine::State
{
public:
  StopBearing();
private:
  virtual PollingStateMachine::State* operation() override;
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

template<class CreatorForNextState>
class Driving: public PollingStateMachine::State
{
private:
  DriveOrders driveOrders;
  bool rotated = false;
  CreatorForNextState creatorForNextState;
public:
  Driving(const DriveOrders &orders, CreatorForNextState creator) :
      driveOrders(orders), creatorForNextState(creator)
  {
    using ReturnTypeOfCreator = decltype(creator());
    static_assert(std::is_pointer<ReturnTypeOfCreator>::value, "creator must return a pointer");
    static_assert(
        std::is_base_of<PollingStateMachine::State, typename std::remove_pointer<ReturnTypeOfCreator>::type>::value,
        "creator must create an object which has B as PollingStateMachine::State");
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
          DEBUG_MSG_VERBOSE("Take order to rotate by %f degrees", currentOrder.angle);
          drives::rotate(currentOrder.angle, drives::cruiseSpeed);
          rotated = true;
        }
        else
        {
          DEBUG_MSG_VERBOSE("Take order to drive by %imm", currentOrder.length);
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
        return creatorForNextState();
      }
      else
      {
        // wait for last command to be finished
      }
    }
    return this;
  }
};

/**
 * Creates a new object of the class template Driving and applies automatic template parameter deduction.
 *
 * This is a wrapper around the constructor such that the template parameter of the object of the resulting
 * class can be deduced automatically from the argument given.
 *
 * @tparam CreatorForNextStateType template parameter for class template Driving
 * @param orders driving orders passed to constructor
 * @param creator forwarding call wrapper for the constructor of the next state
 * @return pointer to object created with `new`
 */
template< typename CreatorForNextStateType >
Driving<CreatorForNextStateType>* newDriver(const DriveOrders& orders, CreatorForNextStateType creator)
{
  return new Driving<CreatorForNextStateType>(orders, creator);
}

class FollowingWall : public PollingStateMachine::State
{
public:
  FollowingWall();
  virtual ~FollowingWall();
  virtual PollingStateMachine::State* operation() override;
  constexpr static Distance targetDistanceToWall = 100; //!< [mm]
private:
  PollingStateMachine* subStateMachine;
};

#endif /* APPLICATION_BEARING_HPP_ */
