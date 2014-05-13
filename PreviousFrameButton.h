// Author: Michael Terry
#ifndef _PREVIOUSFRAMEBUTTON_H_
#define _PREVIOUSFRAMEBUTTON_H_

#include "Button.h"
#include <vector>

using namespace std;

namespace cs349 {
    class PreviousFrameButton : public Button{
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
        PreviousFrameButton(const string & name, const string & label);
        
        //bool HandleMouseEvent(const MouseEvent & e);
    };
}


#endif /* _BUTTON_H_ */
