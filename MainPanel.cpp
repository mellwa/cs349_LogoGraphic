#include "MainPanel.h"
#include "TurtleGraphics.h"
#include "TurtlePanel.h"
#include "Button.h"
#include "Slider.h"
#include "A2WidgetIdentifiers.h"
#include "XApplication.h"
#include "PlaybackPanel.h"
#include "StepPanel.h"
#include "ButtonPanel.h"
#include "PlayButton.h"
#include "ActionListener.h"
#include "Timer.h"
#include "GoToStartButton.h"
#include "GoToEndButton.h"
#include "PreviousFrameButton.h"
#include "NextFrameButton.h"
#include "Slider.h"

using namespace cs349;

Timer* tmptimer;
TurtleGraphics* t;
Slider *step;
Slider *playback;
long n = 0;
PlayButton* playbutton;

MainPanel::MainPanel()
  : Panel(MAIN_PANEL_ID)
{
  // TODO CS349: Create your entire interface here. You will need to
  // modify the provided code below to lay things out the way you like

  // Note the use of unique identifiers for each widget when they are
  // constructed. See A2WidgetIdentifiers.h and the assignment
  // specification for the identifiers you should use for each of the
  // components you create. These identifiers are used for unit tests
    // to easily identify components in the interactor tree.
    
    this->timer = new Timer(XApplication::GetInstance()->GetEventQueue(),100,true,this);
    
    TurtleGraphics* turtle = new TurtleGraphics(TURTLE_GRAPHICS_WIDGET_ID);
    TurtlePanel* turtleGraphicsPanel = new TurtlePanel(TURTLE_GRAPHICS_PANEL_ID,turtle);
    ButtonPanel* button_panel = new ButtonPanel(BUTTON_PANEL_ID);
    GoToStartButton* GoToStart_button = new GoToStartButton( GO_TO_START_BUTTON_ID,"go to start label");
    GoToEndButton* GoToEnd_button = new GoToEndButton(GO_TO_END_BUTTON_ID,"go to end label");
    PreviousFrameButton* Previous_button = new PreviousFrameButton(PREVIOUS_FRAME_BUTTON_ID, "previous button");
    NextFrameButton* Next_button = new NextFrameButton(NEXT_FRAME_BUTTON_ID, "next button");
    
    PlayButton* play_button = new PlayButton(PLAY_BUTTON_ID,"play label",this->timer);
    playbutton = play_button;
    
    t = turtle;
    button_panel->SetVisible(true);
    
    //playback panel
    PlaybackPanel *playbackpanel = new PlaybackPanel(PLAYBACK_PANEL_ID);
    playbackpanel->SetBounds(Rectangle(0,0,200,100));
    this->AddComponent(playbackpanel);
    playbackpanel->SetVisible(true);
    Slider* playBackSlider = new Slider(PLAYBACK_SLIDER_ID,Slider::HORIZONTAL_SLIDER);
    playBackSlider->SetBounds(Rectangle(0,20,200,80));
    playBackSlider->AddValueListener(this);
    playbackpanel->AddComponent(playBackSlider);
    playBackSlider->SetVisible(true);
    playback = playBackSlider;
    
    //Step panel
    StepPanel *steppanel = new StepPanel(CURRENT_TURTLE_STEP_PANEL_ID);
    steppanel->SetBounds(Rectangle(0,0,200,100));
    this->AddComponent(steppanel);
    steppanel->SetVisible(true);
    Slider* stepSlider = new Slider(CURRENT_TURTLE_STEP_SLIDER_ID,Slider::HORIZONTAL_SLIDER);
    stepSlider->SetBounds(Rectangle(0,20,200,80));
    stepSlider->AddValueListener(this);
    steppanel->AddComponent(stepSlider);
    stepSlider->SetVisible(true);
    step = stepSlider;
    
    //play button
    play_button->AddActionListener(this);
    play_button->SetVisible(true);
    //go to end button
    GoToEnd_button->AddActionListener(this);
    GoToEnd_button->SetVisible(true);
    //go to next button
    Next_button->AddActionListener(this);
    Next_button->SetVisible(true);
    //go to previous frame button
    Previous_button->AddActionListener(this);
    Previous_button->SetVisible(true);
    //go to start point button
    GoToStart_button->AddActionListener(this);
    GoToStart_button->SetVisible(true);
    
    this->SetBounds(Rectangle(0, 0, 500, 400));
    turtleGraphicsPanel->SetBounds(Rectangle(0, 0, 280, 280));
    button_panel->SetBounds(Rectangle(0,0,180,70));
    turtle->SetBounds(Rectangle(0, 30, 300, 300));
    turtle->AddValueListener(this);

    
    

    this->AddComponent(turtleGraphicsPanel);
    this->AddComponent(button_panel);

    turtle->SetVisible(true);
    turtleGraphicsPanel->SetVisible(true);

  // TODO CS349: Add other initialization code here
  // TODO CS349
    
    turtleGraphicsPanel->AddComponent(turtle);
    play_button->SetBounds(Rectangle(75,20,30,30));
    GoToStart_button->SetBounds(Rectangle(5,20,30,30));
    Previous_button->SetBounds(Rectangle(40,20,30,30));
    Next_button->SetBounds(Rectangle(110,20,30,30));
    GoToEnd_button->SetBounds(Rectangle(145,20,30,30));
    button_panel->AddComponent(play_button);
    button_panel->AddComponent(GoToStart_button);
    button_panel->AddComponent(GoToEnd_button);
    button_panel->AddComponent(Previous_button);
    button_panel->AddComponent(Next_button);
    
  // Some simple operations to show the turtle when starting up
  // This is an entirely random set of instructions
  // Once started up, try typing this into the console:
  // (turtle-star (find-turtle-graphics-widget) 300 23)
  // The 300 argument is the length of the edges of the star, in
  // pixels
  // The 23 argument is the number of points for the star
    

  turtle->RotateInDegrees(45);
  turtle->Forward(100, true);
  turtle->RotateInDegrees(135);
  turtle->Forward(65, true);
  turtle->RotateInDegrees(135);
  turtle->Forward(100, true);
  turtle->RotateInDegrees(150);
  turtle->Forward(100, true);
    stepSlider->SetMinimum(0);
    stepSlider->SetMaximum((int)turtle->GetNumSteps());
}

void MainPanel::HandleTimerEvent(const TimerEvent & e){
    if(n<t->GetNumSteps()){
        n++;
        step->SetCurValue((int)n);
        t->GoToStep(n);
        this->Repaint();
    }
    else{
        if(this->timer->IsRunning()){
            step->SetCurValue((int)n);
            this->timer->Stop();
            this->Repaint();
        }
    }
}

//---------------------------------------
//Handle all action performed events
//---------------------------------------
void MainPanel::ActionPerformed(Component* source){
    if(source->GetName() == PLAY_BUTTON_ID){
        if(!this->timer->IsRunning()){
            this->timer->Start();
            this->Repaint();
        }
        else{
            this->timer->Stop();
            this->Repaint();
        }
    }
    else if(source->GetName() == GO_TO_END_BUTTON_ID){
            n = t->GetNumSteps();
            t->GoToStep(t->GetNumSteps());
            step->SetCurValue((int)n);
            this->Repaint();
    }
    else if(source->GetName() == GO_TO_START_BUTTON_ID){
            n = 0;
            t->GoToStep(n);
            step->SetCurValue((int)n);
            this->Repaint();
    }
    else if(source->GetName() == PREVIOUS_FRAME_BUTTON_ID){
        if(!this->timer->IsRunning()){
            if(n>0){
            n--;
            }
            t->GoToStep(n);
            step->SetCurValue((int)n);
            this->Repaint();
        }
    }
    else if(source->GetName() == NEXT_FRAME_BUTTON_ID){
        if(!this->timer->IsRunning()){
            if(n<t->GetNumSteps())
            n++;
            t->GoToStep(n);
            step->SetCurValue((int)n);
            this->Repaint();
        }
    }
}

//---------------------------------------
//Handle all value changed events
//---------------------------------------
void MainPanel::ValueChanged(Component* source){
    if(source->GetName()==TURTLE_GRAPHICS_WIDGET_ID){
        step->SetMaximum((int)t->GetNumSteps());
        this->Repaint();
    }
    else if(source->GetName() == PLAYBACK_SLIDER_ID){
        if(!this->timer->IsRunning()){
            float f = 100;
            f /= playback->GetFloat();
            this->timer = new Timer(XApplication::GetInstance()->GetEventQueue(),f,true,this);
            playbutton->SetTimer(this->timer);
        }
        else{
            this->timer->Stop();
            float f = 100;
            f /= playback->GetFloat();
            this->timer = new Timer(XApplication::GetInstance()->GetEventQueue(),f,true,this);
            playbutton->SetTimer(this->timer);
            this->timer->Start();
        }
    }
    else if(source->GetName() == CURRENT_TURTLE_STEP_SLIDER_ID){
        n = step->GetCurValue();
        t->GoToStep(n);
        this->Repaint();
    }
    
}













