#include "Slider.h"
#include "XWindow.h"
#include "Logging.h"
#include "XApplication.h"
#include "A2WidgetIdentifiers.h"
#include <math.h>
#include <algorithm>
#include <string>
#include <sstream>

using namespace cs349;
using namespace std;

Slider::Slider(const string & name, OrientationType orientation)
  : Component(name)
{
  lowerBound = 0;
  upperBound = 1;
  curValue = 0;
  lowerBoundLabel = "";
  upperBoundLabel = "";
  thumbLabel = "0";
  this->orientation = orientation;
    this->x = 30;
    this->y = 20;
    if(name == PLAYBACK_SLIDER_ID){
        lowerBoundLabel = "0.1X";
        upperBoundLabel = "10X";
        thumbLabel = "1X";
        this->y = 30;
        this->x = 85;
    }
    this->f = 1;
// TODO CS349
}

void Slider::AddValueListener(ValueListener* l) {
// TODO CS349
    this->listeners.push_back(l);
}

void Slider::RemoveValueListener(ValueListener* l) {
// TODO CS349
    remove(listeners.begin(), listeners.end(), l);
}

void Slider::NotifyListeners() {
    for (vector<ValueListener*>::iterator i = listeners.begin(); i != listeners.end(); i++) {
        (*i)->ValueChanged(this);
    }
}

int Slider::GetMinimum() const {
  return lowerBound;
}

int Slider::GetMaximum() const {
  return upperBound;
}

int Slider::GetCurValue() const {
  return curValue;
}

void Slider::SetMinimum(int minValue) {
// TODO CS349
    stringstream ss;
    this->lowerBound = minValue;
    ss<<minValue;
    ss>>this->lowerBoundLabel;
}

void Slider::SetMaximum(int maxValue) {
// TODO CS349
    this->upperBound = maxValue;
    stringstream ss;
    ss<<maxValue;
    ss>>this->upperBoundLabel;
}

void Slider::SetCurValue(int value) {
// TODO CS349
    if(value < 0) value = 0;
    if(value > this->upperBound) value = this->upperBound;
    this->curValue = value;
    stringstream ss;
    ss<<value;
    ss>>this->thumbLabel;
}

Slider::OrientationType Slider::GetOrientation() const {
  return orientation;
}

void Slider::SetOrientation(OrientationType orientation) {
  this->orientation = orientation;
  this->Repaint();
}

void Slider::PaintComponent(Graphics* g) {
// TODO CS349
    stringstream ss;
    string s;
    int tmp = 0;
    g->SetForegroundColor(XApplication::GetInstance()->GetBlackColor());
    g->FillRect(this->GetBounds());
    g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
 
    AffineTransform original = g->GetTransform();
    AffineTransform trans = original * original.MakeTranslation(this->GetBounds().x, this->GetBounds().y);
    g->SetTransform(trans);
    
    g->DrawLine(Point(30,40), Point(150,40));
    if(this->GetName()==CURRENT_TURTLE_STEP_SLIDER_ID){
        this->x = 30 + (110*this->curValue)/this->upperBound;
        g->FillRect(Rectangle(this->x,this->y,10,40));
        g->DrawText(Point(15,45), this->lowerBoundLabel);
        g->DrawText(Point(155,45), this->upperBoundLabel);
        g->DrawText(Point(this->x,this->y-5), this->thumbLabel);
    }
    else{
        this->f = this->x - 30;//calculate the fraction of speed
        this->f /= 110;
        this->f = this->f*2-1;
        this->f = pow(10, this->f);
        this->thumbLabel.clear();
        this->f = floor(this->f*100);
        this->f /= 100;
        tmp = this->f;
        if(this->f>1 && (this->f - tmp)>=0.95){
            ss<<(tmp+1);
        }else if(this->f>1 && (this->f - tmp)<=0.05){
            ss<<tmp;
        }
        else{
            ss<<this->f;
        }
        ss>>s;
        ss.clear();
        s.append("X");
        this->thumbLabel.append(s);
        s.clear();
        g->FillRect(Rectangle(this->x,this->y,10,20));
        g->DrawText(Point(15,15), this->lowerBoundLabel);
        g->DrawText(Point(155,15), this->upperBoundLabel);
        g->DrawText(Point(this->x,this->y-5), this->thumbLabel);
    }
}

string Slider::GetLowerBoundLabel() const {
  return lowerBoundLabel;
}

string Slider::GetUpperBoundLabel() const {
  return upperBoundLabel;
}

string Slider::GetThumbLabel() const {
  return thumbLabel;
}

void Slider::SetLowerBoundLabel(const string & label) {
  this->lowerBoundLabel = label;
  this->Repaint();
}

void Slider::SetUpperBoundLabel(const string & label) {
  this->upperBoundLabel = label;
  this->Repaint();
}

void Slider::SetThumbLabel(const string & label) {
  this->thumbLabel = label;
  this->Repaint();
}

bool Slider::HandleMouseEvent(const MouseEvent & e){
    if(this->GetName() == PLAYBACK_SLIDER_ID){
        switch (e.GetType()) {
            case MouseEvent::mouseDown:
            if(e.GetWhere().x>=330 && e.GetWhere().x<=450 && e.GetWhere().y>=60 && e.GetWhere().y<=80){
                this->x = e.GetWhere().x-305;
                if(this->x < 30) this->x=30;
                if(this->x > 140) this->x = 140;
                this->press_in = true;
                this->Repaint();
                this->NotifyListeners();
                return true;
            }
            break;
            
            case MouseEvent::mouseUp:
            if(e.GetWhere().x>=330 && e.GetWhere().x<=450 && e.GetWhere().y>=60 && e.GetWhere().y<=80){
                this->press_in = false;
                return true;
            }
            break;
            
            case MouseEvent::mouseMove://handle mouse move event
            if(this->press_in){
                this->x = e.GetWhere().x-305;
                if(this->x < 30) this->x=30;
                if(this->x > 140) this->x = 140;
                this->Repaint();
                this->NotifyListeners();
                return true;
            }
            break;
            default:
            break;
        }
    }
    else if(this->GetName() == CURRENT_TURTLE_STEP_SLIDER_ID){
        switch (e.GetType()) {
            case MouseEvent::mouseDown:
            if(e.GetWhere().x>=330 && e.GetWhere().x<=450 && e.GetWhere().y>=160 && e.GetWhere().y<=200){
                this->x = e.GetWhere().x-305;
                if(this->x < 30) this->x=30;
                if(this->x > 140) this->x = 140;
                this->press_in=true;
                this->SetCurValue((this->x-30)*this->upperBound/110);
                this->NotifyListeners();
                return true;
            }
            break;
            
            case MouseEvent::mouseUp:
            if(e.GetWhere().x>=330 && e.GetWhere().x<=450 && e.GetWhere().y>=160 && e.GetWhere().y<=200){
                this->press_in = false;
                return true;
            }
            break;
            
            case MouseEvent::mouseMove:
            if(this->press_in){
                this->x = e.GetWhere().x-305;
                if(this->x < 30) this->x=30;
                if(this->x > 140) this->x = 140;
                this->SetCurValue((this->x-30)*this->upperBound/110);
                this->NotifyListeners();
                return true;
            }
            break;
            default:
            break;
        }
    }
    else{
        switch (e.GetType()) {
            case MouseEvent::mouseDown:
            if(this->IsPointInComponent(e.GetWhere())){
                this->press_in=true;
                if(e.GetWhere().x>97){
                    this->SetCurValue(100);
                }
                else{
                    this->SetCurValue(e.GetWhere().x);
                }
                this->NotifyListeners();
                return true;
            }
            break;
            
            case MouseEvent::mouseUp:
            if(this->IsPointInComponent(e.GetWhere())){
                this->press_in = false;
                return true;
            }
            break;
            
            case MouseEvent::mouseMove:
            if(this->press_in){
                this->x = e.GetWhere().x-305;
                if(this->x < 30) this->x=30;
                if(this->x > 140) this->x = 140;
                this->SetCurValue((this->x-30)*this->upperBound/110);
                this->NotifyListeners();
                return true;
            }
            break;
            default:
            break;
        }
    }
    return false;
}

float Slider::GetFloat(){
    this->f = this->x - 30;
    this->f /= 110;
    this->f = this->f*2-1;
    this->f = pow(10, this->f);
    this->f = floor(this->f*100);
    this->f /= 100;
    return (float)this->f;
}













