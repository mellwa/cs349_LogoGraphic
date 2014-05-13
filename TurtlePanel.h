#ifndef _TURTLEPANEL_H_
#define _TURTLEPANEL_H_

#include "Panel.h"
#include "AffineTransform.h"
#include "Timer.h"
#include "TimerListener.h"
#include "TurtleGraphics.h"

using namespace cs349;

namespace cs349 {
  /**
   * A Panel is simply a Component which contains other Components. It
   * has no visual appearance of its own.
   */
  class TurtlePanel : public Panel,TimerListener{
  private:
      string name;
      TurtleGraphics* turtle;
  protected:
    /**
     * A no-op method
     */
    virtual void PaintComponent(Graphics* g);
  public:
    /**
     * A no-op constructor
     */
    TurtlePanel(const string & name, TurtleGraphics* t);
      virtual void HandleTimerEvent(const TimerEvent & e);
  };
}

#endif /* _PANEL_H_ */
