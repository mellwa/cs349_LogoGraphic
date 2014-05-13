#include "StepPanel.h"
#include "XApplication.h"

using namespace cs349;

StepPanel::StepPanel(const string & name)
  : Panel(name)
{
}

void StepPanel::PaintComponent(Graphics* g) {
    AffineTransform trans = g->GetTransform();
    trans = trans * trans.MakeTranslation(300, 120);
    g->SetTransform(trans);
    g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
    g->DrawText(Point(30,10), "Current Turtle Step: ");
    //g->DrawLine(Point(0,60), Point(100,60));
}
