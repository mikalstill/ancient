#include "pandaedit.h"
#include "verbosity.h"

#include <iostream>
#include <fstream>
using namespace std;

int gVerboseLevel;
int gLogLevel;
extern fstream gLog;

void debug(debugLevel dl, string message){
  if(dl < gVerboseLevel)
    cout << message << endl;

  if(dl < gLogLevel)
    gLog << message << endl;
}
