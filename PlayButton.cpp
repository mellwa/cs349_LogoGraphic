// Author: Michael Terry
#include "Button.h"
#include "PlayButton.h"
#include "XWindow.h"
#include "Logging.h"
#include "AffineTransform.h"
#include "XApplication.h"

#include <algorithm>

using namespace cs349;

PlayButton::PlayButton(const string & name, const string & label, Timer* t) : Button(name,label)
{
    // Perform any initialization needed here
    this->timer = t;
    this->press_in = false;
    // TODO CS349
}

void PlayButton::PaintComponent(Graphics* g) {
    // TODO CS349
    if(this->press_in && this->in_here){
        g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
        g->FillRect(this->GetBounds());
        g->DrawRect(this->GetBounds());
        g->SetForegroundColor(XApplication::GetInstance()->GetBlackColor());
    }
    else{
        g->SetForegroundColor(XApplication::GetInstance()->GetBlackColor());
        g->FillRect(this->GetBounds());
        g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
        g->DrawRect(this->GetBounds());
    }
    g->SetBackgroundColor(XApplication::GetInstance()->GetWhiteColor());
    AffineTransform original = g->GetTransform();
    Point p = original * Point(this->GetBounds().x,this->GetBounds().y);
    AffineTransform trans = original * original.MakeTranslation(this->GetBounds().x, this->GetBounds().y);
    g->SetTransform(trans);
    if(this->timer->IsRunning()){
        g->DrawRect(Rectangle(5,5,5,20));
        g->DrawRect(Rectangle(20,5,5,20));
    }
    else{
        g->DrawLine(Point(5,3), Point(5, 27));
        g->DrawLine(Point(5,3), Point(25,15));
        g->DrawLine(Point(5,27), Point(25,15));
    }
    this->real_x = p.x;
    this->real_y = p.y;
    g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
}

// TODO CS349: Implement any other methods needed here
// TODO CS349


void PlayButton::SetTimer(Timer* t){
    this->timer = t;
}






