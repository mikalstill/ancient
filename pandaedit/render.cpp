#include <stdio.h>

#include "render.h"
#include "utility.h"
#include "stringArray.h"

pdfRender::pdfRender(object& contents):
  m_contents(contents),
  m_mode(rmUndefined)
{}

void pdfRender::render()
{
  unsigned int inset = 0;
  char *stream = m_contents.getStream();

  printf("DEBUG: Process page stream\n");

  // todo_mikal: this might be too slow because of the accessor
  string line;
  while(inset < m_contents.getStreamLength())
    {
      if(stream[inset] != '\n'){
	line += stream[inset];
      }
      else{
	// Process the line
	printf("DEBUG: Process line \"%s\"\n", line.c_str());
	processLine(line);
	line = "";
      }

      inset ++;
    }
}

void pdfRender::processLine(string line)
{
  if(line.length() < 1){
    printf("DEBUG: Empty line\n");
    return;
  }

  stringArray tokens(line, " ");
  for(unsigned int i = 0; i < tokens.size(); i++)
    {
      if(isNumber(tokens[i])) pushArguement(tokens[i]);
      else if(isName(tokens[i])) pushArguement(tokens[i]);
      else if("BT" == tokens[i]) commandBT();
      else if("Tf" == tokens[i]) commandTf();
      else if("Tm" == tokens[i]) commandTm();
      else if("Tr" == tokens[i]) commandTr();
      else
	printf("DEBUG: Dropped token %s\n", tokens[i].c_str());
    }
}

/////////////////////////////////////////////////////////////////////
void pdfRender::pushArguement(string arg)
{
  printf("DEBUG: Pushing arguement %s\n", arg.c_str());
  m_arguements.push(arg);
}

/////////////////////////////////////////////////////////////////////
void pdfRender::commandBT()
{
  printf("DEBUG: BT\n");
  if(m_mode == rmText){
    printf("DEBUG: Error -- already in text mode\n");
    return;
  }

  m_mode = rmText;
  m_textMatrix.setIdentity();
}

void pdfRender::commandTf()
{
  printf("DEBUG: Tf [not implemented]\n");
}

void pdfRender::commandTm()
{
  float vals[6];

  printf("DEBUG: Tm\n");
  for(int i = 0; i < 6; i++){
    vals[5 - i] = atof(m_arguements.top().c_str());
    m_arguements.pop();
  }

  m_textMatrix.setValues(vals);
}

void pdfRender::commandTr()
{
  printf("DEBUG: Tr [not implemented]\n");
}
