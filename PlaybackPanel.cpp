#include "PlaybackPanel.h"
#include "XApplication.h"
#include "AffineTransform.h"

using namespace cs349;

PlaybackPanel::PlaybackPanel(const string & name)
  : Panel(name)
{
  this->name = name; // no-op
}

void PlaybackPanel::PaintComponent(Graphics* g) {
    AffineTransform trans = g->GetTransform();
    trans = trans * trans.MakeTranslation(300,10);
    g->SetTransform(trans);
    g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
    g->DrawText(Point(30,10), "Playback Rate: ");
    //g->DrawLine(Point(0,50), Point(100,50));
}
