#ifndef RASTER_H
#define RASTER_H

#include <string>

class object;

class raster
{
public:
  raster (object & obj);
  void setData (char *data);

  int getWidth();
  int getHeight();
  int getK();

private:
  char *m_data;

  // Parameters
  int m_bpc;
  string m_cs;
  int m_k;
  int m_width;
  int m_height;
};

#endif
