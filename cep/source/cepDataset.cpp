/* 
   Imp for the CEP dataset
   Copyright (C) Michael Still                    2002
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "core.h"

cepDataset::cepDataset (string filename):
m_filename (filename)
{
  m_progress = NULL;
}

cepDataset::cepDataset (string filename, cepDatasetProgressCB callback):
m_filename (filename)
{
  m_progress = callback;
}

cepError
cepDataset::munch ()
{
  // Step One: ensure that all of the files we need are actually there,
  // it would be crap to read two of the three files, and _then_ report
  // an error...
  fstream files[3];
  long lines[3];

  files[0].open (string (m_filename + ".dat1").c_str (), ios::in);
  files[1].open (string (m_filename + ".dat2").c_str (), ios::in);
  files[2].open (string (m_filename + ".dat3").c_str (), ios::in);

  // Check they opened ok
  string errString;
  for (int i = 0; i < 3; i++)
    {
      // File is NULL if it couldn't be opened
      if (!files[i])
	{
	  if (errString != "")
	    errString += ";";
	  errString += " " + m_filename + ".dat" + itoa (i + 1);
	}
    }

  if (errString != "")
    {
      return
	cepError
	("File IO error for this dataset. Could not open the file(s):" +
	 errString + ".");
    }

  // Read the file
  for (int i = 0; i < 3; i++)
    {
      if (m_progress)
	m_progress (i + 1, 0);
      else
	{
	  cepError dbg ("No progress handler defined for cepDataset",
			cepError::sevInformational);
	  dbg.display ();
	}

      // Skip the first three lines -- it seems that fstream has no equivalent
      // of fgets(), which will mind the newlines for me...
      char c = 0, prevc;
      lines[i] = 0;
      string thisLine ("");

      while (!files[i].eof ())
	{
	  files[i].read (&c, 1);

	  // We skip the first three lines of the file
	  if ((lines[i] > 3) && (c != 'r') && (c != '\n'))
	    {
	      if (!isblank (c) && !isblank (prevc))
		{
		  thisLine += c;
		}
	    }

	  if (c == '\n')
	    {
	      lines[i]++;

	      if (lines[i] > 3)
		{
		  // We process this line
		  cepError dbg ("Dataset line from " + m_filename + "[" +
				itoa (i) + "]: " + thisLine,
				cepError::sevDebug);
		  dbg.display ();
		  thisLine = "";
		}
	      else
		{
		  cepError dbg ("Dataset line from " + m_filename + "[" +
				itoa (i) + "] skipped...",
				cepError::sevDebug);
		  dbg.display ();
		}

	      if (m_progress)
		m_progress (i + 1, lines[i]);
	      prevc = c;
	    }
	}
    }

  // Did we get the same number of lines from each of the files?
  if ((lines[0] != lines[1]) || (lines[1] != lines[2]))
    {
      return
	cepError
	("The number of lines read from the data files were not equal (" +
	 itoa (lines[0]) + ", " + itoa (lines[1]) + ", " + itoa (lines[2]) +
	 ").");
    }

  // No error
  return cepError ();
}
