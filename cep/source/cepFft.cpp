
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
#include <stdlib.h>


//todo: generate error when argument is not power of 2.
//what if int too small.
int main (int argc, char *argv[])
{

  int arraySize = atoi(argv[1]);
  cout << "type def....";
  typedef complex <double>Complex;

  cout << "<Complex.......";

  cfft<Complex> FFT32 (arraySize);   // build an operator object
  // The constructor builds tables and places them in the object.


  cout << "<Complex array.......";
  Complex Array[arraySize];


  for (int k = 0; k < arraySize; k++)
  {
    Array[k] = sin (k);
    //debugging output
    cout << "Pre transform: Array[" << k << "]: " << Array[k] << "\n";
  }

  //do the transform
  FFT32.fft (Array);  // forward transform
  cout << "\n";
  double re[arraySize];
  double im[arraySize];
  //debugging output..
  for (int k = 0; k < arraySize; k++)
  {
    cout << "Post transform: Array[" << k << "]: real = " << real(Array[k]) << "  imag = " << imag(Array[k]) << "\n";
  }

  cout << "\n";
  //reverse the transform to check if it's even close to working
  FFT32.ifft (Array);            // reverse transform.

  //debugging output..
  for (int k = 0; k < arraySize; k++)
     cout << "Post inverse transform: Array[" << k << "]: real: " << real(Array[k]) << "  imag = " << imag(Array[k]) << "\n";
}//end main
