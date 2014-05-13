// A2 Unit Tests

#include "gtest/gtest.h"
#include "XApplication.h"
#include "XWindow.h"
#include "MainPanel.h"
#include "Button.h"
#include "Slider.h"
#include "TurtleGraphics.h"

using namespace cs349;

TEST(A2UnitTest, XApplicationCreateDestroy) {
  // Delete any app that is already lying around
  XApplication* app = XApplication::GetInstance();
  delete app;

  // Get new instance
  app = XApplication::GetInstance();
  EXPECT_EQ(0UL, XWindow::GetWindowList().size());
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  // Create a window and add an event to the event queue
  XWindow* win = new XWindow("Dummy", Rectangle(0, 0, 100, 100));
  EXPECT_EQ(1UL, XWindow::GetWindowList().size());
  app->GetEventQueue()->AddEventToQueue(new PaintEvent(win, Rectangle()));
  EXPECT_LT(0, app->GetEventQueue()->GetNumEventsInQueue());

  delete app;
  EXPECT_EQ(0UL, XWindow::GetWindowList().size());

  // Get a new instance, verify it's a new instance by checking that
  // the event queue is empty
  app = XApplication::GetInstance();
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());
}

TEST(A2UnitTest, XApplicationRunOnceTest) {
  // Delete any app that is already lying around
  XApplication* app = XApplication::GetInstance();
  delete app;

  // Get new instance
  app = XApplication::GetInstance();
  EXPECT_EQ(0UL, XWindow::GetWindowList().size());
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  // Create a window and add an event to the event queue
  XWindow* win = new XWindow("Dummy", Rectangle(0, 0, 100, 100));
  app->GetEventQueue()->AddEventToQueue(new PaintEvent(win, Rectangle()));

  int numEvents = app->GetEventQueue()->GetNumEventsInQueue();
  // Run one iteration of the event loop
  app->Run(true);
  EXPECT_EQ(numEvents-1, app->GetEventQueue()->GetNumEventsInQueue());
}

class ListenerCounter : public ActionListener, public ValueListener {
public:
  int numActionsPerformed;
  int numValuesChanged;
  string lastComponentName;

  ListenerCounter() {
    numActionsPerformed = 0;
    numValuesChanged = 0;
    lastComponentName = "";
  }
  virtual void ActionPerformed(Component* c) {
    numActionsPerformed++;
    lastComponentName = c->GetName();
  }
  virtual void ValueChanged(Component* c) {
    numValuesChanged++;
    lastComponentName = c->GetName();
  }
};

void FlushEventQueue(XApplication* app);
void FlushEventQueue(XApplication* app) {
  const int MAX_ATTEMPTS_TO_CLEAR_QUEUE = 100;
  for (int i = 0; i < MAX_ATTEMPTS_TO_CLEAR_QUEUE; i++) {
    app->Run(true);
    if (app->GetEventQueue()->GetNumEventsInQueue() < 1) {
      break;
    }
  }
}

void SimulateClick(XApplication* app, XWindow* win, Point where);
void SimulateClick(XApplication* app, XWindow* win, Point where) {
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseDown, where));
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseUp, where));
}

void SimulateDrag(XApplication* app, XWindow* win, Point start, Point middle, Point end);
void SimulateDrag(XApplication* app, XWindow* win, Point start, Point middle, Point end) {
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseDown, start));
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseDrag, middle));
  app->GetEventQueue()->AddEventToQueue(new MouseEvent(win, MouseEvent::mouseUp, end));
}

TEST(A2UnitTest, ButtonTests) {
  // Delete any app that is already lying around
  XApplication* app = XApplication::GetInstance();
  delete app;

  // Get new instance
  app = XApplication::GetInstance();

  const int buttonSize = 100;
  XWindow* win = new XWindow("Dummy", Rectangle(0, 0, 2*buttonSize, buttonSize));
  Button* leftButton = new Button("LEFT", "Left");
  Button* rightButton = new Button("RIGHT", "Right");

  leftButton->SetBounds(Rectangle(0, 0, buttonSize, buttonSize));
  rightButton->SetBounds(Rectangle(buttonSize, 0, buttonSize, buttonSize));
  Point leftButtonCenter(buttonSize/2, buttonSize/2);
  Point rightButtonCenter(buttonSize + buttonSize/2, buttonSize/2);

  win->AddComponent(leftButton);
  win->AddComponent(rightButton);

  ListenerCounter counter;
  leftButton->AddActionListener(&counter);
  rightButton->AddActionListener(&counter);

  // Set the window to visible so it responds to clicks
  win->SetVisible(true);
  // Buttons that are not visible should not respond to clicks...
  leftButton->SetVisible(false);
  rightButton->SetVisible(false);


  // Flush the repaints
  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());
  
  EXPECT_EQ(0, counter.numActionsPerformed);
  EXPECT_EQ(string(""), counter.lastComponentName);


  SimulateClick(app, win, leftButtonCenter);
  SimulateClick(app, win, rightButtonCenter);
  EXPECT_EQ(4, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(0, counter.numActionsPerformed);
  EXPECT_EQ(string(""), counter.lastComponentName);


  // Now test with visible buttons...
  leftButton->SetVisible(true);
  rightButton->SetVisible(true);

  // Flush the repaints
  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  SimulateClick(app, win, leftButtonCenter);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(1, counter.numActionsPerformed);
  EXPECT_EQ(string("LEFT"), counter.lastComponentName);

  SimulateClick(app, win, rightButtonCenter);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(2, counter.numActionsPerformed);
  EXPECT_EQ(string("RIGHT"), counter.lastComponentName);


  // Test mouse down then out then in
  SimulateDrag(app, win, leftButtonCenter, rightButtonCenter, leftButtonCenter);
  EXPECT_EQ(3, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(3, counter.numActionsPerformed);
  EXPECT_EQ(string("LEFT"), counter.lastComponentName);


  // Test mouse down then out with button up on the outside
  SimulateDrag(app, win, leftButtonCenter, rightButtonCenter, rightButtonCenter);
  EXPECT_EQ(3, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(3, counter.numActionsPerformed);
  EXPECT_EQ(string("LEFT"), counter.lastComponentName);
}

TEST(A2UnitTest, SliderTests) {
  // Delete any app that is already lying around
  XApplication* app = XApplication::GetInstance();
  delete app;

  // Get new instance
  app = XApplication::GetInstance();

  const int sliderSize = 100;
  XWindow* win = new XWindow("Dummy", Rectangle(0, 0, 2*sliderSize, sliderSize));
  Slider* slider = new Slider("SLIDER", Slider::HORIZONTAL_SLIDER);

  slider->SetBounds(Rectangle(0, 0, sliderSize, sliderSize));
  Point sliderLeft(0, sliderSize/2);
  Point sliderRight(sliderSize-1, sliderSize/2); // Set right side to
                                                 // be one less than
                                                 // the bounds to be
                                                 // considered "in"
                                                 // the slider
  Point sliderCenter(sliderSize/2, sliderSize/2);

  win->AddComponent(slider);


  // Some basic tests before testing interaction
  slider->SetMinimum(0);
  slider->SetMaximum(100);
  slider->SetCurValue(0);

  EXPECT_EQ(0, slider->GetMinimum());
  EXPECT_EQ(100, slider->GetMaximum());
  EXPECT_EQ(0, slider->GetCurValue());

  slider->SetCurValue(97);
  EXPECT_EQ(97, slider->GetCurValue());

  slider->SetCurValue(-1);
  EXPECT_EQ(0, slider->GetCurValue());

  slider->SetCurValue(101);
  EXPECT_EQ(100, slider->GetCurValue());

  slider->SetCurValue(0);


  // Now test interaction
  ListenerCounter counter;
  slider->AddValueListener(&counter);

  EXPECT_EQ(0, counter.numValuesChanged);
  EXPECT_EQ(string(""), counter.lastComponentName);


  // Set the window to visible so it responds to clicks
  win->SetVisible(true);
  // Sliders that are not visible should not respond to clicks...
  slider->SetVisible(false);

  // Flush the repaints
  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());
  
  EXPECT_EQ(0, counter.numValuesChanged);
  EXPECT_EQ(string(""), counter.lastComponentName);


  SimulateClick(app, win, sliderCenter);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(0, counter.numValuesChanged);
  EXPECT_EQ(string(""), counter.lastComponentName);
  EXPECT_EQ(0, slider->GetCurValue());
  


  // Now test with visible slider...
  slider->SetVisible(true);

  // Flush the repaints
  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(0, slider->GetCurValue());

  SimulateClick(app, win, sliderCenter);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(1, counter.numValuesChanged);
  EXPECT_EQ(string("SLIDER"), counter.lastComponentName);
  EXPECT_EQ(50, slider->GetCurValue());


  SimulateClick(app, win, sliderLeft);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(2, counter.numValuesChanged);
  EXPECT_EQ(string("SLIDER"), counter.lastComponentName);
  EXPECT_EQ(0, slider->GetCurValue());


  SimulateClick(app, win, sliderRight);
  EXPECT_EQ(2, app->GetEventQueue()->GetNumEventsInQueue());

  FlushEventQueue(app);
  EXPECT_EQ(0, app->GetEventQueue()->GetNumEventsInQueue());

  EXPECT_EQ(3, counter.numValuesChanged);
  EXPECT_EQ(string("SLIDER"), counter.lastComponentName);
  EXPECT_EQ(100, slider->GetCurValue());
}

TEST(A2UnitTest, TurtleTests) {
  TurtleGraphics* turtle = new TurtleGraphics("TURTLE");

  ListenerCounter counter;
  turtle->AddValueListener(&counter);

  EXPECT_EQ(0, counter.numValuesChanged);
  EXPECT_EQ(string(""), counter.lastComponentName);

  turtle->BroadcastInstructionsChanged();
  
  EXPECT_EQ(1, counter.numValuesChanged);
  EXPECT_EQ(string("TURTLE"), counter.lastComponentName);
}


TEST(A2UnitTest, Demo) {
    bool in[100];
    for(int i = 0; i < 100; i ++){
        in[i] = false;
    }
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    unsigned long currentTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    XApplication* app = XApplication::GetInstance();
    XWindow* window = new XWindow("Turtle Interfaces Demo :P", Rectangle(50, 50, 500, 500));
    MainPanel* mainPanel = new MainPanel();
    
    // Get the main panel's preferred size
    Rectangle mainPanelBounds = mainPanel->GetBounds();
    
    // Set the window size to match the main panel's size
    // Important! Make sure you set the size of MainPanel to be what you
    // want it to be in its constructor!
    window->SetSize(mainPanelBounds.width, mainPanelBounds.height);
    
    window->AddComponent(mainPanel);
    
    mainPanel->SetVisible(true);
    window->SetVisible(true);
    XFlush(app->XApplication::GetInstance()->GetXDisplay());
    MouseEvent* e;
    MouseEvent* e2;
    EventQueue* queue = XApplication::GetInstance()->GetEventQueue();

    while(1){
        clock_gettime(CLOCK_REALTIME, &tp);
        currentTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
        queue->ProcessNextEvent();
        XFlush(app->XApplication::GetInstance()->GetXDisplay());
        
        if(currentTime-preTime > 30000){
            break;
        }
        if(currentTime-preTime > 29000 && !in[37]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(460, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[37] = true;
        }
        if(currentTime-preTime > 28500 && !in[36]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(460, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[36] = true;
        }
        if(currentTime-preTime > 28000 && !in[35]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(350, 170));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(350, 170));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[35] = true;
        }
        if(currentTime-preTime > 27500 && !in[34]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(430, 170));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(430, 170));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[34] = true;
        }
        if(currentTime-preTime > 27000 && !in[33]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(480, 170));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(480, 170));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[33] = true;
        }
        if(currentTime-preTime > 26500 && !in[32]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(320, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[32] = true;
        }
        if(currentTime-preTime > 26000 && !in[31]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(320, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[31] = true;
        }
        if(currentTime-preTime > 25300 && !in[30]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(450, 70));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(450, 70));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[30] = true;
        }
        if(currentTime-preTime > 24500 && !in[29]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(500, 70));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(500, 70));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[29] = true;
        }
        if(currentTime-preTime > 24000 && !in[28]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(430, 70));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(430, 70));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[28] = true;
        }
        if(currentTime-preTime > 23500 && !in[27]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(500, 70));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(500, 70));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[27] = true;
        }
        if(currentTime-preTime > 22500 && !in[26]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(325, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[26] = true;
        }
        if(currentTime-preTime > 22000 && !in[25]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(325, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[25] = true;
        }
        if(currentTime-preTime > 21000 && !in[24]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(350, 70));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(350, 70));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[24] = true;
        }
        if(currentTime-preTime > 19500 && !in[23]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(395, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[23] = true;
        }
        if(currentTime-preTime > 19000 && !in[22]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(395, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[22] = true;
        }
        if(currentTime-preTime > 18500 && !in[21]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(320, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[21] = true;
        }
        if(currentTime-preTime > 18000 && !in[20]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(320, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[20] = true;
        }
        if(currentTime-preTime > 15500 && !in[19]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(395, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[19] = true;
        }
        if(currentTime-preTime > 15000 && !in[18]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(395, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[18] = true;
        }
        if(currentTime-preTime > 14500 && !in[17]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(445, 70));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(445, 70));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[17] = true;
        }
        if(currentTime-preTime > 14000 && !in[16]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(400, 180));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(400, 180));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[16] = true;
        }
        if(currentTime-preTime > 13500 && !in[15]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(320, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[15] = true;
        }
        if(currentTime-preTime > 13000 && !in[14]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(320, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[14] = true;
        }
        if(currentTime-preTime > 12500 && !in[13]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(460, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[13] = true;
        }
        if(currentTime-preTime > 12000 && !in[12]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(460, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[12] = true;
        }
        if(currentTime-preTime > 11500 && !in[11]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(425, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[11] = true;
        }
        if(currentTime-preTime > 11000 && !in[10]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(425, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[10] = true;
        }
        if(currentTime-preTime > 10500 && !in[9]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(425, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[9] = true;
        }
        if(currentTime-preTime > 10000 && !in[8]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(425, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[8] = true;
        }
        if(currentTime-preTime > 9500 && !in[7]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(355, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[7] = true;
        }
        if(currentTime-preTime > 9000 && !in[6]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(355, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[6] = true;
        }
        if(currentTime-preTime > 8500 && !in[5]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(355, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[5] = true;
        }
        if(currentTime-preTime > 8000 && !in[4]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(355, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[4] = true;
        }
        if(currentTime-preTime > 6000 && !in[3]){
            
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(395, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[3] = true;
        }
        if(currentTime-preTime > 4000 && !in[2]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(395, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[2] = true;
        }
        if(currentTime-preTime > 2100 && !in[1]){
            e2 = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseUp, Point(395, 345));
            queue->AddEventToQueue(e2);
            queue->ProcessNextEvent();
            in[1] = true;
        }
        if(currentTime-preTime > 1500 && !in[0]){
            e = new MouseEvent(window->GetParentWindow(), MouseEvent::mouseDown, Point(395, 345));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            in[0] = true;
        }
        usleep(5000);
    }
    //queue->ClearEventQueue();
    //delete window;
}

