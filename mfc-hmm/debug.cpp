// ---------------------------------------------------------------------
// Debug functions
// environment variable DEBUGLEVEL is checked and output is only
// produced if the parameter MinLevel is <= DEBUGLEVEL
// ---------------------------------------------------------------------

#define DEBUGLEVEL 0
#include "debug.h"
#include <iostream.h>
#include <iomanip.h>
#include <stdlib.h>

// ---------------------------------------------------------------------
// Print a message only
// ---------------------------------------------------------------------
void debug
  (const char* Message,		// message to be printed
   const int&  MinLevel)	// minimum debug level required
{
   if ( DEBUGLEVEL >= MinLevel )
      cout << Message << endl;
}
// ---------------------------------------------------------------------
// Print a message and a short array
// ---------------------------------------------------------------------
void debug
  (const char*  Message,	// message to be printed
   const short* Array,		// array to be printed
   const int&   Length,		// length of Array
   const int&   MinLevel)	// minimum debug level required
{
   if ( DEBUGLEVEL >= MinLevel )
   {
      cout << Message << endl;
      for (int i = 0; i<Length; i++)
         if ( (i+1)%10 != 0 && i+1 != Length)
            cout << setw(6) << Array[i];
         else
            cout << setw(6) << Array[i] << endl;
   }
}
// ---------------------------------------------------------------------
// Print a message and an integer array
// ---------------------------------------------------------------------
void debug
  (const char* Message,		// message to be printed
   const int*  Array,		// array to be printed
   const int&  Length,		// length of Array
   const int&  MinLevel)	// minimum debug level required
{
   if ( DEBUGLEVEL >= MinLevel )
   {
      cout << Message << endl;
      for (int i = 0; i<Length; i++)
         if ( (i+1)%10 != 0 && i+1 != Length)
            cout << setw(6) << Array[i];
         else
            cout << setw(6) << Array[i] << endl;
   }
}
// ---------------------------------------------------------------------
// Print a message and a float array
// ---------------------------------------------------------------------
void debug
  (const char*  Message,	// message to be printed
   const float* Array,		// array to be printed
   const int&   Length,		// length of Array
   const int&   MinLevel)	// minimum debug level required
{
   if ( DEBUGLEVEL >= MinLevel )
   {
      cout << Message << endl;
      //cout.setf(ios::scientific, ios::floatfield);
      cout.precision(4);
      for (int i = 0; i<Length; i++)
         if ( (i+1)%5 != 0 && i+1 != Length)
            cout << setw(12) << Array[i];
         else
            cout << setw(12) << Array[i] << endl;
   }
}
// ---------------------------------------------------------------------
// Print a message and a double array
// ---------------------------------------------------------------------
void debug
  (const char*   Message,	// message to be printed
   const double* Array,		// array to be printed
   const int&    Length,	// length of Array
   const int&    MinLevel)	// minimum debug level required
{
   if ( DEBUGLEVEL >= MinLevel )
   {
      cout << Message << endl;
      //cout.setf(ios::scientific, ios::floatfield);
      cout.precision(4);
      for (int i = 0; i<Length; i++)
         if ( (i+1)%5 != 0 && i+1 != Length)
            cout << setw(12) << Array[i];
         else
            cout << setw(12) << Array[i] << endl;
   }
}

// ---------------------------------------------------------------------
// Error function
// Prints error message and exits program
// ---------------------------------------------------------------------
void error
  (const char* Message)		// message to be printed
{
   cout << Message << endl;
   exit(1);
}
