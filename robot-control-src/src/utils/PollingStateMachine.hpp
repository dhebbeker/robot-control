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

protected:
  PollingStateMachine(State* const startState);

private:
  State* currentState;
};

