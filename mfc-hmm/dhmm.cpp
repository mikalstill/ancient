// Dhmm.cpp: implementation of the CDhmm class.
//
//////////////////////////////////////////////////////////////////////

#define MAX_OBS_LENGTH 100
#include <iostream>
#include <vector>
#include <math.h>
#include "debug.h"
#include "dhmm.h"
using namespace std;

//////////////////////////////////////////////////////////////////////
// Mikal: Constructor for the CDhmm class. Initializes the state of
// the class to a good starting state. This includes initializing the
// pi vector, as well as the A and B matrices.
//////////////////////////////////////////////////////////////////////

CDhmm::CDhmm()
{
   int i,j;

   // Mikal: Initialize pi, A, and B to reasonable initial values
   for (i=1; i<NSTATES; i++)
   {
      pi[i] = 0.0;
      for (j=0; j<NSTATES; j++)
         A[i][j]=0.0;
      for (j=0; j<NOBS; j++)
         B[i][j]=0.0;
   }
}

CDhmm::~CDhmm()
{
}

////////////////////////////////////////////////////////////////////////
// Mikal: Train the HMM based on all the observations which have been
// collected.
////////////////////////////////////////////////////////////////////////

int CDhmm::Train(const obsArray &allObservations)
{
   int count[NSTATES][NOBS], total[NSTATES]; // <comment>

   int meanDur;                              // Mikal: The total number
                                             // of all coded vectors
                                             // in the observations.
                                             // This is divided by the
                                             // the total number of
                                             // observations to determine
                                             // the mean duration

   double sumlogp, lastsumlogp;              // Mikal: The sum of the
                                             // logp for this cycle and 
                                             // the previous. These are
                                             // used to determine if the
                                             // HMM has converged
   int i,j,v,t,cyc;                          // Mikal: Various counters

   //-------------------------------------------------------------------
   // PART 1 - <comment>
   //-------------------------------------------------------------------

   debug("Train: MODEL INITIALISATION",1);

   // Mikal: Initialize the initial state vector with the values 
   // [1, 0, 0, 0, 0] for a left to right HMM
   pi[0] = 1.0;
   for (i=1; i<NSTATES; i++)
      pi[i] = 0.0;
   debug("pi=",pi,NSTATES,1);

   // Mikal: Determine meanDur (which is the total number of coded vectors
   // in all the observations, and then use that value to initialize the
   // A (state transition) matrix with the probabily of moving from a 
   // given state into the next horizontal state. The probability of remaining
   // in a given state is then the remaining probability after the horizontal
   // transition. The probability is remaining in the final state is always 1.0
   meanDur = 0;
   for (v=0; v<allObservations.size(); v++)
      meanDur += allObservations[v].size();
   for (i=0; i<NSTATES; i++)
      for (j=0; j<NSTATES; j++)
         A[i][j] = 0.0;
   for (i=0; i<NSTATES-1; i++)
   {
      A[i][i+1] = 1.0 / ((double)meanDur/allObservations.size()/NSTATES);
      A[i][i]   = 1.0 - A[i][i+1];
   }
   A[NSTATES-1][NSTATES-1] = 1.0;
   debug("A=",(double *)A,NSTATES*NSTATES,1);

   // Mikal: Initialize the counters and the total for all of the states
   for (i=0; i<NSTATES; i++)
   {
      for (j=0; j<NOBS; j++)
         count[i][j] = 0;
      total[i] = 0;
   }

   // <comment>
   double indexRatio;
   int state, maxT;
   for (v=0; v<allObservations.size(); v++)
   {
      indexRatio = (double)allObservations[v].size() / NSTATES;
      state = 0;
      maxT = ((state+1) * indexRatio) - 0.5;
      for (t=0; t<allObservations[v].size(); t++)
      {
         if (t > maxT)
         {
            state++;
            maxT = ((state+1) * indexRatio) - 0.5;
         }
         count[state][allObservations[v][t]]++;
         total[state]++;
      }
   }
   debug("count=",(int *)count,NSTATES*NOBS,1);

   for (i=0; i<NSTATES; i++)
      for (j=0; j<NOBS; j++)
      {
         // <comment>
         B[i][j] = RARE_OBS / NOBS;

         // <comment>
         if (total[i] > 0)
            B[i][j] += (1 - RARE_OBS) * count[i][j] / total[i];
      }
   debug("B=",(double *)B,NSTATES*NOBS,1);

   //-------------------------------------------------------------------
   // PART 2 - <comment>
   //-------------------------------------------------------------------
   for(cyc=0; cyc<NO_CYCLES; cyc++)
   {
      debug("Train: REESTIMATION CYCLE",&cyc,1,1);

      // Mikal: Initialize the Baum Welsh accumulators
      acc0 = 0;
      for (i=0; i<NSTATES; i++)
      {
         acc1[i] = acc3[i] = acc5[i] = 0.0;
         for (j=0; j<NSTATES; j++)
            acc2[i][j] = 0.0;
         for (j=0; j<NOBS; j++)
            acc4[i][j] = 0.0;
      }

      // <comment>
      sumlogp = 0.0;
      for (v=0; v<allObservations.size(); v++)
      {
         sumlogp += TrainOne(allObservations[v]);
      }
      debug("sum log p",&sumlogp,1,2);

      // <comment>
      reestimate();
      debug("Reestimated A",(double *)A,NSTATES*NSTATES,1);
      debug("Reestimated B",(double *)B,NSTATES*NOBS,1);
      debug("Reestimated PI",pi,NSTATES,1);

      //----------------------------------------------------------------
      // Mikal: If the log likelihood has converged within the tolerance
      // specified by EPSILON, then stop reestimating. We cannot exit
      // until we have done at least two reestimations (cyc = 0 and 1)
      //----------------------------------------------------------------
      if ( cyc > 0 && (sumlogp-lastsumlogp)/abs(lastsumlogp) < EPSILON)
         break;
      lastsumlogp = sumlogp;
   }
   return (cyc<NO_CYCLES) ? (cyc+1) : NO_CYCLES;
}

////////////////////////////////////////////////////////////////////////
// Mikal: Accumulate data from an observation vector
////////////////////////////////////////////////////////////////////////

double CDhmm::TrainOne (const obs &o)
{
   double logp = 0.0;         // <comment>
   double xi, gamma, term;
   int   i, j, t;

   // <comment>
   double **alpha = new double* [o.size()];
   for (t=0; t<o.size(); t++)
      alpha[t] = new double [NSTATES];
   double *beta = new double [NSTATES];
   double *nxtbeta = new double [NSTATES];
   double *scale = new double [o.size()];

   // <comment>
   acc0++;

   // <comment>
   for (t=0; t<o.size(); t++)
   {
      scale[t] = 0.0;
      for (i=0; i<NSTATES; i++)
      {
         if (t==0)
            alpha[t][i] = pi[i] * B[i][o[t]];
         else
         {
            alpha[t][i] = 0.0;
            for (j=0; j<NSTATES; j++)
               alpha[t][i] += alpha[t-1][j] * A[j][i];
            alpha[t][i] *= B[i][o[t]];
         }
         scale[t] += alpha[t][i];
      }
      debug("TrainOne: alpha_unscld=",alpha[t],NSTATES,2);

      if (scale[t] < SMALL_NO)
         scale[t] = SMALL_NO;
      logp += log10(scale[t]);

      for (i=0; i<NSTATES; i++)
         alpha[t][i] /= scale[t];
      debug("TrainOne: alpha_scaled=",alpha[t],NSTATES,2);
      debug("TrainOne: logp=",&logp,1,2);
   }

   debug("TrainOne: scale=",scale,o.size(),2);

   // <comment>
   for (t=o.size()-1; t>=0; t--)
   {
      for (i=0; i<NSTATES; i++)
      {
         if (t == o.size()-1)
         {
            nxtbeta[i] = 1.0;
            gamma = alpha[t][i];
         }
         else
         {
            nxtbeta[i] = 0.0;
            for (j=0; j<NSTATES; j++)
            {
               term        = A[i][j]*B[j][o[t+1]]*beta[j];
               nxtbeta[i] += term;
               xi          = alpha[t][i]*term;
               acc2[i][j] += xi;
            }
            gamma    = alpha[t][i]*nxtbeta[i];
            acc3[i] += gamma;
         }
         debug("TrainOne: gamma  =",&gamma,1,2);

         // <comment>
         acc4[i][o[t]] += gamma;
         acc5[i]       += gamma;
         if (t==0)
            acc1[i] += gamma;
      }

      for (i=0; i<NSTATES; i++)
         beta[i] = nxtbeta[i] / scale[t];
      debug("TrainOne: nxtbeta=",nxtbeta,NSTATES,2);
      debug("TrainOne: beta   =",beta,NSTATES,2);
   }
   debug("TrainOne: acc0",&acc0,1,2);
   debug("TrainOne: acc1",acc1,NSTATES,2);
   debug("TrainOne: acc2",(double *)acc2,NSTATES*NSTATES,2);
   debug("TrainOne: acc3",acc3,NSTATES,2);
   debug("TrainOne: acc4",(double *)acc4,NSTATES*o.size(),2);
   debug("TrainOne: acc5",acc5,NSTATES,2);

   // <comment>
   for (t=0; t<o.size(); t++)
      delete [] alpha[t];
   delete [] alpha;
   delete [] beta;
   delete [] nxtbeta;
   delete [] scale;

   return logp;
}

////////////////////////////////////////////////////////////////////////
// Mikal: Reestimate the values of A, B and pi
////////////////////////////////////////////////////////////////////////

void CDhmm::reestimate ()
{
   int i, j, k;

   if (acc0 != 0)
   {
      // <comment>
      for (i=0; i<NSTATES; i++)
         pi[i] = acc1[i]/acc0;
      debug("REESTIMATE: PI",pi,NSTATES,3);

      // <comment>
      for (i=0; i<NSTATES; i++)
         for (j=0; j<NSTATES; j++)
         {
            // <comment>
            A[i][j] = RARE_TRANS / NSTATES;

            // <comment>
            if (acc3[i] > 0.0)
               A[i][j] += (1 - RARE_TRANS) * acc2[i][j] / acc3[i];
         }
      debug("REESTIMATE: A",(double *)A,NSTATES*NSTATES,3);

      // <comment>
      for (i=0; i<NSTATES; i++)
         for (k=0; k<NOBS; k++)
         {
            // <comment>
            B[i][k] = RARE_OBS / NOBS;

            // <comment>
            if (acc5[i] > 0.0)
               B[i][k] += (1 - RARE_OBS) * acc4[i][k] / acc5[i];
         }
      debug("REESTIMATE: B",(double *)B,NSTATES*NOBS,3);
   }
}

//////////////////////////////////////////////////////////////////////
// <comment>
//////////////////////////////////////////////////////////////////////

double CDhmm::LogLikelihood(const obs &oneObservation) const
{
   return 0.0;
}

//////////////////////////////////////////////////////////////////////
// Mikal: Write an initialized HMM out to disc
//////////////////////////////////////////////////////////////////////

int CDhmm::Write(const char *fileName) const
{
   // Mikal: Open the file. If this has failed, return a value indicating
   // that
   ofstream file(fileName, ios::out);
   if(!file)
      return 0;

   // Mikal: Write the A and B matrices, and the pi vector to the file
   int i, j;
   for (i=0; i<NSTATES; i++)
      for (j=0; j<NSTATES; j++)
         file << A[i][j] << (j<(NSTATES-1) ? ' ' : '\n');
   for (i=0; i<NSTATES; i++)
      for (j=0; j<NOBS; j++)
         file << B[i][j] << (j<(NOBS-1) ? ' ' : '\n');
   for (i=0; i<NSTATES; i++)
      file << pi[i] << '\n';

   // Mikal: Close the file and return a value indicating success
   file.close();
   return 1;
}

//////////////////////////////////////////////////////////////////////
// Mikal: Read a trained HMM from disc. Return value indicates
// success or failure
//////////////////////////////////////////////////////////////////////

int CDhmm::Read(const char *fileName)
{
   // Mikal: Open the file. If this has failed, return a value indicating
   // that
   ofstream file(fileName, ios::in);
   if(!file)
      return 0;

   // Mikal: Read the A and B matrices, and the pi vector from the file
   int i, j;
   for (i=0; i<NSTATES; i++)
     for (j=0; j<NSTATES; j++)
       file >> A[i][j];
   for (i=0; i<NSTATES; i++)
     for (j=0; j<NOBS; j++)
       file >> B[i][j];
   for (i=0; i<NSTATES; i++)
     file >> pi[i];
   
   // Mikal: Close the file and return a value indicating success
   file.close();
   return 1;
}

//////////////////////////////////////////////////////////////////////
// Mikal: Read in an observation file from disc and turn it into a
// vector.
//////////////////////////////////////////////////////////////////////

int ReadObs(char *obsFileName, obs &obsVector)
{
   int *Array;

   // Mikal: Open the observation file for input, handle the error case
   ifstream obsFile(obsFileName, ios::in);
   if(!obsFile)
      return -1;

   // Mikal: Ensure the observation vector is empty, and then read in
   // the values from the file until there are no more values in the
   // file. push_back() appends that value to the end of the vector,
   // handling all memory allocation issues for us.
   obsVector.clear();
   int temp;
   while(obsFile >> temp)
   {
      obsVector.push_back(temp);
   }
   Array = new int[ obsVector.size()];
   for (int i=0; i<obsVector.size(); i++)
      Array[i] = obsVector[i];
   debug("utt=", Array, obsVector.size(), 3);

   // Mikal: Clean up by closing the input file (to avoid file descriptor
   // exhaustion), and return the total number of observations read.
   obsFile.close();
   return obsVector.size();
}
