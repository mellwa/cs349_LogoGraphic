#include <string.h>

#include "XApplication.h"
#include "Logging.h"
#include "SchemeInterpreter.h"
#include "StopWatch.h"

using namespace std;
using namespace cs349;

const char* scheme_init_fname = "init.scm";
const char* cs349_init_fname  = "cs349_init.scm";

int main (int argc, char *argv[])
{
  FLAGS_logtostderr = 1;
  //  FLAGS_log_dir = "./log";
  google::InitGoogleLogging(argv[0]);
  LOG_DEBUG << "Starting up a1_main.cpp";

  XApplication* app = XApplication::GetInstance();
  scheme_interpreter_init(scheme_init_fname, cs349_init_fname, argc, argv);

  StopWatch* stopWatchWindow = new StopWatch(app->GetEventQueue(), Rectangle(0, 0, 200, 50));

  stopWatchWindow->SetVisible(true);
  
  app->Run();
	
  delete app;
  return scheme_interpreter_shutdown();
}
