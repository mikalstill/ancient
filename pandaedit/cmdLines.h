// Classes needed to implement lines in streams

#include <wx/gdicmn.h>

#ifndef CMDLINES_H
#define CMDLINES_H

class move:public streamCommand
{
 public:
  move(unsigned int x, unsigned int y);
  void execute();
  
 private:
  move();
};

#endif
