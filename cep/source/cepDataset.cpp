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


cepDataset::cepDataset (string filename):
m_filename (filename)
{
  m_progress = NULL;
}

//this may be a little wrong...
//this constructor will be called at the end of cepDataset::doWindow in order to rerturn
//the 2 values: the vector of windowed data, and the number of windows in the vector.
cepDatset::cepDataset(vector<cep_datarow> windowVector, int numWindows):
{
	m_windowVector = windowVector;
	m_numWindows = numWindows;
	//todo: daniel - suss out whether or not we will need to change other value...ie make some NULL
	//might be a bad idea.
}


//this may also be a little wrong....
//this constructor will be called at the end of cepDataset::doHam in order to rerturn
//the 2 values: the single hamming value, and the weight.
cepDatset::cepDataset(double value, double weight):
{
	m_hamValue = value;
	m_hamWeight = weight;
	//todo: daniel - suss out whether or not we will need to change other value...ie make some NULL
	//might be a bad idea.
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
      char c = 0, prevc = '\n';
      lines[i] = 0;
      string thisLine ("");

      while (!files[i].eof ())
	{
	  files[i].read (&c, 1);

	  // We skip the first three lines of the file
	  if ((lines[i] > 3) && (c != 'r') && (c != '\n'))
	    {
	      if (isblank (c))
		{
		  if(!isblank(prevc))
		    {
		    thisLine += " ";
		    }
		}
	      else
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
		  {
		    cepError dbg ("Dataset line from " + m_filename + "[" +
				  itoa (i) + "]: " + thisLine,
				  cepError::sevDebug);
		    dbg.display ();
		  }

		  // Put the data into the dataset data thingies
		  // todo_mikal: is there a more c++ way to tokenize a string?
		  cep_datarow row;
		  char *line;
		  char *value;
		  
		  line = strdup(thisLine.c_str());
		  row.x.date = atof((value = strtok(line, " ")) != NULL ? value : "-1");
		  row.x.sample = atof((value = strtok(NULL, " ")) != NULL ? value : "-1");
		  row.x.error = atof((value = strtok(NULL, " ")) != NULL ? value : "-1");
		  if(line != NULL)
		    free(line);

		  m_data.push_back(row);

		  {
		    cepError dbg ("Dataset line parsed to [" +
				  ftoa (row.x.date) + ", " + ftoa(row.x.sample) + ", " +
				  ftoa (row.x.error) + "]",
				  cepError::sevDebug);
		    dbg.display ();
		  }

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
	    }

	  prevc = c;
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

}//end munch


cepDataset
cepDataset::doHam (double datRow[3], double startWindow, double winSize)
{

	/*	Imports:
		* datRow: a data row from the standard array of data [date,value,error]
		* startWindow: the date of the start of the hamming window
		* winSize: the size (in decimal years) of the window

		Exports:
		* hamValue: the hamming value
		* hamWeight: the weight of the value within the set
	*/

	double hamWeight=0;
	double hamValue=0;

	hamWeight = 0.54 - 0.46*cos(2*pi*((datRow[0]-startWindow)/(winSize)); //todo:daniel ask nick why 0.54 and .46...shouldn't be hard coded here.
	hamValue  = datRow[1] * weight;
	cepDataset ham =  new cepDataset(hamValue, hamWeight);
	return ham;
	//totdo:daniel- need to ensure a constructor to deal with this returning business above.

}//end doHam


cepDatset
cepDataset::doWindow (double winSize, double overlap) //todo_daniel: once vector exists..fix this..wont need param data.
{
	/*
	Groups data into square windows with a pre-defined width. 
	Imports: 
	* winSize - size of each window (deciaml years)
	* overlap - amount of overlap between each window

	Exports:
	* windowData: Vector of windowed data (numWindows X 3 X largestWindow)
	* numWindows: The number of seperate windows that were populated with data
	*/

	int numSamples; 				//number of smaples in the vector
	int nextFirstRecord; 		//start of next window
	int currentFirstRecord; //start of current window
	int numWindows; 				//the total number  of windows required
	int dataVectorRow;	 		//vector counter
	int col, row; 					//loop counters
	double firstDate;				//the first dat in the the data
	double lastDate;				//the last date in the data
	double overlapWinSize; 
	vector<cep_datarow> windowData;//this is windowsArray
	vector<cep_datarow> dataCopy//need a copy of dataCopy

	//preserving the original data vector
	dataCopy = m_data;
	
  // get timescale of data set 
  numSamples = dataCopy.size() 
  firstdate = dataCopy.pop_first(); //data(1,1);//todo_daniel: vector not defined yet..correct notation needed
  lastdate = dataCopy.pop_back() //[numSamples][0];
	windowData.resize(numSamples);

  // For optimizing speed slightly 
  overlapWinSize = winSize * (1-overlap);

  // Work out number of windows 
  numWindows = ceil(((lastdate - firstdate)*(1+2*overlap))/winSize); //round up to nearest integer
	if (overlap != 0){
    numWindows -=1;//totdo:daniel - may not need this line.
	}
  
  // Divide into windows 
  nextFirstRecord = 0;
  startWindow = dataCopy[0][0] - winSize; 
  for (row=0; numWindows; i++){
	// point vector counter to start of window 
    dataVectorRow = nextFirstRecord;
    currentFirstRecord = nextFirstRecord;

    // save array reference to start of window
    startWindow = startWindow + overlapWinSize;

    // populate first half of window
    while (dataCopy[dataVectorRow][0] < (startWindow+overlapWinSize))
	{
        for (col=0; col<3; col++)
		{
				//windowArray(k,i,vecCtr-(currentFirstRecord)) = dataCopy[vecCtr][k];
				windowData[row][col][vecCtr-currentFirstRecord] = dataCopy[dataVectorRow][col];
        }
        dataVectorRow += 1; // increment vector row counter
		} //end while

		// mark start of next window for later use
		nextFirstRecord = dataVectorRow;

		// populate second half of window
		while dataCopy[dataVectorRow][0] < (startWindow+winSize))
		{
			for (col=0; col<3; col++)
			{
				//windowArray(k,i,vecCtr-(currentFirstRecord)) = dataCopy[vecCtr][k];
				windowData[row][col][dataVectorRow-currentFirstRecord] = dataCopy[dataVectorRow][col];
			}
			dataVectorRow += 1; // increment vector counter
	} //end while
  } // end for


  // populate final window with remaining data
  dataVectorRow = nextFirstRecord; 
  currentFirstRecord = nextFirstRecord;
  startWindow = startWindow + winSize;
  while (dataVectorRow < numSamples)
  {
    for (col=0; col<3; col++)
	{
		//windowArray(k,numWindows,vecCtr-(currentFirstRecord)) = dataCopy[vecCtr][k];
		windowData[numWindows][col][dataVectorRow-currentFirstRecord] = dataCopy[dataVectorRow][col];
    }
    dataVectorRow += 1;
  } //end while

	//create cep_windowData populated withthe windowData vector and numWindows for return to the calling place
	cepDatset cep_windowData = new cepDataset(windowData, numWindows);
	return cep_windowData;
	
}//end doWindow

