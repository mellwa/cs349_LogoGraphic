// Author: Michael Terry

#include "XApplication.h"

#include <X11/Xutil.h>
#include <unistd.h> // For usleep

#include "XWindow.h"
#include "SchemeInterpreter.h"
#include "Logging.h"
#include "MainPanel.h"

namespace cs349
{

  /**
   * This is a callback from X whenever it encounters a serious
   * error (such as when the user closes the window). You can
   * ignore it
   */
  int ErrorHandler(Display* display);

  XApplication* XApplication::s_Instance = NULL;

  int ErrorHandler(Display* display)
  {
    return XApplication::GetInstance()->HandleXError(display);
  }

  XApplication::XApplication()
  {
    eventQueue = new EventQueue();
    // Initialize the X display and get the default screen
    display = XOpenDisplay("");
    if (!display) {
      LOG_FATAL << "Error: Couldn't open X display";
      return;
    }
    screen = DefaultScreen(display);
    quit   = false;
      this->delaytime = 3000;
  }

  XApplication::~XApplication()
  {
    if (display != NULL) {
      vector<XWindow*> windows = XWindow::GetWindowList();
      for (vector<XWindow*>::iterator win = windows.begin(); win != windows.end(); win++) {
        delete (*win);
      }
      XCloseDisplay(display);
      display = NULL;
    }
    delete eventQueue;
    eventQueue = NULL;
    s_Instance = NULL;
  }

  XApplication* XApplication::GetInstance()
  {
    if (!XApplication::s_Instance) {
      s_Instance = new XApplication();
    }
    return s_Instance;
  }


  unsigned long XApplication::GetBlackColor() const
  {
    return BlackPixel(this->display, this->screen);
  }

  int XApplication::GetDefaultScreen() const
  {
    return this->screen;
  }

  EventQueue* XApplication::GetEventQueue()
  {
    return eventQueue;
  }

  void XApplication::SetEventQueue(EventQueue* eventQueue)
  {
    if (!eventQueue) {
      LOG_FATAL << "Error: Cannot pass in NULL eventQueue to XApplication::SetEventQueue";
      return;
    }
    delete this->eventQueue;
    this->eventQueue = eventQueue;
  }

  unsigned long XApplication::GetWhiteColor() const
  {
    return WhitePixel(this->display, this->screen);
  }

  Display* XApplication::GetXDisplay() const
  {
    return this->display;
  }

  int XApplication::HandleXError(Display* display)
  {
    /*
      This method is called whenever there is a serious X error. This
      method will be called when the user manually closes the window.
      You could clean up the event queue, release memory, and anything
      else needed before the application quits, because once this
      method returns, X will forcibly terminate the entire
      application.
    */

    // Shut down the Scheme interpreter so it cleanly exits readline.
    // Otherwise, it puts the terminal in a funky state
    scheme_interpreter_shutdown();
    return -1;
  }

  void XApplication::Quit()
  {
    quit = true;
  }


  void XApplication::CheckForXEvents() {

    /*
     * TODO for CS349 assignment:
     * Check if there is an X event pending. If so, package it up
     * using our own Event classes, and add it to the eventQueue.
     *
     * See the documentation at
     * http://tronche.com/gui/x/xlib/events/types.html to learn how to
     * identify the different types of X events. We have shown how to
     * get the Expose event (which indicates your window should
     * repaint itself) and package it up as a PaintEvent.
     *
     * Note that XEvent is a union of structs. This union is
     * documented on the page following the one above:
     * http://tronche.com/gui/x/xlib/events/structures.html
     *
     * If you are unfamiliar with C/C++ unions, or need a refresher,
     * see:
     * http://www.tutorialspoint.com/cprogramming/c_unions.htm
     * http://stackoverflow.com/questions/346536/difference-between-a-structure-and-a-union-in-c 
     *
     * Another thing to note is that nothing will paint on your window
     * until it is mapped on the screen. You don't necessarily need to
     * be aware of when it is mapped, just be aware that until it's
     * displayed, any painting operations will have no effect when
     * it's actually put on the screen.
     *
     * One thing you may be wondering is why we package up an event
     * and add it to the event queue, rather than dispatching it right
     * away. The reason is that events may be added to the queue by
     * other objects (such as TimerEvents). Thus, to preserve proper
     * temporal ordering of events, you need to add events to the
     * queue and dispath them one by one.
     */
      bool button_pressed = false;
    if (XPending(this->display)) {
      // Get event, package it up, add it to the queue
      Event* newEvent = NULL;
      XEvent event;
      XNextEvent(this->display, &event);
      XWindow* window = XWindow::GetXWindowForWindow(event.xany.window);
        KeySym key;
        char key2;
        int count;
        char buffer[500];
      LOG_TODO << "TODO CS349: Implement XApplication::CheckForXEvents (remove when implemented)";
      switch (event.type) {

      case Expose:
        newEvent = new PaintEvent(window, Rectangle(event.xexpose.x, event.xexpose.y, event.xexpose.width, event.xexpose.height));
        break;

// TODO CS349
          case ConfigureNotify:
              window->GetGraphics()->SetClipRect(Rectangle(0, 0, event.xconfigure.width, event.xconfigure.height));
              newEvent = new PaintEvent(window, Rectangle(0, 0, event.xconfigure.width, event.xconfigure.height));
              break;
          case KeyPress://handle key press event
              count = XLookupString((XKeyEvent*)&event, buffer, 500, &key, NULL);
              key2 = (char)key;
              newEvent = new KeyEvent(window,KeyEvent::keyPress,key2);
              break;
          case KeyRelease:
              count = XLookupString((XKeyEvent*)&event, buffer, 500, &key, NULL);
              key2 = (char)key;
              newEvent = new KeyEvent(window,KeyEvent::keyRelease,key2);
              break;
          case ButtonPress:
              newEvent = new MouseEvent(window,MouseEvent::mouseDown,Point(event.xbutton.x, event.xbutton.y));
              button_pressed = true;
              break;
          case ButtonRelease:
              newEvent = new MouseEvent(window,MouseEvent::mouseUp,Point(event.xbutton.x, event.xbutton.y));
              button_pressed = false;
              break;
          case MotionNotify:
              if(button_pressed){
                  newEvent = new MouseEvent(window,MouseEvent::mouseDrag,Point(event.xmotion.x, event.xmotion.y));
              }
              else{
                  newEvent = new MouseEvent(window,MouseEvent::mouseMove,Point(event.xmotion.x, event.xmotion.y));
              }
              break;

      case DestroyNotify:
        // You can ignore this
        LOG(INFO) << "Destroy notify event received";
        break;
      }

      if (newEvent != NULL) {
        eventQueue->AddEventToQueue(newEvent);
      }
    }
  }

  void XApplication::Run(bool runOnce)
  {
    XSetIOErrorHandler(ErrorHandler);
  
    /*
     * This is where you will implement the event loop. The basic
     * logic of the event loop is as follows:
     * - You should service the CS349 Scheme interpreter to ensure it
     *   progresses (executes any scripts, accepts any user keyboard
     *   input) (already done for you)
     * - Check the return code for scheme_interpreter_service. If it
     *   returns a non-zero value, the application should quit
     * - You need to determine whether there are any events pending in
     *   X lib: call CheckForXEvents().
     * - You should call EventQueue's ProcessNextEvent() method to
     *   process events and service the timers. Note, you should call
     *   this *even if there are no events pending*, since it will
     *   service the timers.
     * - You can optionally call XFlush() to flush X's queue of
     *   graphics commands to the X server, but it is likely
     *   unnecessary given calls to other Xlib functions. See the
     *   following for more information:
     *   http://tronche.com/gui/x/xlib/event-handling/XFlush.html
     * - You should sleep if there is nothing left to do (i.e., there
     *   are no events in our event queue or pending in X's event
     *   queue -- check with XPending). Sleeping for a bit will ensure
     *   the CPU is not throttled.
     * - If you sleep, sleep for a very small amount of time, say
     *   1/1000 of a second, to enable interactive typing rates on the
     *   console. (See the usleep function for fine-grained sleep
     *   amounts).
     * - We *will* check to ensure your app doesn't throttle the CPU
     *   when idle, so make sure you sleep when there are no events.
     */
    this->quit = false;
    do {
      int interpreterReturnCode = 0;

      interpreterReturnCode = scheme_interpreter_service();
      if (interpreterReturnCode == 1) {
        this->Quit();
      }

      LOG_TODO << "TODO CS349: Implement event loop in XApplication::Run (remove when implemented)";
        // TODO CS349
        CheckForXEvents();
        eventQueue->ProcessNextEvent();
        if(eventQueue->GetNumEventsInQueue() > 1){
            this->delaytime = 1000;
        }
        else if(eventQueue->GetNumEventsInQueue() > 0){
            this->delaytime = 2000;
        }
        else{
            this->delaytime = 5000;
        }
        XFlush(display);
        usleep(this->delaytime);
    } while (!this->quit && !runOnce);
  }
}
