/* 
 *   Main entry point for the batch version of GDMS
 *
 *   Copyright (C) Michael Still                    2002
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

#define GLOBALS_HERE
#include "cepCore.h"
#include "cepPresentation.h"
#include "cepPlot.h"
#include "cepStringArray.h"
#include "cepTextErrorHandler.h"
#include "cepApplicator.h"
#include "cepMatrixIO.h"

#include <unistd.h>

int
main (int argc, char *argv[])
{
  gLog.open ("cep.log", ios::out);
  cepErrorHandler *errHandler;
  errHandler = new cepTextErrorHandler ();
  cepError::addErrorHandler (*errHandler);

  // Parse the command line
  string batchfile ("");
  int optchar;
  cepError err;

  while ((optchar = getopt (argc, argv, "b:")) != -1)
    {
      switch (optchar)
	{
	case 'b':
	  batchfile = optarg;
	  break;

	case '?':
	default:
	  err = cepError ("Unknown command line arguement",
			  cepError::sevErrorRecoverable);
	  err.display ();
	  // todo_mikal: A usage message
	}
    }

  // This might also be a batch execution (no UI displays)
  if (batchfile != "")
    {
      fstream commands;
      commands.open (batchfile.c_str (), ios::in);
      if (!commands)
	{
	  cepError err ("Could not open the batch command file: " +
			batchfile, cepError::sevErrorFatal);
	  err.display ();
	}

      string line;
      cepDataset ds;

      while (!commands.eof ())
	{
	  char c;
	  commands.read (&c, 1);

	  if ((c == '\n') && (line.length () > 0))
	    {
	      cepStringArray sa (line, " ");

	      // Open a dataset
	      if (sa[0] == "open")
		{
		  err = ds.read (sa[1]);
		  if (err.isReal ())
		    {
		      err.display ();
		    }
		}

	      // Plot a dataset
	      else if (sa[0] == "plot")
		{
		  float dummyscale1, dummyscale2;
		  long dummyminval1, dummyminval2;
		  long dummyrange;
		  cepDebugPrint ("Started plotting");
		  cepPlot plot (&ds, ds.getDirectionFromName (sa[1]), sa[2],
				700, 300, dummyscale1, dummyscale2,
				dummyminval1, dummyminval2, dummyrange, -1);
		  cepDebugPrint ("Finished plotting");
		  if (plot.getFailed ())
		    {
		      cerr << "Plotting failed" << endl;
		    }
		}
	      
	      // Window a dataset
	      else if (sa[0] == "window")
		{
		  cepError err;
		  int size = atoi(sa[2].c_str());
		  int overlap = atoi(sa[3].c_str());

		  if(sa[1] == "rectangular"){
		    err = processWindow(&ds, cepDataWindower::WINDOW_RECTANGULAR, 
					"Rect window", size, overlap, sa[4]);
		  }
		  else if(sa[1] == "hamming"){
		    err = processWindow(&ds, cepDataWindower::WINDOW_HAMMING, 
					"Hamming window", size, overlap, sa[4]);
		  }
		  else if(sa[1] == "hanning"){
		    err = processWindow(&ds, cepDataWindower::WINDOW_HANNING, 
					"Hanning window", size, overlap, sa[4]);
		  }
		  else if(sa[1] == "blackman"){
		    err = processWindow(&ds, cepDataWindower::WINDOW_BLACKMAN, 
					"Blackman window", size, overlap, sa[4]);
		  }
		  else if(sa[1] == "chebyshev"){
		    err = processWindow(&ds, cepDataWindower::WINDOW_CHEBYSHEV, 
					"Chebyshev window", size, overlap, sa[4]);
		  }
		  else
		    {
		      cepDebugPrint ("Unknown window type: " + sa[1]);
		      cerr << "Unknown window type" << endl;
		      continue;
		    }

		  if(err.isReal()){
		    cout << "Windowing failed: " << err.getMessage() << endl;
		  }
		}

	      // Interpolate a dataset
	      else if (sa[0] == "interp")
		{
		  int iType;
		  string desc;
		  if(sa[1] == "nearest"){
		    iType = NEAREST_INTERP;
		    desc = "Nearest interp";
		  }
		  else if(sa[1] == "linear"){
		    iType = LINEAR_INTERP;
		    desc = "Linear interp";
		  }
		  else if(sa[1] == "nspline"){
		    iType = NATURAL_SPLINE_INTERP;
		    desc = "Natural spline interp";
		  }
		  else if(sa[1] == "cspline"){
		    iType = CUBIC_SPLINE_INTERP;
		    desc = "Cubic spline interp";
		  }
		  else if(sa[1] == "divided"){
		    iType = DIVIDED_INTERP;
		    desc = "Divided interp";
		  }
		  else
		    {
		      cepDebugPrint ("Unknown window type: " + sa[1]);
		      cerr << "Unknown window type" << endl;
		      continue;
		    }

		  cepError err = processInterp(&ds, iType, desc, atof(sa[2].c_str()), sa[3]);
		  if(err.isReal()){
		    cout << "Windowing failed: " << err.getMessage() << endl;
		  }
		}

	      // Perform a FFT on the dataset
	      else if (sa[0] == "fft")
		{
		  cepError err = processFFT(&ds, sa[1]);
		  if(err.isReal()){
		    cout << "FFT failed: " << err.getMessage() << endl;
		  }
		}

	      // Perform a LS regression on the dataset
	      else if(sa[0] == "ls")
		{
		  cepMatrix<double> data[cepDataset::dirUnknown];
		  cepMatrix<double> residuals[cepDataset::dirUnknown];
		  double b1s[cepDataset::dirUnknown];
		  double b2s[cepDataset::dirUnknown];
 
		  if(sa[1] == "auto")
		    {
		      for(int i = 0; i < cepDataset::dirUnknown; i++)
			{
			  cepError err = processLsVCV(&ds, (cepDataset::direction) i,
						      data[i], residuals[i], b1s[i], b2s[i]);
			  if(err.isReal())
			    {
			      cout << "LS failed: " << err.getMessage() << 
				" (failed in direction " << i << ")" << endl;
			      break;
			    }
			}
		    }
		  else
		    {
		      cepStringArray mats(sa[1], ",");
		      for(int i = 0; i < cepDataset::dirUnknown; i++)
			{
			  cepMatrix<double> mat;
			  mat = cepReadMatrix (mats[i]);

			  cepError err = processLsVCV(&ds, (cepDataset::direction) i, mat,
						      data[i], residuals[i], b1s[i], b2s[i]);
			  if(err.isReal())
			    {
			      cout << "LS failed: " << err.getMessage() << 
				" (failed in direction " << i << ")" << endl;
			      break;
			    }
			}
		    }

		  // And now we need to save all this data to the _two_ specified filenames

		  // Actual data
		  {
		    cepDebugPrint("Creating LS data dataset");
		    cepDataset newds(&data[0], &data[1], &data[2],
				     ds.getOffset((cepDataset::direction) 0), 
				     ds.getOffset((cepDataset::direction) 1), 
				     ds.getOffset((cepDataset::direction) 2),
				     ds.getProcHistory() + " : LS VCV", 
				     ds.getHeader((cepDataset::direction) 0), 
				     ds.getHeader((cepDataset::direction) 1), 
				     ds.getHeader((cepDataset::direction) 2),
				     b1s[0], b1s[1], b1s[2], b2s[0], b2s[1], b2s[2],
				     true, true, true);
		    
		    newds.write(sa[2]);
		  }
		  
		  // Residuals
		  {
		    cepDebugPrint("Creating residuals dataset");
		    cepDataset newds(&residuals[0], &residuals[1], &residuals[2], 
				     ds.getOffset((cepDataset::direction) 0), 
				     ds.getOffset((cepDataset::direction) 1), 
				     ds.getOffset((cepDataset::direction) 2),
				     ds.getProcHistory() + " : LS VCV Residuals", 
				     ds.getHeader((cepDataset::direction) 0), 
				     ds.getHeader((cepDataset::direction) 1), 
				     ds.getHeader((cepDataset::direction) 2));
		    
		    newds.write(sa[3]);
		  }
		}

	      // Unknown command
	      else
		{
		  cepDebugPrint ("Command not found: " + sa[0]);
		  cerr << "Command not found: " << sa[0] << endl;
		}

	      // Clear the line, so we can start again...
	      line = "";
	    }
	  else
	    {
	      line += c;
	    }
	}

      cepDebugPrint ("Finished processing batch command file");
      return FALSE;
    }
  else
    {
      cepError err ("You must specify a batch filename\n");
      err.display ();
    }
}
