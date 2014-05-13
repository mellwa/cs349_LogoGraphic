// Author: Michael Terry
#include "Button.h"
#include "XWindow.h"
#include "Logging.h"
#include "XApplication.h"
#include "A2WidgetIdentifiers.h"

#include <algorithm>

using namespace cs349;

Button::Button(const string & name, const string & label)
  : Component(name)
{
  // Perform any initialization needed here
  this->label = label;
// TODO CS349
}

void Button::AddActionListener(ActionListener* l) {
// TODO CS349
    this->listeners.push_back(l);
}

void Button::RemoveActionListener(ActionListener* l) {
    //delete l;
    remove(listeners.begin(), listeners.end(), l);
}

void Button::NotifyListeners() {
    for (vector<ActionListener*>::iterator i = listeners.begin(); i != listeners.end(); i++) {
        (*i)->ActionPerformed(this);
    }
}

string Button::GetLabel() const {
  return label;
}

void Button::SetLabel(const string & label) {
  this->label = label;
}

void Button::PaintComponent(Graphics* g) {
// override in child class
}


bool Button::HandleMouseEvent(const MouseEvent & e){
    if((this->GetName() == PLAY_BUTTON_ID)||(this->GetName() == GO_TO_END_BUTTON_ID)||(this->GetName() == GO_TO_START_BUTTON_ID)||(this->GetName() == PREVIOUS_FRAME_BUTTON_ID)||(this->GetName() == NEXT_FRAME_BUTTON_ID)){
        int x = e.GetWhere().x - 310;
        int y = e.GetWhere().y - 320;
        Point *p = new Point(x,y);
        switch (e.GetType()) {
            case MouseEvent::mouseDown:
            if(this->IsPointInComponent(*p)){
                this->press_in = true;
                this->in_here = true;
                this->Repaint();
                return true;
            }
            else{
                this->press_in = false;
                this->in_here = false;
            }
            break;
            case MouseEvent::mouseUp:
            if(this->IsPointInComponent(*p)){
                if(this->press_in){
                    this->NotifyListeners();
                }
                this->in_here = true;
                this->press_in = false;
                this->Repaint();
                return true;
            }
            else{
                this->in_here = false;
                this->press_in = false;
            }
            break;
            case MouseEvent::mouseMove:
            if(this->IsPointInComponent(*p)){
                if(!this->in_here){
                    this->in_here = true;
                    this->Repaint();
                }
            }else{
                if(this->in_here){
                    this->in_here = false;
                    this->Repaint();
                }
            }
            return false;
            default:
            break;
        }
    }
    else{
        switch (e.GetType()) {
            case MouseEvent::mouseDown:
            if(this->IsPointInComponent(e.GetWhere())){
                this->press_in = true;
                return true;
            }
            break;
            case MouseEvent::mouseUp:
            if(this->IsPointInComponent(e.GetWhere())){
                if(this->press_in){
                    this->NotifyListeners();
                }
                this->press_in = false;
                return true;
            }
            break;
            default:
            break;
        }

    }
    return false;
}

