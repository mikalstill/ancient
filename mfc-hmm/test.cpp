#include <fstream>
#include "dhmm.h"
#include "debug.h"

///////////////////////////////////////////////////////////////////////////////
// PROGRAM TO TEST AN OBSERVATION AGAINST A DISCRETE HIDDEN MARKOV MODEL
// Usage: DHMMTest scriptfile outfile
//        scriptfile: contains the file names for the observation sequences
//        outfile:    contains the file name for the output of the model
///////////////////////////////////////////////////////////////////////////////

void main(int argc, char *argv[])
{
   ifstream scrFile, obsFile; // File objects
   char obsFileName[100];     // Filename for an observation vector
   obs oneObsVector;          // Single observation vector
   int obsSize;               // Size of observation vector
   obsArray allObsVectors;    // Array of observation vectors
   CDhmm lambda;              // DHMM to be trained
   int noCycles;              // Number of reestimation cycles

   // Test for correct command line
   if(argc != 3)
   {
      cerr << "Usage: DHMMTest scriptfile outfile" << endl;
      exit(1);
   }

   ////////////////////////////////////////////////////////////////////////////
   // PART 1: USE THE SCRIPT FILE TO DETERMINE WHICH <comment>
   ////////////////////////////////////////////////////////////////////////////

   // Open script file containing the file names
   scrFile.open(argv[1], ios::in);
   if(!scrFile)
   {
      cerr << "Cannot open script file " << argv[1] << endl;
      exit(1);
   }

   //For each obs file: open file, read obs vector, close obs file
   while(scrFile >> obsFileName)
   {
      if ((obsSize = ReadObs(obsFileName, oneObsVector)) < 0)
      {
         cerr << "Cannot open observation file " << obsFileName << endl;
         continue;
      }
      else if (obsSize == 0)
      {
         cerr << "Empty observation file " << obsFileName << endl;
         continue;
      }
      else
      {
         allObsVectors.push_back(oneObsVector);
         cout << obsFileName << ": " << obsSize << " observations" << endl;
      }
   }

   ////////////////////////////////////////////////////////////////////////////
   //PART 2: TRAIN AND OUTPUT THE DISCRETE HIDDEN MARKOV MODEL
   ////////////////////////////////////////////////////////////////////////////

   noCycles = lambda.Train(allObsVectors);
   cout << "DHMM trained in " << noCycles << " reestimation cycles" << endl;
   if (lambda.Write(argv[2]))
      cout << "DHMM written to file " << argv[2] << endl;
   else
      cout << "DHMM could not be written" << endl;
}
