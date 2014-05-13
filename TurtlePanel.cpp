#include "TurtlePanel.h"
#include "XApplication.h"
#include "TurtleGraphics.h"
#include "A2WidgetIdentifiers.h"
#include "AffineTransform.h"

using namespace cs349;
int i = 0;
TurtlePanel::TurtlePanel(const string & name, TurtleGraphics* t)
  : Panel(name)
{
    this->turtle = t;
    this->name = name;//

}

void TurtlePanel::PaintComponent(Graphics* g) {
    g->DrawText(Point(20, 20), "Turtle Graphic Output: ");
    AffineTransform trans = g->GetTransform();
    trans = trans * trans.MakeTranslation(20, 50);
    g->SetTransform(trans);//set transform
    //repaint
    
    g->SetForegroundColor(XApplication::GetInstance()->GetBlackColor());
    g->FillRect(this->GetBounds());
    g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
    
    
    g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
    g->DrawRect(this->GetBounds());
}

void TurtlePanel::HandleTimerEvent(const TimerEvent & e){
    this->Repaint();
}