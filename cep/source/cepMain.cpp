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

	      if (sa[0] == "open")
		{
		  err = ds.read (sa[1]);
		  if (err.isReal ())
		    {
		      err.display ();
		    }
		}
	      else if (sa[0] == "plot")
		{
		  float dummyscale1, dummyscale2;
		  long dummyminval1, dummyminval2;
		  long dummyrange;
		  cepDebugPrint ("Started plotting");
		  cepPlot plot (&ds, ds.getDirectionFromName (sa[1]), sa[2],
				700, 300, dummyscale1, dummyscale2,
				dummyminval1, dummyminval2, dummyrange, false,
				false, 0.0, -1);
		  cepDebugPrint ("Finished plotting");
		  if (plot.getFailed ())
		    {
		      cerr << "Plotting failed" << endl;
		    }
		}
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
	      else
		{
		  cepDebugPrint ("Command not found: " + sa[0]);
		  cerr << "Command not found" << endl;
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
