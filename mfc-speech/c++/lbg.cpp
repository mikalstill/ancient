// LBG implementation

#include "world.h"

codebook::codebook ():
m_recenters (0), m_donations (0)
{
}

codebook::codebook (mfc & codes):
m_recenters (0), m_donations (0)
{
  unsigned int i;

  m_table.clear ();
  for (i = 0; i < codes.size (); i++)
    {
      m_table.push_back (codes.getFrame (i));
    }
}

size_t
codebook::size ()
{
  return m_table.size ();
}

void
codebook::init (mfc & data)
{
  // If the target size is zero, then it should be one
  // Otherwise, we need to split the current points in twain...
  if (size () == 0)
    {
      m_table.push_back (data.centroid ());
    }
}

void
codebook::fork (float delta)
{
  vector < mfcFrame > oldtable = m_table;
  size_t i;

  m_table.clear ();
  for (i = 0; i < oldtable.size (); i++)
    {
      cout << "\tForking " << oldtable[i] << endl;
      cout << "\t        " << mfcFrame (oldtable[i], 1.0 + delta) << endl;
      cout << "\t        " << mfcFrame (oldtable[i],
					1.0 - delta) << endl << endl;

      m_table.push_back (mfcFrame (oldtable[i], 1.0 + delta));
      m_table.push_back (mfcFrame (oldtable[i], 1.0 - delta));
    }
}

void
codebook::cloud (mfc & data)
{
  // We now build the clouds around the codebook entries
  cout << "Build clouds" << endl;
  size_t i, j;

  // Clear out the old clouds
  m_clouds.resize (size ());
  for (i = 0; i < size (); i++)
    {
      m_clouds[i].clear ();
    }

  for (i = 0; i < data.size (); i++)
    {
      // Find which point this data point is closest to
      size_t found = 0;
      float dist = 2000000;

      for (j = 0; j < size (); j++)
	{
	  float newdist = m_table[j] - data.getFrame (i);

	  if (newdist < dist)
	    {
	      found = j;
	      dist = newdist;
	    }
	}

      // Add the point to that cloud
      m_clouds[found].add (data.getFrame (i));
    }

  cloudSize ();
}

void
codebook::cloudSize ()
{
  // Display information about cloud sizes
  unsigned int i;

  cout << "Cloud sizes: ";
  for (i = 0; i < size (); i++)
    {
      cout << m_clouds[i].size () << " ";
    }
  cout << endl;
}

// Now we change the table entries to match the centroids of that cloud
float
codebook::recenter ()
{
  m_recenters++;

  float maxerr = 0;
  unsigned int i;

  cout << "Recentering cloud centroids" << endl << endl;

  for (i = 0; i < size (); i++)
    {
      if (m_clouds[i].size () != 0)
	{
	  float err;

	  err = m_table[i] - m_clouds[i].centroid ();
	  cout << "\tErr: " << err << endl;
	  cout << "\tOld: " << m_table[i] << endl;
	  m_table[i] = m_clouds[i].centroid ();
	  cout << "\tNew: " << m_table[i] << endl << endl;

	  if (err > maxerr)
	    maxerr = err;
	}
      else
	{
	  cout << "\tSkipping error on zero sized cloud" << endl << endl;
	}
    }

  cloudSize ();
  return maxerr;
}

int
codebook::emptyCount ()
{
  unsigned int i;
  int count = 0;

  for (i = 0; i < size (); i++)
    {
      if (m_clouds[i].size () == 0)
	count++;
    }

  return count;
}

void
codebook::donate (float delta)
{
  m_donations++;

  // Find the biggest cloud
  int maxI;
  unsigned int i, maxSize = 0;

  for (i = 0; i < size (); i++)
    {
      if (m_clouds[i].size () > maxSize)
	{
	  maxSize = m_clouds[i].size ();
	  maxI = i;
	}
    }

  // Find the first empty cloud
  for (i = 0; i < size (); i++)
    {
      if (m_clouds[i].size () == 0)
	{
	  cout << "\tDonate from cloud " << maxI << " to cloud " << i << endl
	    << endl;

	  mfcFrame f = m_table[maxI];
	  m_table[maxI] = mfcFrame (f, 1.0 + delta);
	  m_table[i] = mfcFrame (f, 1.0 - delta);
	  return;
	}
    }
}

void
codebook::stats ()
{
  cout << "Statistics" << endl;
  cout << "\tRecenters performed: " << m_recenters << endl;
  cout << "\tDonations performed: " << m_donations << endl << endl;
}

mfcFrame & codebook::getLine (size_t i)
{
  return m_table[i];
}

void
codebook::save (string filename)
{
  fstream file;

  // Open the file
  file.open (filename.c_str (), ios::out);
  if (!file)
    {
      cerr << "Could not open file to save codebook to" << endl;
      exit (42);
    }

  // Most of these are dodgy hard coded values
  file << size () << " 100000 44 14106" << endl;

  unsigned int i;
  for (i = 0; i < size (); i++)
    {
      for (int j = 0; j < 9; j++)
	file << m_table[i].getC (j) << " ";
      file << m_table[i].getE () << endl;
    }
}

vector < int >
codebook::encode (mfc & target)
{
  unsigned int i;
  vector < int >output;

  output.resize (target.size ());
  for (i = 0; i < target.size (); i++)
    {
      // Find the table entry which is closest to this target entry
      float dist = 2000000;
      unsigned int j, tindex;

      for (j = 0; j < size (); j++)
	{
	  if (m_table[j] - target.getFrame (i) < dist)
	    {
	      dist = m_table[j] - target.getFrame (i);
	      tindex = j;
	    }
	}

      output[i] = tindex;
    }

  return output;
}

ostream & operator << (ostream & stream, codebook cb)
{
  size_t i;

  for (i = 0; i < cb.size (); i++)
    {
      stream << (i +
		 1) << " of " << cb.size () << ": " << cb.getLine (i) << endl;
    }

  return stream;
}
