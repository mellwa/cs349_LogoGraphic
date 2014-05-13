// Author: Michael Terry
#ifndef _GOTOENDBUTTON_H_
#define _GOTOENDBUTTON_H_

#include "Button.h"
#include <vector>

using namespace std;

namespace cs349 {

  class GoToEndButton : public Button{
  private:
    string label;
      int real_x;
      int real_y;
// TODO CS349
  protected:
    /**
     * The method that does the actual painting of the component once
     * the Graphics context object has been set up by the Paint
     * method in Component.
     */
    virtual void PaintComponent(Graphics* g);

  public:
      GoToEndButton(const string & name, const string & label);

    //bool HandleMouseEvent(const MouseEvent & e);
  };
}


#endif /* _BUTTON_H_ */
