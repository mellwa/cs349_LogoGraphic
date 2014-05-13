// Author: Michael Terry
#ifndef _GOTOSTARTBUTTON_H_
#define _GOTOSTARTBUTTON_H_

#include "Button.h"
#include <vector>

using namespace std;

namespace cs349 {
    
    class GoToStartButton : public Button{
        private:
        string label;
        // TODO CS349
        int real_x;
        int real_y;
        protected:
        /**
         * The method that does the actual painting of the component once
         * the Graphics context object has been set up by the Paint
         * method in Component.
         */
        virtual void PaintComponent(Graphics* g);
        
        public:
        GoToStartButton(const string & name, const string & label);
        
        //bool HandleMouseEvent(const MouseEvent & e);
    };
}


#endif /* _BUTTON_H_ */
