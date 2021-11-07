#ifndef APPLICATION_BEARING_HPP_
#define APPLICATION_BEARING_HPP_

#include "Drives.hpp"
#include "board.hpp"

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

#endif /* APPLICATION_BEARING_HPP_ */
