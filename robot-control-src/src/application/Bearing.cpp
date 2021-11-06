#include "Bearing.hpp"
#include "board.hpp"
#include "Drives.hpp"
#include "../utils/array.hpp"

using namespace drives;

class Lost: public Bearing::State
{
private:
  static constexpr Counter maxNumberOfScans = 360 * stepsPerDeg;
  Counter numberOfScan = 0;
  board::Distance minDistance = std::numeric_limits<board::Distance>::max();
  Counter rotationToMinDistance = 0; //!< relates to the rotation counter
  Counter orientationToMinDistance = 0; //!< relates to the orientation of the sensor
  bool foundBlip = false;
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
        board::Distance currentMinDistance = std::numeric_limits<board::Distance>::max();
        Counter currentMinOrientation = 0;
        const auto& distances = board::getDistances();

        // check the shortest distance of all sensors
        for(Counter i=0; i<size(distances); i++)
        {
          if(distances[i] < currentMinDistance)
          {
            currentMinOrientation = i;
            currentMinDistance = distances[i];
          }
        }

        // compare with previous scans
        if(currentMinDistance < minDistance)
        {
          minDistance = currentMinDistance;
          orientationToMinDistance = currentMinOrientation;
          rotationToMinDistance = numberOfScan;
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
