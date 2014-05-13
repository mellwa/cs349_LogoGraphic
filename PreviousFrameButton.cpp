// Author: Michael Terry
#include "Button.h"
#include "PreviousFrameButton.h"
#include "XWindow.h"
#include "Logging.h"
#include "XApplication.h"

#include <algorithm>

using namespace cs349;

PreviousFrameButton::PreviousFrameButton(const string & name, const string & label) : Button(name,label)
{
    // Perform any initialization needed here
    this->label = label;
    
    // TODO CS349
}

void PreviousFrameButton::PaintComponent(Graphics* g) {
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
    AffineTransform original = g->GetTransform();
    Point p = original * Point(this->GetBounds().x,this->GetBounds().y);
    AffineTransform trans = original * original.MakeTranslation(this->GetBounds().x, this->GetBounds().y);
    g->SetTransform(trans);
    vector<Point> vertices;
    vertices.push_back(Point(5,15));
    vertices.push_back(Point(25,27));
    vertices.push_back(Point(25,3));
    g->FillPolygon(vertices);
    this->real_x = p.x;
    this->real_y = p.y;
}


