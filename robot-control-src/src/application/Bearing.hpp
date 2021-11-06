#ifndef APPLICATION_BEARING_HPP_
#define APPLICATION_BEARING_HPP_

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
    State(Bearing &context) :
        context(context)
    {
    }
    virtual ~State() {}
    virtual void operation() = 0;
  private:
      Bearing& context;
  };

  void setState(State* const nextState);

private:
  State* state;
};

#endif /* APPLICATION_BEARING_HPP_ */
