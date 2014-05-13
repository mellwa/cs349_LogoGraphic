// Author: Michael Terry
#ifndef _PLAYBUTTON_H_
#define _PLAYBUTTON_H_

#include "Button.h"
#include "Timer.h"
#include <vector>

using namespace std;

namespace cs349 {
    class PlayButton : public Button{
        private:
        string label;
        int real_x;
        int real_y;
        Timer* timer;
        // TODO CS349
        protected:
        /**
         * The method that does the actual painting of the component once
         * the Graphics context object has been set up by the Paint
         * method in Component.
         */
        virtual void PaintComponent(Graphics* g);
        
        public:
        PlayButton(const string & name, const string & label, Timer* t);
        
        // TODO CS349: Add any other necessary methods or overrides here
        // TODO CS349
        //bool HandleMouseEvent(const MouseEvent & e);
        void SetTimer(Timer* t);
    };
}


#endif /* _BUTTON_H_ */
