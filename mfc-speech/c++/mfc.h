// Header for the MFC file parser

#include "world.h"
#ifndef MFC_H
#define MFC_H

class mfcFrame
{
public:
  mfcFrame ();
  mfcFrame (mfcFrame & other, float delta);

  void setC (size_t index, float val);
  void setE (float val);
  float getC (size_t index);
  float getE ();

  // Returns the "distance" between two frames
  float operator- (mfcFrame other);

private:
  float m_c[10];
  float m_e;
};

class mfc
{
public:
  string add (string & filename);
  string add (mfcFrame & frame);
    mfcFrame & getFrame (size_t index);

  // Returns the number of frames in the data pool
  size_t size ();

  // Return the centroid of this dataset
  mfcFrame centroid ();

  // Clear the data
  void clear ();

private:
    vector < mfcFrame > m_frames;
};

// Allow out stream stuff
ostream & operator<< (ostream & stream, mfcFrame frame);

#endif
