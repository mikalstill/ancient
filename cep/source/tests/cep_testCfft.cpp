/***************************************************************************
 * Testing Framework for cepcepCfft
 *
 * (c) Daniel Fernandez, Blake Swadling 2002 ..
 *   ( or whoever simply removes this comment and steals it for themselves )
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <fstream>
#include <stdio.h>

#include "../cepMatrix.h"
#include "../cepCfft.h"

namespace
{
  class Test:public CppUnit::TestFixture
  {

  public:
    /* default constructor */
    Test ():CppUnit::TestFixture ()
    {
    }

  /** setup - run prior to each test */
    void setUp ()
    {				/* initialise any resources */
    }

  /** teardown - run after each test */
    void tearDown ()
    {				/* free any allocated resources */
    }

  /**
   * constructs a test suite.
   * Add your tests to the suite by copying and editing the addTest call
   */
    static CppUnit::Test * suite ()
    {
      CppUnit::TestSuite * suiteOfTests = new CppUnit::TestSuite ("Test");

      /* REGISTER YOUR TEST HERE */

      suiteOfTests->addTest (new CppUnit::TestCaller < Test >
			     ("test2dFFTcos", &Test::test2dFFTcos));
/*
      suiteOfTests->addTest (new CppUnit::TestCaller < Test >
			     ("test3dForward", &Test::test3dFwdFft));

      suiteOfTests->addTest (new CppUnit::TestCaller < Test >
			     ("test3dInverse", &Test::test3dInverseFFT));
*/
      return suiteOfTests;
    }

  protected:
  /**
   * DEFINE YOUR TESTS HERE:
   * make your tests protected cosce you dont need to expose them
   */

  /** Tests assignment operator */

    void test2dFFTcos ()
    {
      //const double REALTOL = 0.0001;
      //const double IMAGTOL = 0.0001;
      const double MAGTOL = 0.001;
      int numTables = 1;
      int numRows = 256;
      int numCols = 2;
      int cosIndex = 0;
      double year = 2000;
      string prefftfile = "cosarray";
      string postfftfile = "fftarraycos";
      //char junk;
      cepMatrix < ComplexDble > myMatrix (numRows, numCols, numTables);

      // populate column 1 myMatrix with indexes
      for (int row = 0; row < numRows; row++)
	{
	  myMatrix.setValue (row, 0, 0, year);
	  year+=0.002737850787; //increment 1 day
	  if (int(fmod(double(row),double(3))) != 0)
	     year += 0.1;
	  //cout << "Value: " << myMatrix.getValue(row,0,0) << endl;
	}

      cosIndex = 0;

      //populate myMatrix with cos values
      cout << endl << "Test: Testing COS ........" << endl;
      for (int col = 1; col < numCols; col++)
	{
	  for (int row = 0; row < numRows; row++)
	    {
	      myMatrix.setValue (row, col, 0, cos (cosIndex));
	      cosIndex++;
	    }
	}

      cepCfft < ComplexDble > FFT (256);

      myMatrix = FFT.matrixFft (myMatrix, 1);
      //cout << "test: MAtrix returned ok ..outputing fowrad FFT results..." << endl;

      //test matrix contents with file

      ifstream infile (postfftfile.c_str (), ios::in);
      if (!infile.is_open ())
	{
	  cerr << "Could not open " << postfftfile << endl;
	  exit (1);
	}

      ComplexDble fftarraycos[numRows];
      double mag=0,magDif=0;
      int arrayCount = 0;
      while (!infile.eof ())
	{
	  infile >> fftarraycos[arrayCount];
	  arrayCount++;
	}
      infile.close ();
      double realResult = 0;
      double imagResult = 0;
      for (int t = 0; t < numTables; t++)
	{
	  for (int c = 1; c < numCols; c++)
	    {
	      for (int r = 0; r < numRows/2; r++)
		{
		  realResult = real (fftarraycos[r]);
		  imagResult = imag (fftarraycos[r]);
		  mag = sqrt (pow(realResult,2) + pow(imagResult,2) );
                  magDif = mag - real(myMatrix.getValue (r, c, t));
		  CPPUNIT_ASSERT (magDif < MAGTOL);
		}		//for r
	    }			//for c
	}			//end for t

    }				// end 2dfftcos


  };				// end Test


 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
  CPPUNIT_TEST_SUITE_REGISTRATION (Test);

}				// end namespace
