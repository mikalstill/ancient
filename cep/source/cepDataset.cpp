
/* 
 *   Imp for the CEP dataset
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Daniel Fernandez                 2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cepDataset.h"

cepDataset::cepDataset ():
  m_filename(""),
  m_ready(false),
  m_wellformed(false)
{
  m_progress = NULL;
}

cepDataset::cepDataset (cepDatasetProgressCB callback):
  m_filename (""),
  m_ready(false),
  m_wellformed(false)
{
  m_progress = callback;
}


cepError cepDataset::read (const string& filename)
{
  m_filename = filename;

  // Step One: ensure that all of the files we need are actually there,
  // it would be bad to read two of the three files, and _then_ report
  // an error...
  fstream files[3];
  long numLines;
  char c, prevc;
  string thisLine;
  cep_datarow row, lastRow;
  char * line;
  
  files[0].open (string (m_filename + ".dat1").c_str (), ios::in);
  files[1].open (string (m_filename + ".dat2").c_str (), ios::in);
  files[2].open (string (m_filename + ".dat3").c_str (), ios::in);
  cepDebugPrint("Opened the dataset files");

  // Check they opened ok
  string errString;

  for (int i = 0; i < 3; i++)
  {
    // File is NULL if it couldn't be opened
    if (!files[i])
    {
      if (errString != "")
        errString += ";";
      errString += " " + m_filename + ".dat" + cepToString (i + 1);
    }
  }

  if (errString != "")
  {
    m_ready = true;
    return cepError("File IO error for this dataset. Could not open the file(s):" + 
		    errString + ".");
  }
  cepDebugPrint("All dataset files exist");

  // Read the file
  for (int i = 0; i < 3; i++)
  {
    if (m_progress)
      m_progress (i + 1, 0);

    c = 0;
    prevc = '\n';
    thisLine = "";
    numLines = 1;
    
    lastRow.date = -1;
    lastRow.sample = -1;
    lastRow.error = -1;
    cepDebugPrint("File read initialization complete");

    while (!files[i].eof ())
    {   
      files[i].read (&c, 1);

      // If the line contains non-numbers then ignore the whole line
      if(!cepIsNumeric(c) && !cepIsBlank(c))
      {
	string igLine("");
	igLine += c;
	cepDebugPrint("Ignore mode because: (" + cepToString(c) + ") numeric = " + cepToString(cepIsNumeric(c)));

        while((c != '\n') && (!files[i].eof()))
        {
          files[i].read(&c,1);
	  igLine += c;
        }

	cepDebugPrint("Ignoring line: " + igLine);
        thisLine = "";
        numLines++;
      }
      else
      {
	// Squelch repeated whitespace
        if(cepIsBlank(c) == true)
        {
          if(cepIsBlank(prevc) != true)
          {
            thisLine += " ";
          }
        }
        else
        {
          thisLine += c;
        }
      }

      // End of line?
      if ((c == '\n') && (thisLine != ""))
      {
	cepDebugPrint("Processing line: " + thisLine);
        numLines ++;
        line = strdup(thisLine.c_str());

        row.date = atof(strtok(line, " "));
        row.sample = atof(strtok(NULL, " "));
        row.error = atof(strtok(NULL, " "));

        if(lastRow.date == -1)
        {
          getData((cepDataset::direction) i).push_back(row);
          lastRow = row;
        }
        else
        {
          if(row.date < lastRow.date)
          {
	    m_ready = true;
            return cepError("dataset: " + m_filename + ".dat" + 
			    cepToString(i + 1) + " is not in date order! At line " + 
			    cepToString(numLines), cepError::sevErrorRecoverable);
          }
          else
          {
            if(row.date == lastRow.date)
            {
	      m_ready = true;
              return cepError("dataset: " + m_filename + ".dat" + 
			      cepToString(i + 1) + " contains repeated values at line " + 
			      cepToString(numLines),cepError::sevErrorRecoverable);
            }
            else
            {
              getData((cepDataset::direction) i).push_back(row);
              lastRow = row;
            }
          }
        }
        thisLine = "";
      }
      prevc = c;
    }
    files[i].close();
  }
  cepDebugPrint("Finished reading all of the dataset files");

  // Are the files over the same period??
  if (((m_datax.front().date != m_datay.front().date) || (m_datay.front().date != m_dataz.front().date)) 
      || ((m_datax.back().date != m_datay.back().date) || (m_datay.back().date != m_dataz.back().date)))
  {
    m_ready = true;
    return cepError("The data set " + m_filename + 
		    " values do not represent the same time period",
		    cepError::sevErrorRecoverable);
  }
    
  m_ready = true;
  m_wellformed = true;
  return cepError ();
}

vector < cep_datarow > &cepDataset::getData (direction dir)
{
  switch (dir)
  {
  case dirX:
    return m_datax;

  case dirY:
    return m_datay;

  case dirZ:
    return m_dataz;

  default:
    cepError oor ("The data direction requested was out of range",
                  cepError::sevErrorFatal);
    oor.display ();
  }
  return m_datax;
}

// This needs to return a pointer so that the view class can have a NULL
// until the dataset has finished parsing...
cepMatrix < double > *cepDataset::getMatrix(direction dir)
{
  vector < cep_datarow > vec = getData(dir);
  cepMatrix < double > *mat = new cepMatrix<double>((signed) vec.size(), 3);

  for(int i = 0; i < (signed)vec.size(); i ++)
  {
    mat->setValue(i, 0, vec[i].date);
    mat->setValue(i, 1, vec[i].sample);
    mat->setValue(i, 2, vec[i].error);
  }
  return mat;
}

bool cepDataset::isReady()
{
  return m_ready;
}

bool cepDataset::isWellFormed()
{
  return m_wellformed;
}

cepDataset::direction cepDataset::getDirectionFromName(string name)
{
  string lname = cepToLower(name);

  cepDebugPrint("Getting direction: " + lname);
  if(name == "x")
    return dirX;
  if(name == "y")
    return dirY;
  if(name == "z")
    return dirZ;

  cepError unknown("Unknown direction " + name + " requested of the dataset",
		   cepError::sevErrorRecoverable);
  unknown.display();
  return dirX;
}
