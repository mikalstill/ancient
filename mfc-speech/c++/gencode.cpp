// Generate a codebook

#include "world.h"

int
main (int argc, char *argv[])
{
  mfc data;
  float delta = 0.01;
  float errTarget = 0.0;

  cout << "Delta is " << delta << endl;
  data.clear ();
  load (data, "s014d001.mfc");
  load (data, "s014d002.mfc");
  load (data, "s014d003.mfc");
  load (data, "s017d001.mfc");
  load (data, "s017d002.mfc");
  load (data, "s017d003.mfc");
  load (data, "s029d001.mfc");
  load (data, "s029d002.mfc");
  load (data, "s029d003.mfc");
  cout << "Total frame count: " << data.size () << endl << endl;

  // Initialize the codebook
  codebook cb;
  cb.init (data);
  cb.cloud (data);

  // We fork five times, because we want 32 output codebook entries
  int i;
  for (i = 0; i < 5; i++)
    {
      cout << "------------------------------------------------------" <<
	endl;
      cout << "- CALCULATE THE NEXT SEQUENCE OF CODEBOOK VECTORS    -" <<
	endl;
      cout << "------------------------------------------------------" <<
	endl;
      cb.stats ();

      // Dump the final output of this round
      cout << "Codebook entries are currently:" << endl;
      for (size_t i = 0; i < cb.size (); i++)
	{
	  cout << cb.getLine (i) << endl;
	}
      cout << endl;

      cb.fork (delta);
      cb.cloud (data);

      // Recenter whilst the error is outside acceptable bounds
      float err = 42;
      while (err > errTarget)
	{
	  err = cb.recenter ();
	  cb.cloud (data);
	}

      // Donate and recenter again if we have to
      bool needRecenter (false);
      while (cb.emptyCount () > 0)
	{
	  cout << "Donating" << endl;
	  cb.donate (delta);
	  cb.cloud (data);
	  needRecenter = true;
	}

      if (needRecenter)
	{
	  cout << "Recentering because of donations" << endl;
	  err = 42;
	  while (err > errTarget)
	    {
	      err = cb.recenter ();
	    }
	}

      // Dump the final output of this round
      cout << endl << "Final results from round:" << endl;
      for (size_t i = 0; i < cb.size (); i++)
	{
	  cout << cb.getLine (i) << endl;
	}
      cout << endl;
    }

  cout << "Final codebook size is: " << cb.size () << endl;
  cb.stats ();
  cb.cloudSize ();
  cout << endl;

  cout << cb;
  cout << "------------------------------------------------------" << endl;

  // We need to save the codebook to disc
  cb.save ("codebook.mfc");

  return (0);
}

void
load (mfc & data, string filename)
{
  string result;

  result = data.add (filename);
  if (result != "")
    {
      cout << "File load error: " << filename << ": " << result << endl;
      exit (-1);
    }
}
