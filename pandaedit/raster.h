#ifndef RASTER_H
#define RASTER_H

#include <string>

class object;

class raster
{
public:
  raster (object & obj);

  int getWidth();
  int getHeight();
  int getK();
  int getSamplesPerPixel();
  int getBitsPerSample();

private:
  int m_k;
  int m_width;
  int m_height;
  int m_spp;
  int m_bps;
};

#endif
