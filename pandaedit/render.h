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
  void processCommandString(char *commandString, unsigned int length,
			    bool parsing);

  void processLine (string line, bool parsing);
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

  // The render functions actually mark the page based on the parsed drawing
  // chain
  void render_b ();
  void render_bstar ();
  void render_B ();
  void render_Bstar ();
  void render_BT ();
  void render_c ();
  void render_cm ();
  void render_Do ();
  void render_ET ();
  void render_f ();
  void render_fstar ();
  void render_F ();
  void render_g ();
  void render_G ();
  void render_h ();
  void render_l ();
  void render_m ();
  void render_q ();
  void render_Q ();
  void render_re ();
  void render_rg ();
  void render_RG ();
  void render_S ();
  void render_Td ();
  void render_TD ();
  void render_Tf ();
  void render_Tj ();
  void render_TL ();
  void render_Tm ();
  void render_Tr ();
  void render_v ();
  void render_w ();
  void render_y ();

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
