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

  void command_BT();
  void command_ET();
  void command_l();
  void command_m();
  void command_q();
  void command_Q();
  void command_S();
  void command_Td();
  void command_Tf();
  void command_Tj();
  void command_Tm();
  void command_Tr();

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
  pdf m_pdf;
  bool m_hasLine;

  plot_state *m_plot;
  unsigned int m_width, m_height;
};

#endif
