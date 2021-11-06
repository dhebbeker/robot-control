#include "Bearing.hpp"
#include "Drives.hpp"
#include "board.hpp"
#include "../utils/array.hpp"
#include <cstdint>
#include <limits>

class Lost: public Bearing::State
{
private:
  static constexpr drives::Counter maxNumberOfScans = 360 * drives::stepsPerDeg;
  drives::Counter numberOfScan = 0;
  board::Distance minDistance = std::numeric_limits<Distance>::max();
  drives::Counter minOrientation = 0;
public:
  Lost(Bearing &context) :
      Bearing::State(context)
  {
  }
  virtual void operation() override
  {
    if (numberOfScan<=maxNumberOfScans)
    {
      if(drives::LeftDrive::isIdle && drives::RightDrive::isIdle && !board::isBumperPressed())
      {
        // TODO measure
        const board::Distance currentMinDistance = 0; // TODO
        if(currentMinDistance < minDistance)
        {
          minDistance = currentMinDistance;
          minOrientation = numberOfScan;
        }
        drives::rotateCounter(1, drives::cruiseSpeed, true);
        numberOfScan++;
      }
    }
    else
    {

    }
  }
};

Bearing::Bearing() :
    state(new Lost(*this))
{

}

Bearing::~Bearing()
{
  delete state;
}

void Bearing::loop()
{
  state->operation();
}

void Bearing::setState(State *const nextState)
{
  delete state;
  state = nextState;
}
