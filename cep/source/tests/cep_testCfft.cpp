/***************************************************************************
 * Testing Framework for cepCfft
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
			     ("test2dForward", &Test::test2dFwdFft));
/*
      suiteOfTests->addTest (new CppUnit::TestCaller < Test >
			     ("test2dInverse", &Test::test2dInverseFft));

      suiteOfTests->addTest (new CppUnit::TestCaller < Test >
			     ("test3dForward", &Test::test3dFwdFft));

      suiteOfTests->addTest (new CppUnit::TestCaller < Test >
			     ("test3dInverse", &Test::test3dInverseFft));
*/
      return suiteOfTests;
    }

  protected:
  /**
   * DEFINE YOUR TESTS HERE:
   * make your tests protected since you dont need to expose them
   */

  /** Tests assignment operator */

    /*
    void setup2dfft (cepMatrix < double >&myMatrix)
    {

    }				//setup2dfft

    void setup3dfft ()
    {

      //setup2dfft ();

      //window the 2d matrix   

      //we now have 3d matrix


    }				//setup3dfft

    //  double re[arraySize];
    //  double im[arraySize];
    //debugging output..
    */
    //typedef complex < double >Complex;

    void test2dFwdFft ()
    {
      int numTables = 1;
      int numRows = 256;
      int numCols = 2;
      int sinIndex = 0;
      //char junk;
      cepMatrix < ComplexDble >myMatrix (numRows, numCols, numTables);

      // populate column 1 myMatrix with indexes
      cout << "Populating myMatrix ..." << endl;
           
//      for (int col = 0; col < numCols; col++)
//	{
	  for (int row = 0; row < numRows; row++)
	    {
	      myMatrix.setValue (row, 0, 0,sinIndex);
	      sinIndex++;
	      cout << myMatrix.getValue(row,0,0) << "  ";
	    }
	    cout << endl << endl;
//	}

      sinIndex = 0;

      //populate myMatrix with sin values
      cout << "Populating myMatrix with sinvalues..." << endl << endl;
      for (int col = 1; col < numCols; col++)
	{
	  for (int row = 0; row < numRows; row++)
	    {
	      myMatrix.setValue (row, col, 0, sin (sinIndex));
	      cout << sinIndex << " -> " << myMatrix.getValue(row,col,0)  << "  ";
	      sinIndex++;
	    }
	    cout << endl << endl;
	}

      //cfft<ComplexDble> FFT (256); //nbuild operator object
      cfft < ComplexDble > FFT (256);
      
      //ComplexDble Array[256];
      //cepMatrix < ComplexDble > myComplexMatrix(numRows,numCols,numTables);

      //....
      //FFT.fft (Array);
      //FFT.ifft (Array);
      myMatrix = FFT.matrixFft (myMatrix, 1);
      cout << "MAtrix returned ok ..outputing results..." << endl;
      
      for (int t = 0; t < numTables; t++)
      {
        for (int c =1; c < numCols; c ++)
        {
          for (int r = 0; r < numRows; r++)
	  {
             cout << "Value for (r,c,t) - (" << r << "," << c << "," << t
	     << " .. " << myMatrix.getValue(r,c,t)
	     << endl;
          }//for r
	 
	 cout << endl;
        } //for c
      }//end for t
      
      CPPUNIT_ASSERT_EQUAL_MESSAGE( "Poo bear", 1, 1);
    }// 
    /*
    void test2dInverseFft ()
    {
      //setup2dfft ();
      //do the transform
      //cepCfft.matrixFft (myMatrix, 0);
    }

    void test3dFwdFft ()
    {

      //setup3dfft ();
      //myWindows = new cepDataWindower ();
      //myWindows.cepDataWindower (WINDWOW_RECTANGULAR, 512, 0);
      //do the transform
      //cepCfft.matrixFft (myMatrix, 1);
    }

    void test3dInverseFft ()
    {
      //setup2dfft ();
      //do the transform
      //cepCfft.matrixFft (myMatrix, 0);
    }
    */

  };				// end Test


 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
  CPPUNIT_TEST_SUITE_REGISTRATION (Test);

}// end namespace
