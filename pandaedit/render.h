#include <stack>
#include <libmplot.h>
#include <png.h>

#include "objectmodel.h"
#include "matrix.h"
#include "pdfDoc.h"

#ifndef RENDER_H
#define RENDER_H

class pdfRender
{
public:
  pdfRender (pdfDoc *theDoc, int pageno);
  bool render ();
  bool parseStream ();
  
  char *getSelectRaster();
  string getPNGfile ();

private:
  bool processContentsObject(const object &obj);
  void processCommandString(char *commandString, unsigned int length);

  void processLine (string line);
  void pushArguement (string arg);

  // Commands group up drawing operations, and push them onto the drawing
  // chain
  void command_b ();
  void command_bstar ();
  void command_B ();
  void command_Bstar ();
  void command_BT ();
  void command_c ();
  void command_cm ();
  void command_Do ();
  void command_ET ();
  void command_f ();
  void command_fstar ();
  void command_F ();
  void command_g ();
  void command_G ();
  void command_h ();
  void command_l ();
  void command_m ();
  void command_q ();
  void command_Q ();
  void command_re ();
  void command_rg ();
  void command_RG ();
  void command_S ();
  void command_Td ();
  void command_TD ();
  void command_Tf ();
  void command_Tj ();
  void command_TL ();
  void command_Tm ();
  void command_Tr ();
  void command_v ();
  void command_w ();
  void command_y ();

  void appendCommand();

  enum rmMode
  {
    rmUndefined = 0,
    rmText,
    rmGraphics
  };

  pdfDoc *m_doc;
  objectlist m_contents;
  rmMode m_mode;
  matrix m_textMatrix, m_textLineMatrix, m_graphicsMatrix;
    stack < string > m_arguements;
  bool m_hasLine;
  int m_pageno;

  plot_state *m_plot, *m_select;
  unsigned int m_width, m_height;

  string m_commandString, m_controlString;
};

#endif
