
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
#include "../cepCfft.h"
#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
   
  //step 1: define complex type... 
  typedef complex <double>Complex;
  int arraySize = 256;
  
  //step 2: build the object - giving it the size you need (power of 2)
  cepCfft<Complex> FFT256 (arraySize);   // build an operator object
  // The constructor builds tables and places them in the object.

  //step 3: declare complex array
  Complex Array[arraySize];

  //step 4: fill the array
  for (int k = 0; k < arraySize; k++)
  {
    Array[k] = sin (k);
  }

  //step 5: do the transform
  FFT256.fft (Array);  // forward transform

  double re[arraySize];
  double im[arraySize];
  //debugging output..
  for (int k = 0; k < arraySize; k++)
  {
    cout << "Post transform: Array[" << k << "]: real = " << real(Array[k]) << "  imag = " << imag(Array[k]) << "\n";
  }

  cout << "\n";
  //reverse the transform to check if it's even close to working
  FFT256.ifft (Array);  // reverse transform.

  //debugging output..
  for (int k = 0; k < arraySize; k++)
    cout << "Post inverse transform: Array[" << k << "]: real: " << real(Array[k]) << "  imag = " << imag(Array[k]) << "\n";
}//end main
