
 /* cepFft.cpp - test file for using template class cfft.h
  * cfft.h was taken from FXT (c) by Joerg Arndt, see http://www.jjj.de/fxt/
  * Copyright (C) Daniel Fernandez             2002
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
*/
#include <complex.h>
#include "cfft.h"
#include <stdio.h>
#include <iostream.h>


//topdo: daniel. The results from cepFft.cpp miss those of octave by a factor of 32..
//seems to be because the transforms are arse about..i.e fft is actually ifft.
int
main (void)
{

  int arraySize = 32;
  typedef complex <double>Complex;

  cfft<Complex> FFT32 (arraySize);   // build an operator object
  // The constructor builds tables and places them in the object.

  Complex Array[arraySize];

  //fill the array ...
  for (int k = 0; k < arraySize; k++)
  {
    Array[k] = sin (k);
    //debugging output
    cout << "Pre transform: Array[" << k << "]: " << Array[k] << "\n";
  }

  //do the transform
  FFT32.fft (Array);  // forward transform
  cout << "\n";

  //debugging output..
  for (int k = 0; k < arraySize; k++)
	{
    cout << "Post transform: Array[" << k << "]: " << Array[k] << "\n";
    //the scaling factor of 32 - test make answer right...debugging...interesting??!
    //cout << "Post transform: Array[" << k << "]: " << ((Array[k])*32) << "\n";
  }

  cout << "\n";
  //reverse the transform to check if it's even close to working
  FFT32.fft (Array);            // reverse transform.

  //debugging output..
  for (int k = 0; k < arraySize; k++)
     cout << "Post inverse transform: Array[" << k << "]: " << Array[k] << "\n";
}//end main
