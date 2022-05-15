#pragma once

class PollingStateMachine
{
public:

  /**
   * Execute one cycle of the state machine.
   */
  void loop();

  class State
  {
    friend PollingStateMachine;
  protected:
    State() = default;
  private:
    /**
     * \returns the next state (may be `this`)
     */
    virtual State* operation() = 0;
  protected:
    virtual ~State() = default;
  };

  ~PollingStateMachine();

  PollingStateMachine(State* const startState);
  void setForcedNextState(State * const forcedNextState);

private:
  State* forcedNextState;
  State* currentState;
};

