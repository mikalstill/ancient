// Parse the MFC files into memory

#include "world.h"

mfcFrame::mfcFrame ()
{
}

mfcFrame::mfcFrame (mfcFrame & other, float delta)
{
  int i;

  for (i = 0; i < 10; i++)
    {
      setC (i, other.getC (i) * delta);
    }
  setE (other.getE () * delta);
}

void
mfcFrame::setC (size_t index, float val)
{
  m_c[index] = val;
}

void
mfcFrame::setE (float val)
{
  m_e = val;
}

float
mfcFrame::getC (size_t index)
{
  return m_c[index];
}

float
mfcFrame::getE ()
{
  return m_e;
}

float
mfcFrame::operator- (mfcFrame other)
{
  float dist = 0;
  size_t i;

  for (i = 0; i < 10; i++)
    {
      dist += pow (getC (i) - other.getC (i), 2);
    }
  dist += pow (getE () - other.getE (), 2);

  return sqrt (dist);
}

// Returns a non empty string if there was an error
string mfc::add (mfcFrame & frame)
{
  m_frames.push_back (frame);
  return string ("");
}

// Returns a non empty string if there was an error
string mfc::add (string & filename)
{
  fstream
    file;

  // Open the file
  file.open (filename.c_str (), ios::in);
  if (!file)
    {
      return string ("Failed to open the file " + filename);
    }

  // Read until end of file
  int
  stage (0),
  col (0),
    numframes,
    timescale,
    bytes,
    type;
  char
    c;
  string
  line ("");
  mfcFrame
    f;

  while (!file.eof ())
    {
      file.read (&c, 1);

      if ((c != '\n') && (c != ' '))
	line += c;
      else if (line.length () > 0)
	{
	  // The first four fields in the file are information about the file
	  switch (stage)
	    {
	    case 0:
	      numframes = atoi (line.c_str ());
	      break;
	    case 1:
	      timescale = atoi (line.c_str ());
	      break;
	    case 2:
	      bytes = atoi (line.c_str ());
	      break;
	    case 3:
	      type = atoi (line.c_str ());
	      break;
	    default:
	      // The rest go in the cycle of ten MFC values, then a log energy
	      if (col < 10)
		{
		  f.setC (col, atof (line.c_str ()));
		  col++;
		}
	      else
		{
		  f.setE (atof (line.c_str ()));
		  m_frames.push_back (f);
		  col = 0;
		}
	      break;
	    }

	  line = "";
	  ++stage;
	}
    }

  // Print out some information about the file...
  cout << filename << ": " << numframes << " frames" << endl;
  return string ("");
}

mfcFrame & mfc::getFrame (size_t index)
{
  return m_frames[index];
}

size_t mfc::size ()
{
  return m_frames.size ();
}

mfcFrame mfc::centroid ()
{
  mfcFrame
    cent;
  size_t
    i;
  int
    j;

  for (i = 0; i < 10; i++)
    cent.setC (i, 0.0);
  cent.setE (0.0);

  for (i = 0; i < m_frames.size (); i++)
    {
      for (j = 0; j < 10; j++)
	cent.setC (j, cent.getC (j) + m_frames[i].getC (j));
      cent.setE (cent.getE () + m_frames[i].getE ());
    }

  for (i = 0; i < 10; i++)
    cent.setC (i, cent.getC (i) / m_frames.size ());
  cent.setE (cent.getE () / m_frames.size ());

  return cent;
}

void
mfc::clear ()
{
  m_frames.clear ();
}

ostream & operator<< (ostream & stream, mfcFrame frame)
{
  stream << "Frame: ";

  int i;
  for (i = 0; i < 10; i++)
    stream << frame.getC (i) << " ";

  stream << "energy: " << frame.getE ();
  return stream;
}
