// Header for the LBG implementation

#include "world.h"

#ifndef LBG_H
#define LBG_H

class codebook
{
public:
  codebook ();
  codebook (mfc & codes);

  size_t size ();
    mfcFrame & getLine (size_t i);

  void init (mfc & data);
  void fork (float delta);
  void cloud (mfc & data);
  void cloudSize ();
  float recenter ();
  int emptyCount ();
  void donate (float delta);
  void stats ();
  void save (string filename);
    vector < int >encode (mfc & target);

private:
    vector < mfcFrame > m_table;
    vector < mfc > m_clouds;

  long m_recenters;
  long m_donations;
};

ostream & operator<< (ostream & stream, codebook cb);

#endif
