
/*************************************************************************
 *cepCfft.h.
 * Template class to be used with GDMS for computing fast fourier transforms.
 * Originally called cfft.h or cplxfft.h taken from FXT (c) by Joerg Arndt,
 * see http://www.jjj.de/fxt/
 *
 * Modifications have been made to this file to be better integrated with the GDMS package.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 675
 * Mass Ave, Cambridge, MA 02139, USA.

 * Original code: Copyright (c) by Joerg Arndt
 * Modifications: Copyright (C) Daniel Fernandez 2002
 */
/******************************************************************************
DOCBOOK START

FUNCTION <!-- class name -->
cepCfft

PURPOSE <!-- use cases -->
computes forward and inverse fft  

SYNOPSIS START
<!-- how to instantiate one? -->
#include <complex.h>
#include <math.h>
#include <cepCfft.h>
typedef complex<double> Complex; //Gnu CC
...
cfft<Complex> FFT256( 256 ); // build an operator object
// The constructor builds tables and places them
//in the object.
Complex Array[256]; 
cepMatrix <Complex> myMatrix(rows,cols);//for matrix fft
 ...
FFT256.fft( Array ); // forward transform
FFT256.ifft( Array ); // reverse transform.

todo: Daniel - update matrix docco here..
SYNOPSIS END

DESCRIPTION START
<!-- description goes here -->
<para>This class can be used in 2 ways..The first is as originally intended. The is: computing an FFT on an array of data. The
second is spceific to GDMS. The FFT is computed on a matrix of data, with the results returned in a complex matrix. The FFT can be
in either direction as specified in the function call.</para>

<para> <!-- per function descriptions -->
<command>FFT.matrixFft (myMatrix, direction);</command>
Computes the FFT on the matrix.
<command>myMatrix</command> The matrix containing the data to be fft'd
<command>direction</command>. int direction. 1 for forward 0 for inverse.
DESCRIPTION END

DOCBOOK END
******************************************************************************/

#if !defined( _CFFT_H_INC__ )
#define _CFFT_H_INC__ 1
#include <math.h>               // for sin and cos
#include <stdio.h>
#include <complex.h>
#include "cepMatrix.h"

typedef complex <double> ComplexDble;

template < class CPLX > class cfft
{
  int N, log2N;                 // these define size of FFT buffer
  CPLX *w;                      // array [N/2] of cos/sin values
  int *bitrev;                  // bit-reversing table, in 0..N
  double fscales[2];            // f-transform scalings
  double iscales[2];            // i-transform scales
  void fft_func (CPLX * buf, int iflag);

public:
  cfft (int size,               // size is power of 2
        double scalef1 = 0.5, double scalef2 = 1.0,     // fwd transform
                                                        // scalings
        double scalei1 = 1.0, double scalei2 = 1.0      // rev xform
    );

  ~cfft ();
  inline void fft (CPLX * buf)  // perform forward fft on buffer
  {
    fft_func (buf, 1); //note: changed from 0 to 1 to effectively change fft sign - Daniel Fernandez
  }
  inline void ifft (CPLX * buf) // perform reverse fft on buffer
  {
    fft_func (buf, 0); //note: changed from 1 to 0 to effectively change fft sign - Daniel Fernandez
  }
  inline int length () const
  {
    return N;
  }

  cepMatrix<ComplexDble> matrixFft(cepMatrix<ComplexDble> & matrix, int dir);

  // used to fill in last half of complex spectrum of real signal
  // when the first half is already there.
  // 
  void hermitian (CPLX * buf);

};                              // class cfft

//////////////////////////////  cfft methods //////////////////////////////

/*
 * constructor takes an int, power-of-2.
 * scalef1,scalef2, are the post-pass and post-transform
 * scalings for the forward transform; scalei1 and scalei2 are
 * the same for the inverse transform.
 */
template < class CPLX >
  cfft < CPLX >::cfft (int size, double scalef1, double scalef2,
                       double scalei1, double scalei2)
{
  int i, j, k;
  double t;

  fscales[0] = scalef1;
  fscales[1] = scalef2;
  iscales[0] = scalei1;
  iscales[1] = scalei2;

  for (k = 0;; ++k)
  {
    if ((1 << k) == size)
      break;
    if (k == 14 || (1 << k) > size)
      throw "cfft: size not power of 2";
  }
  N = 1 << k;
  log2N = k;

  bitrev = new int[N];

  if (k > 0)
    w = new CPLX[N >> 1];
  else
    w = NULL;
  if (bitrev == NULL || ((k > 0) && w == NULL))
    throw "cfft: out of memory";

  // 
  // do bit-rev table
  // 
  bitrev[0] = 0;

  for (j = 1; j < N; j <<= 1)
  {
    for (i = 0; i < j; ++i)
    {
      bitrev[i] <<= 1;
      bitrev[i + j] = bitrev[i] + 1;
    }
  }
  // 
  // prepare the cos/sin table. This is bit-reversed, and goes
  // like this: 0, 90, 45, 135, 22.5 ...  for N/2 entries.
  // 
  if (k > 0)
  {
    CPLX ww;

    k = (1 << (k - 1));
    for (i = 0; i < k; ++i)
    {
      t = double (bitrev[i << 1]) * M_PI / double (k);

      ww = CPLX (cos (t), sin (t));
      w[i] = conj (ww);         // force limiting of imag part if applic.
      // cout << w[i] << "\n";
    }
  }
}

/*
 * destructor frees the memory
 */
template < class CPLX > cfft < CPLX >::~cfft ()
{
  delete[]bitrev;
  if (w != NULL)
    delete[]w;
}

/*
 * hermitian() assumes the array has been filled in with values
 * up to the center and including the center point. It reflects these
 * conjugate-wise into the last half.
 */
template < class CPLX > void cfft < CPLX >::hermitian (CPLX * buf)
{
  int i, j;

  if (N <= 2)
    return;                     // nothing to do
  i = (N >> 1) - 1;             // input
  j = i + 2;                    // output
  while (i > 0)
  {
    buf[j] = conj (buf[i]);
    --i;
    ++j;
  }
}

/*
 *
 */
//template < class CPLX > int cfft < CPLX >::canFft(cepMatrix<ComplexDble> & matrix)
//{
//}
/*
 * cfft::matrixFft
 * imports: matrix: the matrix holding the data to be fft'd
 *        : the column number of the specific column to be fft'd.
          : direction of the transform - 1 = forward, 0 = inverse.
   returns the fft'd data in the matrix whic was originally passed.
 */
template < class CPLX > cepMatrix<ComplexDble>
                        cfft < CPLX >::matrixFft(cepMatrix<ComplexDble> & matrix, int dir)
{
  int numRows = matrix.getNumRows();
  int numCols = matrix.getNumCols();
  int numTables = matrix.getNumTables();
  //int rowCount = 0, colCount = 0;
  int arraySize = numRows;
  int col, row, table,count;
  //int checkValues1, checkValues2;
  
  const int FIRSTCOLUMN = 0;
  //const int FIRSTROW = 0;
  const int NUMCHECKS = 3;
  const double SECSINYEAR = 31557600; //365.25*24*3600 - days*hours*minutes(in seconds)
  double checks[NUMCHECKS];

  ComplexDble arrayToFft[arraySize];
  cepMatrix<ComplexDble> ffteedMatrix( numRows, numCols, numTables); //matrix contain to store processed values

//calculate the frequency scale and place it in the return matrix
  cout << endl
       << "There are " << numRows << " rows; "
       << numCols << " cols; " << numTables
       << " tables." << endl;
  cout << "Calculating frequency scale.." << endl;
  //char junk;
  for (table=0; table < numTables; table++)
  {
    //size of dataset = numRows
    int halfSetSize = (int)(numRows*0.5);
    double sampleRate = ( real( matrix.getValue(0,0,0) )  
                        - real (matrix.getValue(0,1,0) ) )*SECSINYEAR;
    double freq = 1/sampleRate;
    
    cout << "Setting fft matrix scale values ..." << endl;
    cout << "todo Daniel - something wrong with scaling!!!" << endl;
    for(row=0; row< halfSetSize; row++)
    {
    	ffteedMatrix.setValue(row,FIRSTCOLUMN,table, (freq*row)/numRows);
	//cout << ffteedMatrix.getValue(row,FIRSTCOLUMN, table) << endl;
    }
  }
  //cout << "key and enter to continue.." <<endl;
  //cin >> junk;
  cout << endl;
  cout << "Populating array to be fft'd .." << endl;
  //populate the array to send to fft module.
  //start at 1st column as we do not want to fft this.
  for (table = 0; table < numTables; table++)
  {
      
    for (col = 1; col < numCols; col++)
    {//while there are still columns
        for (row = 0; row < numRows; row++)
        {
	    for (count = 0; count < NUMCHECKS; count++)
    	    {
	        checks[count] = real( matrix.getValue(count,col-1,table) );
	    }
	    if ( (checks[0] - checks[1]) != (checks[1] - checks[2]) )
	        ;//todo daniel: throw an error values not equidistant.
            else
	    {
	        arrayToFft[row]= matrix.getValue(row,col,table);;  //populate the arry to be fft'd            
		//cout << row << " - " << arrayToFft[row] << endl;
	    }
        }//end for row
    }//end for col 
    //cout << "key and enter to continue.." <<endl;
    //cin >> junk;

    /*********************************compute the fft.************************************/

    cout << endl;
    if (dir == 1)
    {
        cout << "Performing forward fft on Matrix " << endl;
        fft(arrayToFft);
    }
    else //(dir == 0)
    {
        cout << "Performing Inverse fft on Matrix: " << endl;
        ifft(arrayToFft);
    }
   
    //place the processed values into the marix.
    cout << "Outputting the results for column ";
    for (col =1; col < numCols; col ++)
    {
        cout << col << endl;
        for (row = 0; row < numRows; row++)
	{
             ffteedMatrix.setValue(row,col,table,arrayToFft[row]);
             //cout << "Value for (r,c,t) - (" << row << "," << col << "," << table
	     //<< " .. " << ffteedMatrix.getValue(row,col,table)
	     //<< endl;
         }//
	 cout << endl;
    } //for col
    
  }//end for table
  
  cout << " Returning ffteedMatirx " << endl;
  return ffteedMatrix;
}//end method



/*
 * cfft::fft_func(buf,1) performs a forward fft on the data in the buffer specified.
 * cfft::fft_func(buf,0) performs an inverse fft on the data in the buffer specified.
 * note: reversed iflag - ie reversed sign..we want fft to have negative sign and
 * inverse fft to have positive. Daniel Fernandez
 */
template < class CPLX > void cfft < CPLX >::fft_func (CPLX * buf, int iflag)
{
  int i, j, k;
  CPLX *buf0, *buf2, *bufe;
  CPLX z1, z2, zw;
  double *sp, s;

  sp = iflag ? iscales : fscales;
  s = sp[0];                    // per-pass scale

  if (log2N == 0)
  {                             // only 1 element !
    s = sp[1];
    buf[0] = buf[0] * s;        // final scale only
    return;
  }
  // 
  // first pass:
  // 1st element = sum of 1st & middle, middle element = diff.
  // repeat N/2 times.

  k = N >> 1;

  if (log2N == 1)
    s *= sp[1];                 // final scale

  buf2 = buf + k;
  for (i = 0; i < k; ++i)
  {                             // first pass is faster
    z1 = buf[i] + buf2[i];
    z2 = buf[i] - buf2[i];
    buf[i] = z1 * s;
    buf2[i] = z2 * s;
  }
  if (log2N == 1)
    return;                     // only 2!

  k >>= 1;                      // k is N/4 now
  bufe = buf + N;               // past end
  for (; k; k >>= 1)
  {
    if (k == 1)
    {                           // last pass - include final scale 
      s *= sp[1];               // final scale
    }
    buf0 = buf;
    for (j = 0; buf0 < bufe; ++j)
    {
      if (iflag)
      {
        zw = conj (w[j]);
      }
      else
      {
        zw = w[j];              /* get w-factor */
      }
      buf2 = buf0 + k;
      for (i = 0; i < k; ++i)
      {                         // a butterfly
        z1 = zw * buf2[i];
        z2 = buf0[i] + z1;
        buf2[i] = (buf0[i] - z1) * s;
        buf0[i] = z2 * s;
      }
      buf0 += (k << 1);
    }
  }
  // bitrev the sucker 
  for (i = 0; i < N; ++i)
  {
    j = bitrev[i];
    if (i <= j)
      continue;                 // don't do these
    z1 = buf[i];
    buf[i] = buf[j];
    buf[j] = z1;
  }
}

////////////////////////////// end cfft //////////////////////////////

#endif
