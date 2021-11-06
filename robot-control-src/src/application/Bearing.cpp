#include "Bearing.hpp"
#include "board.hpp"
#include "Drives.hpp"

using namespace drives;

class Lost: public Bearing::State
{
private:
  static constexpr Counter maxNumberOfScans = 360 * stepsPerDeg;
  Counter numberOfScan = 0;
  board::Distance minDistance = board::distanceErrorValue;
  Counter minOrientation = 0;
public:
  Lost(Bearing &context) :
      Bearing::State(context)
  {
  }
  virtual void operation() override
  {
    if (numberOfScan<=maxNumberOfScans)
    {
      if(isIdle() && !board::isBumperPressed())
      {
        // TODO measure
        const board::Distance currentMinDistance = 0; // TODO
        if(currentMinDistance < minDistance)
        {
          minDistance = currentMinDistance;
          minOrientation = numberOfScan;
        }
        rotateCounter(1, cruiseSpeed, true);
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
