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
			     ("test2dFFTsin", &Test::test2dFFTsin));

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
      const double REALTOL = 0.0001;
      const double IMAGTOL = 0.0001;
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
	  year+=0.1;
	  //cout << myMatrix.getValue(row,0,0) << "  ";
	}

      cosIndex = 0;

      //populate myMatrix with cos values
      cout << endl << "Test: Testing COS ........" << endl;
      for (int col = 1; col < numCols; col++)
	{
	  for (int row = 0; row < numRows; row++)
	    {
	      myMatrix.setValue (row, col, 0, cos (cosIndex));
	      //cout << cosIndex << " -> " << myMatrix.getValue(row,col,0)  << endl;
	      cosIndex++;
	    }
	  //cout << endl << endl;
	}

      cfft < ComplexDble > FFT (256);

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
      int arrayCount = 0;
      while (!infile.eof ())
	{
	  infile >> fftarraycos[arrayCount];
	  arrayCount++;
	}
      //cout << "finished COS read **************************" << endl;
      infile.close ();
      //cout << "Forward FFT results for COS..." << endl;
      double realResult = 0;
      double imagResult = 0;
      for (int t = 0; t < numTables; t++)
	{
	  for (int c = 1; c < numCols; c++)
	    {
	      for (int r = 0; r < numRows; r++)
		{
		  realResult =
		    abs (real (fftarraycos[r]) -
			 real (myMatrix.getValue (r, c, t)));
		  imagResult =
		    abs (imag (fftarraycos[r]) -
			 imag (myMatrix.getValue (r, c, t)));
		  //cout << real( fftarraycos[r] ) << " - " << real( myMatrix.getValue(r,c,t));
		  //cout << "  realResult = " << realResult << endl;
		  ///cout << imag( fftarraycos[r] ) << " - " << imag( myMatrix.getValue(r,c,t));
		  //cout << "  imagResult = " << imagResult << endl;
		  //cout << "COS test: Row index: " << r << " - Scale value: " 
		      // << real(myMatrix.getValue(r,0,t)) 
		      //<< " - Value (" << r << "," << c << "," << t << "): "
		      // << myMatrix.getValue(r,c,t) 
		      // << endl; 
		  CPPUNIT_ASSERT (realResult < REALTOL);
		  CPPUNIT_ASSERT (imagResult < IMAGTOL);
		}		//for r
	      //cout << endl;
	    }			//for c
	}			//end for t

      //test inverse fft
      myMatrix = FFT.matrixFft (myMatrix, 0);

      //test matrix contents with file

      ifstream infile1 (prefftfile.c_str (), ios::in);
      if (!infile1.is_open ())
	{
	  cerr << "Could not open " << prefftfile << endl;
	  exit (1);
	}

      double prefftarray[numRows];
      arrayCount = 0;
      while (!infile1.eof ())
	{
	  infile1 >> prefftarray[arrayCount];
	  arrayCount++;
	}
      //cout << "finished COS read **************************" << endl;
      infile1.close ();

      //cout << "testing inverse cos FFT results..." << endl;
      for (int t = 0; t < numTables; t++)
	{
	  for (int c = 1; c < numCols; c++)
	    {
	      for (int r = 0; r < numRows; r++)
		{
		  realResult =
		    abs (prefftarray[r] - real (myMatrix.getValue (r, c, t)));
		  CPPUNIT_ASSERT (realResult < REALTOL);
		  //cout << "test: Value for (r,c,t) - (" << r << "," << c << "," << t
		  //<< " .. " << myMatrix.getValue(r,c,t)
		  //<< endl;
		}		//for r
	      cout << endl;
	    }			//for c
	}			//end for t
    }				// end 2dfftcos

    void test2dFFTsin ()
    {
      const double REALTOL = 0.0001;
      const double IMAGTOL = 0.0001;
      int numTables = 1;
      int numRows = 256;
      int numCols = 2;
      int sinIndex = 0;
      double year=2000;
      string prefftfile = "sinarray";
      string postfftfile = "fftarraysin";
      //char junk;
      cepMatrix < ComplexDble > myMatrix (numRows, numCols, numTables);

      // populate column 1 myMatrix with indexes
      cout << endl << "test: Testing SIN ************************" << endl;
      for (int row = 0; row < numRows; row++)
	{
	  myMatrix.setValue (row, 0, 0, year);
	  year+=0.1;
	  //cout << myMatrix.getValue(row,0,0) << "  ";
	}

      sinIndex = 0;

      //populate myMatrix with cos values
      //cout << "test: Populating myMatrix with sin values..." << endl << endl;
      for (int col = 1; col < numCols; col++)
	{
	  for (int row = 0; row < numRows; row++)
	    {
	      myMatrix.setValue (row, col, 0, sin (sinIndex));
	      //cout << cosIndex << " -> " << myMatrix.getValue(row,col,0)  << endl;
	      sinIndex++;
	    }
	  //cout << endl << endl;
	}

      cfft < ComplexDble > FFT (256);

      myMatrix = FFT.matrixFft (myMatrix, 1);

      /**********************test matrix contents with file*******************/
      ifstream infile (postfftfile.c_str (), ios::in);
      if (!infile.is_open ())
	{
	  cerr << "Could not open " << postfftfile << endl;
	  exit (1);
	}

      ComplexDble fftarraysin[numRows];
      int arrayCount = 0;
      while (!infile.eof ())
	{
	  infile >> fftarraysin[arrayCount];
	  arrayCount++;
	}
      //cout << "finished SIN read *******************************" << endl;
      infile.close ();
      cout << "Forward FFT results for SIN..." << endl;
      double realResult = 0;
      double imagResult = 0;
      for (int t = 0; t < numTables; t++)
	{
	  for (int c = 1; c < numCols; c++)
	    {
	      for (int r = 0; r < numRows; r++)
		{
		  realResult =
		    abs (real (fftarraysin[r]) -
			 real (myMatrix.getValue (r, c, t)));
		  imagResult =
		    abs (imag (fftarraysin[r]) -
			 imag (myMatrix.getValue (r, c, t)));
		  //cout << real( fftarraysin[r] ) << " - " << real( myMatrix.getValue(r,c,t));
		  //cout << "  realResult = " << realResult << endl;
		  //cout << imag( fftarraysin[r] ) << " - " << imag( myMatrix.getValue(r,c,t));
		  //cout << "  imagResult = " << imagResult << endl;
		  if (r < numRows/2)
		  {
		    cout << "SIN test: Row index: " << r << " - Scale value: " 
		         << real(myMatrix.getValue(r,0,t)) 
		         << " - Value (" << r << "," << c << "," << t << "): "
		         << myMatrix.getValue(r,c,t) 
		         << endl; 
		  }
		  CPPUNIT_ASSERT (realResult < REALTOL); //test the real components
		  CPPUNIT_ASSERT (imagResult < IMAGTOL); //test the imaginary components
		}		//for r
	      //cout << endl;
	    }			//for c
	}			//end for t

      //test inverse fft
      myMatrix = FFT.matrixFft (myMatrix, 0);

      //test matrix contents with file

      ifstream infile1 (prefftfile.c_str (), ios::in);
      if (!infile1.is_open ())
	{
	  cerr << "Could not open " << prefftfile << endl;
	  exit (1);
	}

      double prefftarray[numRows];
      arrayCount = 0;
      while (!infile1.eof ())
	{
	  infile1 >> prefftarray[arrayCount];
	  arrayCount++;
	}
      //cout << "finished SIN read ***************************" << endl;
      infile1.close ();

      //cout << "testing inverse FFT results..." << endl;
      for (int t = 0; t < numTables; t++)
	{
	  for (int c = 1; c < numCols; c++)
	    {
	      for (int r = 0; r < numRows; r++)
		{
		  realResult =
		    abs (prefftarray[r] - real (myMatrix.getValue (r, c, t)));
		  CPPUNIT_ASSERT (realResult < REALTOL);
		  //cout << "SIN test: Value for (r,c,t) - (" << r << "," << c << "," << t
		  //<< " .. " << myMatrix.getValue(r,c,t)
		  //<< endl;
		}		//for r
	      //cout << endl;
	    }			//for c
	}			//end for t
    }				// end 2dfftsin



  private:
//    void test2dFwd(cepMatrix <ComplexDble> matrix);
//    void test2dInverse(cepMatrix <ComplexDble> matrix);


  };				// end Test


 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
  CPPUNIT_TEST_SUITE_REGISTRATION (Test);

}				// end namespace
