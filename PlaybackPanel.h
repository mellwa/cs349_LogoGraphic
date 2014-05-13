#ifndef _PLAYBACKPANEL_H_
#define _PLAYBACKPANEL_H_

#include "Panel.h"

using namespace cs349;

namespace cs349 {
  /**
   * A Panel is simply a Component which contains other Components. It
   * has no visual appearance of its own.
   */
  class PlaybackPanel : public Panel{
  private:
      string name;
  protected:
    /**
     * A no-op method
     */
    virtual void PaintComponent(Graphics* g);
  public:
    /**
     * A no-op constructor
     */
    PlaybackPanel(const string & name);
  };
}

#endif /* _PANEL_H_ */
