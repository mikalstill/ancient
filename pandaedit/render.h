#include <stack>
#include <libmplot.h>
#include <png.h>

#include "objectmodel.h"
#include "matrix.h"

#ifndef RENDER_H
#define RENDER_H

class pdfRender{
 public:
  pdfRender(pdf& thePDF, object page);
  bool render();
  string getPNGfile();

 private:
  pdfRender(const pdfRender& other);

  void processLine(string line);
  void pushArguement(string arg);

  void commandBT();
  void commandET();
  void commandTd();
  void commandTf();
  void commandTj();
  void commandTm();
  void commandTr();

  enum rmMode{
    rmUndefined = 0,
    rmText,
    rmGraphics
  };

  object m_page, m_contents;
  rmMode m_mode;
  matrix m_textMatrix;
  stack<string> m_arguements;
  bool m_invalid;

  plot_state *m_plot;
  unsigned int m_width, m_height;
};

#endif
