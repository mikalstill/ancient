// This file is used to implement the verbosity and file logging options

#ifndef VERBOSITY_H
#define VERBOSITY_H

enum debugLevel{
  dlError = 0,
  dlWarning,
  dlInformational,
  dlTrace,
  dlCode
};

void debug(debugLevel dl, string message);

#endif
