// Dhmm.h: interface for the CDhmm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DHMM_H__90A0E944_65DC_4022_9CF7_2BD0C7E5B851__INCLUDED_)
#define AFX_DHMM_H__90A0E944_65DC_4022_9CF7_2BD0C7E5B851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Mikal: Headers needed for the standard IO functionality, and for the
// vector template. Use the std namespace implicitly.
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


//////////////////////////////////////////////////////////////////////
// Mikal: typedefs for our own datatypes

typedef vector<int> obs;         // Mikal: A typedef of a vector of
                                 // integers, this is done for
                                 // convenience reasons and for the
                                 // benefit of the next line.

typedef vector<obs> obsArray;    // Mikal: A typedef for a vector of
                                 // observation vectors. This
                                 // effectively forms a matrix.

// Mikal: Read the observations from the file specified in the first
// arguement into the observation vector specifed by the second
// arguement (which is passed by reference), and then return the total
// number of observations read from the file.
int ReadObs(char *, obs &);


//////////////////////////////////////////////////////////////////////
// Mikal: Global constants which control the operation of the HMM
// algorithm (the parameters to the algorithm)

const int NSTATES = 5;           // Mikal: The number of states in the HMM

const int NOBS    = 32;          // Mikal: The number of observations

const double NO_CYCLES = 10;     // Mikal: Maximum number of cycles to
                                 // perform seeking stabilization

const double EPSILON = 0.01;     // Mikal: This value is checked to
                                 // to determine if the log likelihood has
                                 // converged (an acceptable level of change)

const double SMALL_NO = 1.0e-10; // Mikal: The threshhold for the minimum
                                 // size of an entry in the scale vector

const double RARE_TRANS = 0.0;   // Mikal: The probability of a 
                                 // rare transition

const double RARE_OBS = 0.1;     // Mikal: The probability of a 
                                 // rare observation

//////////////////////////////////////////////////////////////////////
// Mikal: CDhmm, a class which implements the training of a HMM

class CDhmm  
{
public:
   // Mikal: Prototypes for the constructor and destructor
	CDhmm();
	virtual ~CDhmm();

   // Mikal: Train the HMM based on the observations passed as
   // arguement one. The return value is the number of 
   // reestimation cycles which were needed before the model
   // converged.
   int Train(const obsArray &);

   // Mikal: Calculate the log likelihood of the passed observations
   // and return that value
   double LogLikelihood(const obs &) const;

   // Mikal: File IO routines for persisting the HMM to disc, and
   // reading it back in. Return 0 for failure, and 1 for success
   int Write(const char*) const;
   int Read(const char*);

private:
   double A[NSTATES][NSTATES];   // Mikal: The state transition
                                 // matrix
   double B[NSTATES][NOBS];      // Mikal: The symbol ejection 
                                 // probability matrix
   double pi[NSTATES];           // Mikal: The initial state
                                 // probability vector

   int    acc0;                  // Mikal: Baum Welsh accumulators,
   double acc1[NSTATES];         // used in dhmm.cpp to recalculate
   double acc2[NSTATES][NSTATES];// A, B and pi for each iteration
   double acc3[NSTATES];
   double acc4[NSTATES][NOBS];
   double acc5[NSTATES];

   // Mikal: Train the HMM based on one set of observations,
   // returns the log probability (logp)
   double TrainOne(const obs &);

   // Mikal: Reestimate the values of A, B and pi
   void reestimate();
};

#endif // !defined(AFX_DHMM_H__90A0E944_65DC_4022_9CF7_2BD0C7E5B851__INCLUDED_)
