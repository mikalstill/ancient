
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
#include "cepStringArray.h"

cepDataset::cepDataset ():
  m_filename(""),
  m_offset(""),
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
  
  files[0].open (string (m_filename + ".dat1").c_str (), ios::in);
  files[1].open (string (m_filename + ".dat2").c_str (), ios::in);
  files[2].open (string (m_filename + ".dat3").c_str (), ios::in);
  cepDebugPrint("Opened the dataset files");

  // Check they opened ok
  string errString;

  for (int i = 0; i < 3; i++)
  {
    // File is NULL if it couldn't be opened
    if (!files[i].is_open())
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

	// End of line?
	if ((c == '\n') && (thisLine != ""))
	  {
	    cepDebugPrint("Processing line: " + thisLine);

	    // If this is a data bearing line
	    if(cepIsNumeric(thisLine.c_str()[0]))
	      {
		numLines ++;
		
		// Break the line into it's columns, I prefer this to the strtok method we used to use...
		cepStringArray sa(thisLine, " ");
		row.date = atof(sa[0].c_str());
		// todo_mikal: perhaps cache the float version of the offset in a member variable?
		row.sample = atof(applyOffset(sa[1]).c_str()) - atof(m_offset.c_str());
		row.error = atof(sa[2].c_str());
		
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
	      }
	    else
	      {
		// This is a header / textual line -- perhaps it's even an offset line
		m_header[1] += thisLine;

		cepStringArray sa(thisLine, " ");
		if(cepIsNumeric(sa[6].c_str()[0]))
		  {
		    m_offset = sa[6];
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

cepError cepDataset::write (const string& filename)
{
  m_filename = filename;

  fstream files[3];
  string thisLine;
  cep_datarow row;
  
  files[0].open (string (m_filename + ".dat1").c_str (), ios::out);
  files[1].open (string (m_filename + ".dat2").c_str (), ios::out);
  files[2].open (string (m_filename + ".dat3").c_str (), ios::out);
  cepDebugPrint("Opened the dataset files");

  // Check they opened ok
  string errString;

  for (int i = 0; i < 3; i++)
  {
    // File is NULL if it couldn't be opened
    if (!files[i].is_open())
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

  // Write the files
  for (int i = 0; i < 3; i++)
  {
    if (m_progress)
      m_progress (i + 1, 0);

    files[i] << m_header[i];
    files[i].close();
  }

  cepDebugPrint("Finished writing all of the dataset files");
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

cepMatrix <double> cepDataset::getP(const string &filename)
{
  fstream file;
  int rows = 0, cols = 0;
  double val = 0.0;
  
  file.open(filename.c_str(), ios::in);

  if (!file.is_open())
  {
    cepError oor ("Error! Can not open file:" + filename,
                  cepError::sevErrorFatal);
    oor.display ();
  }

  file >> rows;
  file >> cols;

  //check the matrix is square
  if(rows != cols)
  {
    cepError oor ("Error! The file: " + filename + " does not contain a square matrix",
                  cepError::sevErrorFatal);
    oor.display ();
  }

  cepMatrix <double> matP(rows, cols);

  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      file >> val;

      //check to see if we have hit the eof to early
      if(file.eof())
      {
        cepError oor ("Error! File:" + filename + " contain to few values",
                  cepError::sevErrorFatal);
        oor.display ();
      }
      matP.setValue(i,j,val);
    }
  }

  file >> val;

  //check to see there are no more values in the file
  if(!file.eof())
  {
    cepError oor ("Error! File: " + filename + " contains too many values",
                  cepError::sevErrorFatal);
    oor.display ();
  }
    
  return matP;
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

// The datasets have an offset which we need to apply. For instance:
// m_offset = 425634.1235
// value = 1.1234
//
// should result in 425631.1234
string cepDataset::applyOffset(string value)
{
  // Remove the sign from the value
  string newvalue;
  cepDebugPrint("Value leading character is " + value.substr(0, 1));
  if(value.substr(0, 1) == "-")
    newvalue = value.substr(1, value.length() - 1);
  else newvalue = value;
  cepDebugPrint("New value is " + newvalue);

  // Find the decimal points
  cepStringArray offset(m_offset, ".");
  cepStringArray val(newvalue, ".");
  string retval;
  
  retval = offset[0].substr(0, offset[0].length() - val[0].length());
  retval += newvalue;

  cepDebugPrint("Offset calculation: offset = " + m_offset +
		" value = " + newvalue);
  cepDebugPrint("Resultant value = " + retval);
  return retval;
}
