/*
 *   This file holds the logic for the various mathematical functions
 *   so that it can be used in multiple places
 *
 *   Copyright (C) Daniel Fernandez                 2002
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Nick Wheatstone                  2002
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

#include "cepApplicator.h"

cepError
processWindow (cepDataset * ds, const cepWindow wType, string desc, int size,
	       int overlap, string newcfname)
{
  cepMatrix < double >windowed[cepDataset::dirUnknown];

  cepDebugPrint ("Set window type");
  cepDataWindower::setWindowType (wType, size, overlap);

  // Now actually window
  for (int i = 0; i < cepDataset::dirUnknown; i++)
    {
      cepDebugPrint ("Window: " + cepToString (i));
      cepError werr =
	cepDataWindower::window (*ds->getMatrix ((cepDataset::direction) i),
				 windowed[i]);
      if (werr.isReal ())
	{
	  return werr;
	}

      cepDebugPrint ("Number of rows: " +
		     cepToString (windowed[i].getNumRows () - 1));
      if (windowed[i].getValue (windowed[i].getNumRows () - 1, 0, 3) == 0.0)
	{
	  cepError err ("Bad value at the end of the window",
			cepError::sevInformational);
	  return err;
	}
    }

  // Now we can process the results
  cepDebugPrint ("Display results");
  cepDataset newds (&windowed[0], &windowed[1], &windowed[2],
		    ds->getOffset ((cepDataset::direction) 0),
		    ds->getOffset ((cepDataset::direction) 1),
		    ds->getOffset ((cepDataset::direction) 2),
		    ds->getProcHistory () + " : " + desc,
		    ds->getHeader ((cepDataset::direction) 0),
		    ds->getHeader ((cepDataset::direction) 1),
		    ds->getHeader ((cepDataset::direction) 2));
  newds.write (newcfname.c_str ());
  return cepError ();;
}

cepError
processInterp (cepDataset * ds, const int iType, string desc,
	       double sampleRate, string newcfname)
{
  cepMatrix < double >interped[cepDataset::dirUnknown];

  for (int i = 0; i < cepDataset::dirUnknown; i++)
    {
      cepInterp myInterp;
      interped[i] =
	myInterp.doInterp (*ds->getMatrix ((cepDataset::direction) i),
			   sampleRate, iType);
    }

  // Now we can process the results
  cepDataset newds (&interped[0], &interped[1], &interped[2],
		    ds->getOffset ((cepDataset::direction) 0),
		    ds->getOffset ((cepDataset::direction) 1),
		    ds->getOffset ((cepDataset::direction) 2),
		    ds->getProcHistory () + " : " + desc,
		    ds->getHeader ((cepDataset::direction) 0),
		    ds->getHeader ((cepDataset::direction) 1),
		    ds->getHeader ((cepDataset::direction) 2));
  newds.write (newcfname);

  return cepError ();
}


cepError
processInterpLs (cepDataset * ds, double sampleRate, string newcfname)
{
  cepMatrix < double >interped[cepDataset::dirUnknown];

  for (int i = 0; i < cepDataset::dirUnknown; i++)
    {
      cepInterp myInterp;
      interped[i] =
	myInterp.LSinterp (*ds->getMatrix ((cepDataset::direction) i),
			   sampleRate, ds->getB1 ((cepDataset::direction) i),
			   ds->getB2 ((cepDataset::direction) i));
    }

  // Now we can process the results
  cepDataset newds (&interped[0], &interped[1], &interped[2],
		    ds->getOffset ((cepDataset::direction) 0),
		    ds->getOffset ((cepDataset::direction) 1),
		    ds->getOffset ((cepDataset::direction) 2),
		    ds->getProcHistory () + " : LS Interp",
		    ds->getHeader ((cepDataset::direction) 0),
		    ds->getHeader ((cepDataset::direction) 1),
		    ds->getHeader ((cepDataset::direction) 2));
  newds.write (newcfname);
  return cepError ();
}


cepError
processFFT (cepDataset * ds, string newcfname)
{
  cepMatrix < double >ffted[cepDataset::dirUnknown];
  float energies[cepDataset::dirUnknown];

  for (int i = 0; i < cepDataset::dirUnknown; i++)
    {
      cepDebugPrint ("Performing FFT in " + cepToString (i) + " direction");

      // Determine how many items we are going to perform an FFT on
      // It has to be a power of two
      cepDebugPrint ("Dataset contains " +
		     cepToString (ds->getMatrix ((cepDataset::direction) i)->
				  getNumRows ()) + " elements");
      int fftScale = ds->getMatrix ((cepDataset::direction) i)->getNumRows ();
      for (int k = 0;; ++k)
	{
	  // The size is already a power of two
	  if ((1 << k) == fftScale)
	    break;

	  // It is not a power of two
	  if (k == 14 || (1 << k) > fftScale)
	    {
	      // Always go low for now...
	      fftScale = 1 << (k - 1);
	      break;
	    }
	}

      if ((ds->getMatrix ((cepDataset::direction) i)->getNumRows () -
	   fftScale) > (fftScale / 2))
	{
	  cepDebugPrint ("Dataset padded");
	  fftScale *= 2;
	}

      cepDebugPrint ("FFT applied to " + cepToString (fftScale) +
		     " elements");

      // We need to copy add the data across into complex land...
      cepMatrix < ComplexDble > input (fftScale,
				       ds->
				       getMatrix ((cepDataset::direction) i)->
				       getNumCols (),
				       ds->
				       getMatrix ((cepDataset::direction) i)->
				       getNumTables ());

      // We continue the date range with padding
      float last = ds->getMatrix ((cepDataset::direction) i)->
	getValue (ds->getMatrix ((cepDataset::direction) i)->getNumRows () -
		  1, cepDataset::colDate, 0);
      if (ds->getMatrix ((cepDataset::direction) i)->getError ().isReal ())
	{
	  return ds->getMatrix ((cepDataset::direction) i)->getError ();
	}

      float gap = ds->getMatrix ((cepDataset::direction) i)->
	getValue (ds->getMatrix ((cepDataset::direction) i)->getNumRows () -
		  2, cepDataset::colDate, 0) - last;
      if (ds->getMatrix ((cepDataset::direction) i)->getError ().isReal ())
	{
	  return ds->getMatrix ((cepDataset::direction) i)->getError ();
	}

      for (int table = 0;
	   table < ds->getMatrix ((cepDataset::direction) i)->getNumTables ();
	   table++)
	{
	  for (int row = 0; row < fftScale; row++)
	    {
	      for (int col = 0;
		   col <
		   ds->getMatrix ((cepDataset::direction) i)->getNumCols ();
		   col++)
		{
		  // If this entry exists
		  if (row <
		      ds->getMatrix ((cepDataset::direction) i)->
		      getNumRows ())
		    {
		      input.setValue (row, col, table,
				      ds->
				      getMatrix ((cepDataset::direction) i)->
				      getValue (row, col, table));

		      if (ds->getMatrix ((cepDataset::direction) i)->
			  getError ().isReal ())
			{
			  cepDebugPrint
			    ("FFT data conversion, extract from dataset");
			  return ds->getMatrix ((cepDataset::direction) i)->
			    getError ();
			}
		    }
		  else
		    {
		      if (col == cepDataset::colDate)
			{
			  last += gap;
			  input.setValue (row, col, table, last);
			}
		      else
			input.setValue (row, col, table, 0.0);
		    }

		  if (input.getError ().isReal ())
		    {
		      cepDebugPrint ("FFT data conversion, push to input");
		      return input.getError ();
		    }
		}
	    }
	}

      // Make a new matrix to put this into
      cepDebugPrint ("Number of tables " +
		     cepToString (ds->getMatrix ((cepDataset::direction) i)->
				  getNumTables ()));
      ffted[i] =
	cepMatrix < double >((fftScale / 2) - 1,
			     ds->getMatrix ((cepDataset::direction) i)->
			     getNumCols (),
			     ds->getMatrix ((cepDataset::direction) i)->
			     getNumTables ());
      if (ds->getMatrix ((cepDataset::direction) i)->getError ().isReal ())
	{
	  cepDebugPrint ("Error determining size of FFT output");
	  return ds->getMatrix ((cepDataset::direction) i)->getError ();
	}

      // Do the FFT on each table
      for (int table = 0;
	   table < ds->getMatrix ((cepDataset::direction) i)->getNumTables ();
	   table++)
	{
	  // Setup the FFT
	  cepCfft < ComplexDble > myFFT (fftScale);
	  if (myFFT.getError ().isReal ())
	    {
	      cepDebugPrint ("Error from FFT initialization function");
	      return myFFT.getError ();
	    }

	  // Perform the FFT
	  cepMatrix < ComplexDble > output = myFFT.matrixFft (input, 1);
	  if (myFFT.getError ().isReal ())
	    {
	      cepDebugPrint ("Error from FFT calculation function");
	      return myFFT.getError ();
	    }

	  // Now we need to get the data back to what we want
	  cepDebugPrint ("The output matrix is " +
			 cepToString (output.getNumRows ()) + " x " +
			 cepToString (output.getNumCols ()));
	  for (int row = 0; row < output.getNumRows (); row++)
	    {
	      for (int col = 0; col < output.getNumCols (); col++)
		{
		  cepDebugPrint ("Getting output: row = " +
				 cepToString (row) + " col = " +
				 cepToString (col));

		  // The first row of the FFT output is treated separately...
		  if ((row == 0) && (col == 1) && (table == 0))
		    {
		      energies[i] = real (output.getValue (row, col, table));
		    }
		  else if (row != 0)
		    {
		      cepDebugPrint ("Setting output: row = " +
				     cepToString (row) + " col = " +
				     cepToString (col) + " table = " +
				     cepToString (table));
		      ffted[i].setValue (row - 1, col, table,
					 real (output.
					       getValue (row, col, table)));
		      if (output.getError ().isReal ())
			{
			  cepDebugPrint ("FFT data conversion, get output");
			  return output.getError ();
			}

		      if (ffted[i].getError ().isReal ())
			{
			  cepDebugPrint
			    ("FFT data conversion, push to new dataset");
			  return ffted[i].getError ();
			}
		    }
		}
	    }
	}
    }

  // Now we can process the results
  cepDataset newds (&ffted[0], &ffted[1], &ffted[2],
		    ds->getOffset ((cepDataset::direction) 0),
		    ds->getOffset ((cepDataset::direction) 1),
		    ds->getOffset ((cepDataset::direction) 2),
		    ds->getProcHistory () + " : FFT",
		    ds->getHeader ((cepDataset::direction) 0),
		    ds->getHeader ((cepDataset::direction) 1),
		    ds->getHeader ((cepDataset::direction) 2));
  newds.setFreqDomain (true);
  newds.setFreqEnergies (energies[0], energies[1], energies[2]);
  newds.write (newcfname);
  cepDebugPrint ("Finished writing");
  return cepError ();
}

cepError
processLsVCV (cepDataset * ds, int isReweight, cepDataset & normal,
	      cepDataset & residual)
{
  return cepError ();
}

cepError
processLsRW (cepDataset * ds)
{
  return cepError ();
}
