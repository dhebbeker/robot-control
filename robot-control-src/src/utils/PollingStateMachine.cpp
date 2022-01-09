#include "PollingStateMachine.hpp"
#include "arduino_helpers.hpp"

PollingStateMachine::PollingStateMachine(State* const startState) : currentState(startState)
{
}

PollingStateMachine::~PollingStateMachine()
{
  delete currentState;
}

void PollingStateMachine::loop()
{
  static bool waiting = false;
  if(!waiting)
    {
    State * const newState = currentState->operation();
    if(newState != currentState)
    {
      delete currentState;
      currentState = newState;
      waiting = true;
    }
  }
  if(waiting)
  {
    WAIT_AND_RETURN(10*1000);
    waiting = false;
  }
}
