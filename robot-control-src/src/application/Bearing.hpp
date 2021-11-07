#ifndef APPLICATION_BEARING_HPP_
#define APPLICATION_BEARING_HPP_

#include "Drives.hpp"
#include "board.hpp"
#include <queue>

struct PolarVector
{
  float angle; //!< in degrees
  Distance length; //!< in mm
};

class Bearing
{
public:
  Bearing();
  ~Bearing();

  /**
   * Execute one cycle of the state machine.
   */
  void loop();

  class State
  {
  public:
    State(Bearing &context);
    virtual ~State();
    virtual void operation() = 0;
  protected:
    Bearing &context;
  };

  void setState(State *const nextState);

private:
  State *state;
};

class Lost: public Bearing::State
{
private:
  static constexpr drives::Counter maxNumberOfScans = 360 * drives::stepsPerDeg;
  drives::Counter numberOfScan = 0;
  board::Distance minDistance = std::numeric_limits<board::Distance>::max();
  drives::Counter rotationToMinDistance = 0; //!< relates to the rotation counter
  drives::Counter orientationToMinDistance = 0; //!< relates to the orientation of the sensor
  bool foundBlip = false;
public:
  Lost(Bearing &context);
  virtual void operation() override;
};

class AligningToWall: public Bearing::State
{
private:
  const PolarVector vectorToWall;
public:
  AligningToWall(Bearing &context, const PolarVector vectorToWall);
  virtual void operation() override;
};

using DriveOrders =std::queue<PolarVector>;

template<class OldState>
class Driving: public Bearing::State
{
private:
  using NextState = typename std::remove_reference<OldState>::type;
  DriveOrders driveOrders;
  bool rotated = false;
public:
  Driving(Bearing &context, const DriveOrders &orders) :
      Bearing::State(context), driveOrders(orders)
  {
  }
  virtual void operation() override
  {
    if (!driveOrders.empty())
    {
      if (drives::isIdle() && !board::isBumperPressed())
      {
        const auto currentOrder = driveOrders.front();
        if (!rotated)
        {
          drives::rotate(currentOrder.angle, drives::cruiseSpeed);
          rotated = true;
        }
        else
        {
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
        context.setState(new NextState(context));
      }
      else
      {
        // wait for last command to be finished
      }
    }
  }
};

class FollowingWall : public Bearing::State
{
public:
  FollowingWall(Bearing& context);
  virtual void operation() override;
};

#endif /* APPLICATION_BEARING_HPP_ */
