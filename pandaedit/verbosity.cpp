#include "pandaedit.h"
#include "verbosity.h"

#include <fstream>

int gVerboseLevel;
int gLogLevel;
extern fstream gLog;

void debug(debugLevel dl, string message){
  if(dl < gVerboseLevel)
    cout << message << endl;

  if(dl < gLogLevel)
    gLog << message << endl;
}
