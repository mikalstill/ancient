/* 
   Main entry point for the application
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

#define GLOBALS_HERE
#include "core.h"

void ds_progressCallback (int plane, long line);

int
main (int argc, char *argv[])
{
  cepError ce;

  // Define some simple default options
  gOptions.errorDisplay[cepError::sevDebug] = false;
  gOptions.errorDisplay[cepError::sevInformational] = true;
  gOptions.errorDisplay[cepError::sevWarning] = true;
  gOptions.errorDisplay[cepError::sevErrorRecoverable] = true;
  gOptions.errorDisplay[cepError::sevErrorFatal] = true;

  cepDataset myds ("../datasets/mb_ANKR_GPS", ds_progressCallback);
  ce = myds.munch ();
  if (ce.isReal ())
    {
      ce.display ();
      exit (42);
    }

  // Test presentation
  cepPresentation myPres;
  myPres.addDataPoint (1, 42);
  myPres.addDataPoint (3, 45);
  myPres.addDataPoint (4, 46);
  myPres.createBitmap ("foo.pdf");

  // Test date
  cepDate myDate (2002.712329);
  // todo_mikal: this gives the wrong answer - -should be 17 sept 2002
  cout << myDate.toString () << endl;
}

void
ds_progressCallback (int plane, long line)
{
  //cout << "Dataset munch progress: " << plane << ": " << line << endl;
}
