#ifndef _STEPPANEL_H_
#define _STEPPANEL_H_

#include "Panel.h"

using namespace cs349;

namespace cs349 {
  /**
   * A Panel is simply a Component which contains other Components. It
   * has no visual appearance of its own.
   */
  class StepPanel : public Panel {
  protected:
    /**
     * A no-op method
     */
    virtual void PaintComponent(Graphics* g);
  public:
    /**
     * A no-op constructor
     */
    StepPanel(const string & name);
  };
}

#endif /* _PANEL_H_ */
