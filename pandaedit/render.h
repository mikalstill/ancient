#include <stack>

#include "objectmodel.h"
#include "matrix.h"

#ifndef RENDER_H
#define RENDER_H

class pdfRender{
 public:
  pdfRender(object& contents);
  void render();

 private:
  void processLine(string line);
  void pushArguement(string arg);

  void commandBT();
  void commandTf();
  void commandTm();
  void commandTr();

  enum rmMode{
    rmUndefined = 0,
    rmText,
    rmGraphics
  };

  object& m_contents;
  rmMode m_mode;
  matrix m_textMatrix;
  stack<string> m_arguements;
};

#endif
