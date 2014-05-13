#include "Panel.h"

using namespace cs349;

Panel::Panel(const string & name)
  : Component(name)
{
  ; // no-op
}

void Panel::PaintComponent(Graphics* g) {
  ; // no-op. We're just a container -- we don't paint anything.
}
