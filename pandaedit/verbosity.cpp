#include "pandaedit.h"
#include "verbosity.h"

#include <sys/timeb.h>
#include <iostream>
#include <fstream>
using namespace std;

int gVerboseLevel;
int gLogLevel;
time_t gStartSecs = -1;
extern fstream gLog;

void debug(debugLevel dl, string message){
  if(dl < gVerboseLevel)
    cout << message << endl;

  if(dl < gLogLevel){
    // Output the time that the log message reached here. This can be used
    // for profiling anf other interesting things
    struct timeb timeInfo;
    ftime(&timeInfo);
    if(gStartSecs == -1)
      gStartSecs  = timeInfo.time;
    
    gLog << (timeInfo.time - gStartSecs) <<"." << timeInfo.millitm << ": ";
    if(dl == dlTrace)
      gLog << "\t\t";

    gLog << message << endl;
  }
}
