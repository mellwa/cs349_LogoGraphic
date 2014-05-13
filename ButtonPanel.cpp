#include "ButtonPanel.h"

using namespace cs349;

ButtonPanel::ButtonPanel(const string & name)
  : Panel(name)
{
    this->name = name;
}

void ButtonPanel::PaintComponent(Graphics* g) {
    AffineTransform trans = g->GetTransform();
    trans = trans * trans.MakeTranslation(310, 320);;
    g->SetTransform(trans);
    
    g->DrawRect(this->GetBounds());
}
