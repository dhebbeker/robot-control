#include "PollingStateMachine.hpp"
#include "arduino_helpers.hpp"

PollingStateMachine::PollingStateMachine(State* const startState) : forcedNextState(nullptr), currentState(startState)
{
}

PollingStateMachine::~PollingStateMachine()
{
  delete currentState;
}

void PollingStateMachine::loop()
{
  State *const newState = (forcedNextState ? forcedNextState : currentState->operation());
  if(forcedNextState) forcedNextState=nullptr;
  if (newState != currentState)
  {
    delete currentState;
    currentState = newState;
  }
}

void PollingStateMachine::setForcedNextState(State * const forcedNextState)
{
  this->forcedNextState = forcedNextState;
}
