// ---------------------------------------------------------------------
// Debug functions and error function
// environment variable DEBUGLEVEL is checked and output is only
// produced if the parameter MinLevel is <= DEBUGLEVEL
// ---------------------------------------------------------------------

// Print a message only
void debug
  (const char* Message,		// message to be printed
   const int&  MinLevel);	// minimum debug level required

// Print a message and a short array
void debug
  (const char*  Message,	// message to be printed
   const short* Array,		// array to be printed
   const int&   Length,		// length of Array
   const int&   MinLevel);	// minimum debug level required

// Print a message and an integer array
void debug
  (const char* Message,		// message to be printed
   const int*  Array,		// array to be printed
   const int&  Length,		// length of Array
   const int&  MinLevel);	// minimum debug level required

// Print a message and a float array
void debug
  (const char*  Message,	// message to be printed
   const float* Array,		// array to be printed
   const int&   Length,		// length of Array
   const int&   MinLevel);	// minimum debug level required

// Print a message and a double array
void debug
  (const char*   Message,	// message to be printed
   const double* Array,		// array to be printed
   const int&    Length,	// length of Array
   const int&    MinLevel);// minimum debug level required

// Error function
// Print an error message and exit the program
void error
  (const char* Message);	// message to be printed
