
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
  m_procHistory(""),
  m_ready(false),
  m_wellformed(false)
{
  for(int i = 0; i < dirUnknown; i++){
    m_data[i] = NULL;
    m_offset[i] = "";
    m_offsetFloat[i] = 0.0;
  }

  m_progress = NULL;
}

cepDataset::cepDataset (cepDatasetProgressCB callback):
  m_filename (""),
  m_procHistory(""),
  m_ready(false),
  m_wellformed(false)
{ 
  for(int i = 0; i < dirUnknown; i++){
    m_data[i] = NULL;
    m_offset[i] = "";
    m_offsetFloat[i] = 0.0;
  }

  m_progress = callback;
}

cepDataset::cepDataset (cepMatrix<double> *data0, cepMatrix<double> *data1, cepMatrix<double> *data2, 
			string offsets[3], string procHistory, string headers[3]):
  m_filename(""),
  m_procHistory(procHistory),
  m_ready(true),
  m_wellformed(true)
{
  m_data[0] = data0;
  m_data[1] = data1;
  m_data[2] = data2;
  
  m_offset[0] = offsets[0];
  m_offsetFloat[0] = atof(offsets[0].c_str());
  m_offset[1] = offsets[1];
  m_offsetFloat[1] = atof(offsets[1].c_str());
  m_offset[2] = offsets[2];
  m_offsetFloat[2] = atof(offsets[2].c_str());

  m_header[0] = headers[0];
  m_header[1] = headers[1];
  m_header[2] = headers[2];
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
  double rowdate, rowsample, rowerror;
  double lastRowdate, lastRowsample, lastRowerror;
  
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

    vector<double> dates;
    vector<double> samples;
    vector<double> errors;

    c = 0;
    prevc = '\n';
    thisLine = "";
    numLines = 1;
    
    lastRowdate = -1;
    lastRowsample = -1;
    lastRowerror = -1;
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
		if (m_progress)
		  m_progress (i + 1, numLines);
		numLines ++;
		
		// Break the line into it's columns, I prefer this to the strtok method we used to use...
		cepStringArray sa(thisLine, " ");
		rowdate = atof(sa[0].c_str());
		rowsample = atof(applyOffset((direction) i, sa[1]).c_str()) - m_offsetFloat[i];
		rowerror = atof(sa[2].c_str());
		
		if(lastRowdate == -1)
		  {
		    dates.push_back(rowdate);
		    samples.push_back(rowsample);
		    errors.push_back(rowerror);
		    
		    lastRowdate = rowdate;
		    lastRowsample = rowsample;
		    lastRowerror = rowerror;
		  }
		else
		  {
		    if(rowdate < lastRowdate)
		      {
			m_ready = true;
			return cepError("dataset: " + m_filename + ".dat" + 
					cepToString(i + 1) + " is not in date order! At line " + 
					cepToString(numLines), cepError::sevErrorRecoverable);
		      }
		    else
		      {
			if(rowdate == lastRowdate)
			  {
			    m_ready = true;
			    return cepError("dataset: " + m_filename + ".dat" + 
					    cepToString(i + 1) + " contains repeated values at line " + 
					    cepToString(numLines),cepError::sevErrorRecoverable);
			  }
			else
			  {
			    dates.push_back(rowdate);
			    samples.push_back(rowsample);
			    errors.push_back(rowerror);
			    
			    lastRowdate = rowdate;
			    lastRowsample = rowsample;
			    lastRowerror = rowerror;
			  }
		      }
		  }
	      }
	    else
	      {
		// This is a header / textual line -- perhaps it's even an offset line
		if(numLines == 1){
		  cepStringArray sa(thisLine, " ");

		  // Is this an offset line?
		  if(cepIsNumeric(sa[6].c_str()[0]))
		    {
		      m_header[i] += thisLine;
		      m_offset[i] = sa[6];
		      m_offsetFloat[i] = atof(m_offset[i].c_str());
		    }

		  // Well, then it must be a processing statement line
		  else if(i == 0)
		    {
		      m_procHistory = thisLine;
		    }
		}
	      }
	    
	    thisLine = "";
	  }
	prevc = c;
      }

    // Copy the vectors into the matrix
    m_data[i] = new cepMatrix<double>(dates.size(), 3);
    for(unsigned int vcount = 0; vcount < dates.size(); vcount++){
      m_data[i]->setValue(vcount, colDate, dates[vcount]);
      if(m_data[i]->getError().isReal())
	return m_data[i]->getError();

      m_data[i]->setValue(vcount, colSample, samples[vcount]);
      if(m_data[i]->getError().isReal())
	return m_data[i]->getError();

      m_data[i]->setValue(vcount, colError, errors[vcount]);
      if(m_data[i]->getError().isReal())
	return m_data[i]->getError();
    }
    
    files[i].close();
  }
  
  // Are the files over the same period??
  if (((m_data[0]->getValue(0, 0) != m_data[1]->getValue(0, 0)) 
       || (m_data[1]->getValue(0, 0) != m_data[2]->getValue(0, 0)) 
       || ((m_data[0]->getValue(m_data[0]->getNumRows() - 1, 0) 
	    != m_data[1]->getValue(m_data[0]->getNumRows() - 1, 0)) 
	   || (m_data[1]->getValue(m_data[0]->getNumRows() - 1, 0) 
	       != m_data[2]->getValue(m_data[0]->getNumRows() - 1, 0)))))
    {
      m_ready = true;
      return cepError("The data set " + m_filename + 
  		      " values do not represent the same time period",
  		      cepError::sevErrorRecoverable);
    }

  if(m_data[0]->getError().isReal())
    return m_data[0]->getError();
  if(m_data[1]->getError().isReal())
    return m_data[1]->getError();
  if(m_data[2]->getError().isReal())
    return m_data[2]->getError();
  
  m_ready = true;
  m_wellformed = true;
  return cepError ();
}

cepError cepDataset::write (const string& filename)
{
  m_filename = filename;

  // Step One: ensure that all of the files we need are actually there,
  // it would be bad to read two of the three files, and _then_ report
  // an error...
  fstream files[3];
  
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

  for(int i = 0; i < 3; i++)
    {
      files[i] << m_procHistory << endl;
      files[i] << m_header[i] << endl << endl;
      cepDebugPrint("Writing dataset with offset of " + m_offset[i] + " in direction " + cepToString(i));

      for(int vcount = 0; vcount < m_data[i]->getNumRows(); vcount++)
	{
	  files[i] << " " << cepToString(m_data[i]->getValue(vcount, colDate));
	  if(m_data[i]->getError().isReal())
	    return m_data[i]->getError();

	  files[i] << "     " << reverseOffset((direction) i,
					       cepToString(m_data[i]->getValue(vcount, colSample) + 
							   m_offsetFloat[i]));
	  if(m_data[i]->getError().isReal())
	    return m_data[i]->getError();

	  files[i] << "   " << cepToString(m_data[i]->getValue(vcount, colError));
	  if(m_data[i]->getError().isReal())
	    return m_data[i]->getError();

	  files[i] << endl;
	}

      files[i].close();
    }

  return cepError();
}

cepMatrix<double> *cepDataset::getMatrix (direction dir)
{
  return m_data[dir];
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
string cepDataset::applyOffset(cepDataset::direction i, string value)
{
  // Remove the sign from the value
  string newvalue;
  if(value.substr(0, 1) == "-")
    newvalue = value.substr(1, value.length() - 1);
  else newvalue = value;

  // Find the decimal points
  cepStringArray offset(m_offset[i], ".");
  cepStringArray val(newvalue, ".");
  string retval;
  
  retval = offset[0].substr(0, offset[0].length() - val[0].length());
  retval += newvalue;

  cepDebugPrint("Offset calculation: offset = " + m_offset[i] +
		" value = " + newvalue + " resultant value = " + retval);
  return retval;
}

string cepDataset::reverseOffset(cepDataset::direction i, string value)
{
  // Remove the sign from the value
  string sign("");
  if(value.substr(0, 1) == "-")
    sign = "-";

  int count = 1;
  while(value.substr(0, count) == m_offset[i].substr(0, count)){
    cepDebugPrint("Finding: " + value.substr(0, count) +
		  ", " + m_offset[i].substr(0, count) + " count = " +
		  cepToString(count));
    if(value.substr(count, 1) == "."){
      cepDebugPrint("Breaking because of a decimal point");
      break;
    }

    count++;
  }

  cepDebugPrint("Deoffset calculation offset = " + m_offset[i] + " value = " + value +
		" indent count = " + cepToString(count + 1) + " resultant value = " +
		sign + value.substr(count - 1, value.length()));
  return string(sign + value.substr(count - 1, value.length()));
}

// Return the root filename for the dataset
string cepDataset::getRootFilename()
{
  return m_filename;
}

// Return a filtered dataset
cepDataset cepDataset::filter(float low, float high)
{
  vector<double> dates;
  vector<double> samples;
  vector<double> errors;
  cepMatrix<double> *data[dirUnknown];

  for(int dir = 0; dir < dirUnknown; dir++){
    dates.clear();
    samples.clear();
    errors.clear();

    for(int i = 0; i < m_data[dir]->getNumRows(); i++){
      if((m_data[dir]->getValue(i, colDate) >= low) &&
	 (m_data[dir]->getValue(i, colDate) <= high)){
	dates.push_back(m_data[dir]->getValue(i, colDate));
	if(m_data[dir]->getError().isReal()){
	  m_data[dir]->getError().display();
	  return cepDataset();
	}

	samples.push_back(m_data[dir]->getValue(i, colSample));
	if(m_data[dir]->getError().isReal()){
	  m_data[dir]->getError().display();
	  return cepDataset();
	}

	errors.push_back(m_data[dir]->getValue(i, colError));
	if(m_data[dir]->getError().isReal()){
	  m_data[dir]->getError().display();
	  return cepDataset();
	}
      }
    }

    if(dates.size() > 0){
      // Copy the vectors into the matrix
      data[dir] = new cepMatrix<double>(dates.size(), 3);
      for(unsigned int vcount = 0; vcount < dates.size(); vcount++){
	data[dir]->setValue(vcount, colDate, dates[vcount]);
	if(m_data[dir]->getError().isReal()){
	  m_data[dir]->getError().display();
	  return cepDataset();
	}

	data[dir]->setValue(vcount, colSample, samples[vcount]);
	if(m_data[dir]->getError().isReal()){
	  m_data[dir]->getError().display();
	  return cepDataset();
	}

	data[dir]->setValue(vcount, colError, errors[vcount]);
	if(m_data[dir]->getError().isReal()){
	  m_data[dir]->getError().display();
	  return cepDataset();
	}
      }
    }
    else{
      return cepDataset();
    }
  }

  return cepDataset(data[0], data[1], data[2], m_offset, m_procHistory + ": Zoomed",
		    m_header);
}

// The display name of the dataset
string cepDataset::getName()
{
  // todo_mikal: squelch repeated uses of the word Zoomed...
  cepStringArray name(m_filename, "/~");
  return name[name.size() - 1];
}

// The processing history for this dataset
string cepDataset::getProcHistory()
{
  return m_procHistory;
}

// Return the offset applied to this dataset
string cepDataset::getOffset(direction i)
{
  return m_offset[i];
}
