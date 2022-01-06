#include "PollingStateMachine.hpp"

PollingStateMachine::PollingStateMachine(State* const startState) : currentState(startState)
{
}

PollingStateMachine::~PollingStateMachine()
{
  delete currentState;
}

void PollingStateMachine::loop()
{
  State * const newState = currentState->operation();
  if(newState != currentState)
  {
    delete currentState;
    currentState = newState;
  }
}
