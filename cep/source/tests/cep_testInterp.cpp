/***************************************************************************
 * Testing Framework for cepInterp
 *
 * (c) Nick Wheatstone, Kristy Van Der Vlist, Blake Swadling 2002 ..
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

#include "../cepInterp.h"
#include <iomanip>


/**
 * A test template for use when creating a new test suite. I has been
 * declared in the anonymous namespace so that the test does not need
 * to be uniquely named. Simple add your test
 *
 * The method setUp() is called immediately prior to running each test.
 * The method tearDown() is called immediately after running each test.
 * Use these to do any initialisation/finalisation of objects, etc
 * they must be declared public and void/void like so
 *
 *   public:
 *     void setUp( void ) { ... }
 *     void tearDown( void ) { ... }
 *
 * @author <your name here>
 * @version $Revision: 1.9 $ $Date: 2002-11-21 06:28:51 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.8  2002/11/14 04:19:21  u983131
 * Few bits and bobs tweaked
 *
 * Revision 1.5  2002/11/12 22:30:08  u983131
 * Added a new test for divided differences
 *
 * Revision 1.4  2002/11/11 04:55:47  u983131
 * cepInterp - now has fully working and validated natural splines
 * also changed makefile to reinclude it in the build
 *
 * Revision 1.3  2002/11/10 04:49:46  u983131
 * *** empty log message ***
 *
 * Revision 1.2  2002/10/22 06:06:37  u983131
 * Linear interp now through testing an ready to be integrated into the GUI
 *
 * Revision 1.11  2002/09/23 07:32:54  u983131
 *
 *
 *
 */

namespace {
class Test : public CppUnit::TestFixture {

public:
  /* default constructor */
  Test() : CppUnit::TestFixture() {}

  /** setup - run prior to each test */
  void setUp ()
  { /* initialise any resources*/ }

  /** teardown - run after each test */
  void tearDown ()
  { /* free any allocated resources */ }

  /**
   * constructs a test suite.
   * Add your tests to the suite by copying and editing the addTest call
   */
  static CppUnit::Test *suite()
  {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "testinterp" );

    /* REGISTER YOUR TEST HERE */
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Basic linear test", &Test::testLinear ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Equal sample rate test", &Test::testSampleRate ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Window division test", &Test::testWindow ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Window overlap test", &Test::testOverlap ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Nearest neighbour test", &Test::testNearestNeighbour ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Natural spline test 1", &Test::testNatSpline1 ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Natural spline test 2", &Test::testNatSpline2 ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Natural spline test 3", &Test::testNatSpline3 ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Natural spline test 4",	&Test::testNatSpline4 ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Divided difference test 1",	&Test::testDivDiff1 ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Divided difference test 2",	&Test::testDivDiff2 ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Cubic spline test 1", &Test::testCubicSpline1 ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Cubic spline test 2", &Test::testCubicSpline2 ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Cubic spline test 3", &Test::testCubicSpline3 ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "Cubic spline test 4", &Test::testCubicSpline4 ) );


    return suiteOfTests;
  }

protected:
  /**
   * DEFINE YOUR TESTS HERE:
   * make your tests protected since you dont need to expose them
   */

  /** Tests linear interp */
  void testLinear ()
  {
    int rows = 3, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 2 * rows -1, cols );
    cepMatrix<double> finish2( 2 * rows -1, cols );
		cepInterp interpolate;

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols; j++ )
        start.setValue( i, j, (double)i * 2 );

		// populate matrix with expected values
		for (int i=0; i<2*rows-1; i++)
		{
			finish1.setValue(i,0, (double)i);
			for (int j=0; j<cols-1; j++)
			{
				finish2.setValue(i,j, (double)i);
			}
		}

		// generate linear interp
		interpolate.doInterp(start, finish1, LINEAR_INTERP);
    for( int i=0; i<2*rows-1; i++ )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               finish2.getValue(i,j), finish1.getValue(i,j));


  }

	/** Tests that the interp engine can build its own timescale */
  void testSampleRate()
  {
    int rows = 3, cols = 4;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 2 * rows -1, cols );
    cepMatrix<double> finish2( 2 * rows -1 , cols );
		cepInterp interpolate;

    for( int i=0; i<rows; i++ )
    {
        start.setValue( i, 0, 2002.1 + (double)i * 2*0.00273 );
        start.setValue( i, 1, (double)i * 2);
        start.setValue( i, 2, 0.0);
        start.setValue( i, 3, 0.0);
    }


		// populate matrix with expected values
		for (int i=0; i<2*rows-1; i++)
		{
			for (int j=0; j<cols-1; j++)
				finish2.setValue(2002.1 + i*0.00274,j, (double)i);
		}

		// generate linear interp
		finish1 = interpolate.doInterp(start, 1, LINEAR_INTERP);
//		cout << "Back here\n";
//		for (int i=0; i<finish1.getNumRows(); i++)
//		{
//			for (int j=0; j<finish1.getNumCols(); j++)
//				cout << finish1.getValue(i,j);
//			cout << '\n';
//		}

    for( int i=0; i<2*rows-1; i++ )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               finish2.getValue(i,j), finish1.getValue(i,j));


  }

  /** Tests the code for scaling the dataset length to fit a window size */
  void testWindow()
  {
    int rows = 3, cols = 3;
    cepMatrix<double> start( rows, cols );

    cepMatrix<double> finish2( 4 , cols );
		cepInterp interpolate;

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols; j++ )
        start.setValue( i, j, (double)i * 2 );


		int sampleRate = 1;
		int winSize = 4;
		// generate linear interp
    cepMatrix<double> finish1(interpolate.doInterp(start, sampleRate, LINEAR_INTERP, winSize));

//		for (int i=0; i<finish1.getNumRows(); i++)
//		{
//			for (int j=0; j<finish1.getNumCols(); j++)
//				cout << finish1.getValue(i,j);
//			cout << '\n';
//		}

		CPPUNIT_ASSERT_EQUAL_MESSAGE( "Sizes not equal",
				               finish2.getNumRows(), finish1.getNumRows());


  }
  /** Tests the code for scaling the dataset length to fit a window size */
  void testOverlap()
  {
    int rows = 4, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish2( 6 , cols );
		cepInterp interpolate;

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols; j++ )
        start.setValue( i, j, (double)i * 2 );


		int sampleRate = 1;
		int winSize = 4;
		double overlap = 0.5;
		// generate linear interp
    cepMatrix<double> finish1(interpolate.doInterp(start, sampleRate, LINEAR_INTERP, winSize, overlap));

//		for (int i=0; i<finish1.getNumRows(); i++)
//		{
//			for (int j=0; j<finish1.getNumCols(); j++)
//				cout << finish1.getValue(i,j);
//			cout << '\n';
//		}

		CPPUNIT_ASSERT_EQUAL_MESSAGE( "Sizes not equal",
				               finish2.getNumRows(), finish1.getNumRows());


  }

  /** Tests nearest neighbour interp */
  void testNearestNeighbour ()
  {
		int rows = 2, cols = 3;
		int expectedRows = 4;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( expectedRows, cols );
    cepMatrix<double> finish2( expectedRows, cols );
		cepInterp interpolate;

		start.setValue( 0, 0, 0.0 );
		start.setValue( 0, 1, 0.0 );
		start.setValue( 1, 0, 3.0 );
		start.setValue( 1, 1, 3.0 );


		// populate matrix with expected values
		finish2.setValue(0,0, 0.0);
		finish2.setValue(0,1, 0.0);
		finish2.setValue(1,0, 1.0);
		finish2.setValue(1,1, 0.0);
		finish2.setValue(2,0, 2.0);
		finish2.setValue(2,1, 3.0);
		finish2.setValue(3,0, 3.0);
		finish2.setValue(3,1, 3.0);

		// generate linear interp
		finish1 = interpolate.doInterp(start, 1.0, NEAREST_INTERP);


//		for (int i=0; i<finish1.getNumRows(); i++)
//		{
//			for (int j=0; j<finish1.getNumCols(); j++)
//				cout << finish1.getValue(i,j);
//			cout << '\n';
//		}

    for( int i=0; i<2*rows-1; i++ )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               finish2.getValue(i,j), finish1.getValue(i,j));
  }

	/** Tests that a natural spline interpolation back onto the sample */
	/* points get the same value as was started with*/
  void testNatSpline1()
  {
    int rows = 6, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( rows , cols );
		cepInterp interpolate;

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols; j++ )
        start.setValue( i, j, (double)i );

		// generate linear interp
		finish1 = interpolate.doInterp(start, 1, NATURAL_SPLINE_INTERP);

/*		for (int i=0; i<finish1.getNumRows(); i++)
		{
			for (int j=0; j<finish1.getNumCols(); j++)
				cout << finish1.getValue(i,j);
			cout << '\n';
		}
			cout << '\n';*/


/*		for (int i=0; i<start.getNumRows(); i++)
		{
			for (int j=0; j<start.getNumCols(); j++)
				cout << start.getValue(i,j);
			cout << '\n';
		}*/

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               start.getValue(i,j), finish1.getValue(i,j));


  }

  void testNatSpline2()
  {
    int rows = 6, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 2* rows -1 , cols );
    cepMatrix<double> finish2( 2* rows -1 , cols );
		cepInterp interpolate;

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols; j++ )
        start.setValue( i, j, (double)i*2.0 );

    for( int i=0; i<2*rows-1; i++ )
      for( int j=0; j<cols; j++ )
        finish2.setValue( i, j, (double)i );


		// generate linear interp
		finish1 = interpolate.doInterp(start, 1, NATURAL_SPLINE_INTERP);

/*		for (int i=0; i<finish1.getNumRows(); i++)
		{
			for (int j=0; j<finish1.getNumCols(); j++)
				cout << finish1.getValue(i,j) << " ";
			cout << '\n';
		}
			cout << '\n';*/

    for( int i=0; i<11; i+=2 )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               finish2.getValue(i,j), finish1.getValue(i,j));


  }

  void testNatSpline3()
  {
    int rows = 6, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 11 , cols );
    cepMatrix<double> finish2( 2* rows -1 , cols );
		cepInterp interpolate;

    start.setValue( 0, 0, 0.0 );
    start.setValue( 0, 1, 0.0 );
    start.setValue( 1, 0, 2.0 );
    start.setValue( 1, 1, 1.0 );
    start.setValue( 2, 0, 4.0 );
    start.setValue( 2, 1, 2.0 );
    start.setValue( 3, 0, 6.0 );
    start.setValue( 3, 1, 3.0 );
    start.setValue( 4, 0, 8.0 );
    start.setValue( 4, 1, 4.0 );
    start.setValue( 5, 0, 10.0);
    start.setValue( 5, 1, 5.0 );


    finish1.setValue( 0, 0, 0.0 );
    finish1.setValue( 1, 0, 1.0 );
    finish1.setValue( 2, 0, 2.0 );
    finish1.setValue( 3, 0, 3.0 );
    finish1.setValue( 4, 0, 4.0 );
    finish1.setValue( 5, 0, 5.0 );
    finish1.setValue( 6, 0, 6.0 );
    finish1.setValue( 7, 0, 7.0 );
    finish1.setValue( 8, 0, 8.0 );
    finish1.setValue( 9, 0, 9.0 );
    finish1.setValue( 10, 0, 10.0 );

    finish1.setValue( 0, 1, 0.0 );
    finish1.setValue( 1, 1, 0.0 );
    finish1.setValue( 2, 1, 0.0 );
    finish1.setValue( 3, 1, 0.0 );
    finish1.setValue( 4, 1, 0.0 );
    finish1.setValue( 5, 1, 0.0 );
    finish1.setValue( 6, 1, 0.0 );
    finish1.setValue( 7, 1, 0.0 );
    finish1.setValue( 8, 1, 0.0 );
    finish1.setValue( 9, 1, 0.0 );
    finish1.setValue( 10, 1, 0.0 );
    finish1.setValue( 0, 2, 0.0 );
    finish1.setValue( 1, 2, 0.0 );
    finish1.setValue( 2, 2, 0.0 );
    finish1.setValue( 3, 2, 0.0 );
    finish1.setValue( 4, 2, 0.0 );
    finish1.setValue( 5, 2, 0.0 );
    finish1.setValue( 6, 2, 0.0 );
    finish1.setValue( 7, 2, 0.0 );
    finish1.setValue( 8, 2, 0.0 );
    finish1.setValue( 9, 2, 0.0 );
    finish1.setValue( 10, 2, 0.0 );

    finish2.setValue( 0, 0, 0.0 );
    finish2.setValue( 0, 1, 0.0 );
    finish2.setValue( 1, 0, 1.0 );
    finish2.setValue( 1, 1, 0.5 );
    finish2.setValue( 2, 0, 2.0 );
    finish2.setValue( 2, 1, 1.0 );
    finish2.setValue( 3, 0, 3.0 );
    finish2.setValue( 3, 1, 1.5 );
    finish2.setValue( 4, 0, 4.0 );
    finish2.setValue( 4, 1, 2.0 );
    finish2.setValue( 5, 0, 5.0 );
    finish2.setValue( 5, 1, 2.5 );
    finish2.setValue( 6, 0, 6.0 );
    finish2.setValue( 6, 1, 3.0 );
    finish2.setValue( 7, 0, 7.0 );
    finish2.setValue( 7, 1, 3.5 );
    finish2.setValue( 8, 0, 8.0 );
    finish2.setValue( 8, 1, 4.0 );
    finish2.setValue( 9, 0, 9.0 );
    finish2.setValue( 9, 1, 4.5 );
    finish2.setValue( 10, 0, 10.0 );
    finish2.setValue( 10, 1, 5.0 );

    finish2.setValue( 0, 2, 0.0 );
    finish2.setValue( 1, 2, 0.0 );
    finish2.setValue( 2, 2, 0.0 );
    finish2.setValue( 3, 2, 0.0 );
    finish2.setValue( 4, 2, 0.0 );
    finish2.setValue( 5, 2, 0.0 );
    finish2.setValue( 6, 2, 0.0 );
    finish2.setValue( 7, 2, 0.0 );
    finish2.setValue( 8, 2, 0.0 );
    finish2.setValue( 9, 2, 0.0 );
    finish2.setValue( 10, 2, 0.0 );


		// generate linear interp
		interpolate.doInterp(start, finish1, NATURAL_SPLINE_INTERP);


    for( int i=0; i<11; i+=2 )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               finish2.getValue(i,j), finish1.getValue(i,j));


  }

  void testNatSpline4()
  {
    int rows = 6, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 1, 1);
		cepInterp interpolate;

    start.setValue( 0, 0, 0.0 );
    start.setValue( 0, 1, 10.0 );
    start.setValue( 1, 0, 4.0 );
    start.setValue( 1, 1, 6.0 );
    start.setValue( 2, 0, 6.0 );
    start.setValue( 2, 1, 5.0 );
    start.setValue( 3, 0, 7.0 );
    start.setValue( 3, 1, 7.0 );
    start.setValue( 4, 0, 9.0 );
    start.setValue( 4, 1, 2.0 );
    start.setValue( 5, 0, 10.5 );
    start.setValue( 5, 1, 1.0 );



		// generate linear interp
		finish1 = interpolate.doInterp(start, 0.5, NATURAL_SPLINE_INTERP);

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               10.0, finish1.getValue(0,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               6.0, finish1.getValue(8,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               5.0, finish1.getValue(12,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               7.0, finish1.getValue(14,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               2.0, finish1.getValue(18,1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, finish1.getValue(21,1),0.005);


  }


	/** Tests that divided differences can interpolate a line of slope 1 */
  void testDivDiff1()
  {
    int rows = 10, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 2 * rows -1, cols );
    cepMatrix<double> finish2( 2 * rows -1 , cols );
		cepInterp interpolate;

//    cout << "Input values\n";
    for( int i=0; i<rows; i++ )
    {
      for( int j=0; j<cols; j++ )
      {
        start.setValue( i, j, (double)i * 2 );
//        cout << start.getValue(i,j) << " ";
      }
//      cout << '\n';
    }
//    cout << '\n';


		// populate matrix with expected values
		for (int i=0; i<2*rows-1; i++)
		{
			for (int j=0; j<cols-1; j++)
				finish2.setValue(i,j, (double)i);
		}

		// generate linear interp
		finish1 = interpolate.doInterp(start, 1, DIVIDED_INTERP);

/*		for (int i=0; i<finish1.getNumRows(); i++)
		{
			for (int j=0; j<finish1.getNumCols(); j++)
				cout << finish1.getValue(i,j) << " ";
			cout << '\n';
		}*/

    for( int i=0; i<2*rows-1; i++ )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               (double)i, finish1.getValue(i,j));


  }


  void testDivDiff2()
  {
    int rows = 9, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 2 * rows -1, cols );
    cepMatrix<double> finish2( 2 * rows -1 , cols );
		cepInterp interpolate;

    start.setValue( 0, 0, 0.0 );
    start.setValue( 0, 1, 4.0 );
    start.setValue( 1, 0, 2.0 );
    start.setValue( 1, 1, 0.0 );
    start.setValue( 2, 0, 5.0 );
    start.setValue( 2, 1, 4.0 );
    start.setValue( 3, 0, 7.0 );
    start.setValue( 3, 1, 4.0 );
    start.setValue( 4, 0, 9.0 );
    start.setValue( 4, 1, 0.0 );
    start.setValue( 5, 0, 11.0 );
    start.setValue( 5, 1, 4.0 );
    start.setValue( 6, 0, 13.0 );
    start.setValue( 6, 1, 4.0 );
    start.setValue( 7, 0, 15.0 );
    start.setValue( 7, 1, 0.0 );
    start.setValue( 8, 0, 17.0 );
    start.setValue( 8, 1, 4.0 );

/*    cout << "Start Value\n";
    for (int i=0; i<rows; i++)
    {
      cout << start.getValue(i,0) << " "
           << start.getValue(i,1) << '\n';
    }
    cout << '\n';*/

		// populate matrix with expected values
		for (int i=0; i<2*rows-1; i++)
		{
			for (int j=0; j<cols-1; j++)
				finish2.setValue(i,j, (double)i);
		}

		// generate linear interp
		finish1 = interpolate.doInterp(start, 1, DIVIDED_INTERP);

    cout << "Final values\n";
		for (int i=0; i<finish1.getNumRows(); i++)
		{
			for (int j=0; j<finish1.getNumCols(); j++)
				cout << finish1.getValue(i,j) << " ";
			cout << '\n';
		}

   CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
               4.0, finish1.getValue(0,1));
   CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
               0.0, finish1.getValue(2,1));
   CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
               4.0, finish1.getValue(5,1));
   CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
               4.0, finish1.getValue(7,1));
   CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
               0.0, finish1.getValue(9,1));
   CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
               4.0, finish1.getValue(11,1));
   CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
               4.0, finish1.getValue(13,1));
   CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
               0.0, finish1.getValue(15,1));
   CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
               4.0, finish1.getValue(17,1));

  }

	/** Tests that a natural spline interpolation back onto the sample */
	/* points get the same value as was started with*/
  void testCubicSpline1()
  {
    int rows = 6, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( rows , cols );
		cepInterp interpolate;

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols; j++ )
        start.setValue( i, j, (double)i );

		// generate linear interp
		finish1 = interpolate.doInterp(start, 1, CUBIC_SPLINE_INTERP);

/*		for (int i=0; i<finish1.getNumRows(); i++)
		{
			for (int j=0; j<finish1.getNumCols(); j++)
				cout << finish1.getValue(i,j);
			cout << '\n';
		}
			cout << '\n';*/


/*		for (int i=0; i<start.getNumRows(); i++)
		{
			for (int j=0; j<start.getNumCols(); j++)
				cout << start.getValue(i,j);
			cout << '\n';
		}*/

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               start.getValue(i,j), finish1.getValue(i,j));


  }

  void testCubicSpline2()
  {
    int rows = 6, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 2* rows -1 , cols );
    cepMatrix<double> finish2( 2* rows -1 , cols );
		cepInterp interpolate;

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols; j++ )
        start.setValue( i, j, (double)i*2.0 );

    for( int i=0; i<2*rows-1; i++ )
      for( int j=0; j<cols; j++ )
        finish2.setValue( i, j, (double)i );


		// generate linear interp
		finish1 = interpolate.doInterp(start, 1, CUBIC_SPLINE_INTERP);

/*		for (int i=0; i<finish1.getNumRows(); i++)
		{
			for (int j=0; j<finish1.getNumCols(); j++)
				cout << finish1.getValue(i,j) << " ";
			cout << '\n';
		}
			cout << '\n';*/

    for( int i=0; i<11; i+=2 )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               finish2.getValue(i,j), finish1.getValue(i,j));


  }

  void testCubicSpline3()
  {
    int rows = 6, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 11 , cols );
    cepMatrix<double> finish2( 2* rows -1 , cols );
		cepInterp interpolate;

    start.setValue( 0, 0, 0.0 );
    start.setValue( 0, 1, 0.0 );
    start.setValue( 1, 0, 2.0 );
    start.setValue( 1, 1, 1.0 );
    start.setValue( 2, 0, 4.0 );
    start.setValue( 2, 1, 2.0 );
    start.setValue( 3, 0, 6.0 );
    start.setValue( 3, 1, 3.0 );
    start.setValue( 4, 0, 8.0 );
    start.setValue( 4, 1, 4.0 );
    start.setValue( 5, 0, 10.0);
    start.setValue( 5, 1, 5.0 );


    finish1.setValue( 0, 0, 0.0 );
    finish1.setValue( 1, 0, 1.0 );
    finish1.setValue( 2, 0, 2.0 );
    finish1.setValue( 3, 0, 3.0 );
    finish1.setValue( 4, 0, 4.0 );
    finish1.setValue( 5, 0, 5.0 );
    finish1.setValue( 6, 0, 6.0 );
    finish1.setValue( 7, 0, 7.0 );
    finish1.setValue( 8, 0, 8.0 );
    finish1.setValue( 9, 0, 9.0 );
    finish1.setValue( 10, 0, 10.0 );

    finish1.setValue( 0, 1, 0.0 );
    finish1.setValue( 1, 1, 0.0 );
    finish1.setValue( 2, 1, 0.0 );
    finish1.setValue( 3, 1, 0.0 );
    finish1.setValue( 4, 1, 0.0 );
    finish1.setValue( 5, 1, 0.0 );
    finish1.setValue( 6, 1, 0.0 );
    finish1.setValue( 7, 1, 0.0 );
    finish1.setValue( 8, 1, 0.0 );
    finish1.setValue( 9, 1, 0.0 );
    finish1.setValue( 10, 1, 0.0 );
    finish1.setValue( 0, 2, 0.0 );
    finish1.setValue( 1, 2, 0.0 );
    finish1.setValue( 2, 2, 0.0 );
    finish1.setValue( 3, 2, 0.0 );
    finish1.setValue( 4, 2, 0.0 );
    finish1.setValue( 5, 2, 0.0 );
    finish1.setValue( 6, 2, 0.0 );
    finish1.setValue( 7, 2, 0.0 );
    finish1.setValue( 8, 2, 0.0 );
    finish1.setValue( 9, 2, 0.0 );
    finish1.setValue( 10, 2, 0.0 );

    finish2.setValue( 0, 0, 0.0 );
    finish2.setValue( 0, 1, 0.0 );
    finish2.setValue( 1, 0, 1.0 );
    finish2.setValue( 1, 1, 0.5 );
    finish2.setValue( 2, 0, 2.0 );
    finish2.setValue( 2, 1, 1.0 );
    finish2.setValue( 3, 0, 3.0 );
    finish2.setValue( 3, 1, 1.5 );
    finish2.setValue( 4, 0, 4.0 );
    finish2.setValue( 4, 1, 2.0 );
    finish2.setValue( 5, 0, 5.0 );
    finish2.setValue( 5, 1, 2.5 );
    finish2.setValue( 6, 0, 6.0 );
    finish2.setValue( 6, 1, 3.0 );
    finish2.setValue( 7, 0, 7.0 );
    finish2.setValue( 7, 1, 3.5 );
    finish2.setValue( 8, 0, 8.0 );
    finish2.setValue( 8, 1, 4.0 );
    finish2.setValue( 9, 0, 9.0 );
    finish2.setValue( 9, 1, 4.5 );
    finish2.setValue( 10, 0, 10.0 );
    finish2.setValue( 10, 1, 5.0 );

    finish2.setValue( 0, 2, 0.0 );
    finish2.setValue( 1, 2, 0.0 );
    finish2.setValue( 2, 2, 0.0 );
    finish2.setValue( 3, 2, 0.0 );
    finish2.setValue( 4, 2, 0.0 );
    finish2.setValue( 5, 2, 0.0 );
    finish2.setValue( 6, 2, 0.0 );
    finish2.setValue( 7, 2, 0.0 );
    finish2.setValue( 8, 2, 0.0 );
    finish2.setValue( 9, 2, 0.0 );
    finish2.setValue( 10, 2, 0.0 );


		// generate linear interp
		interpolate.doInterp(start, finish1, CUBIC_SPLINE_INTERP);


    for( int i=0; i<11; i+=2 )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               finish2.getValue(i,j), finish1.getValue(i,j));


  }

  void testCubicSpline4()
  {
    int rows = 6, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 1, 1);
		cepInterp interpolate;

    start.setValue( 0, 0, 0.0 );
    start.setValue( 0, 1, 10.0 );
    start.setValue( 1, 0, 4.0 );
    start.setValue( 1, 1, 6.0 );
    start.setValue( 2, 0, 6.0 );
    start.setValue( 2, 1, 5.0 );
    start.setValue( 3, 0, 7.0 );
    start.setValue( 3, 1, 7.0 );
    start.setValue( 4, 0, 9.0 );
    start.setValue( 4, 1, 2.0 );
    start.setValue( 5, 0, 10.5 );
    start.setValue( 5, 1, 1.0 );



		// generate linear interp
		finish1 = interpolate.doInterp(start, 0.5, CUBIC_SPLINE_INTERP);

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               10.0, finish1.getValue(0,1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0, finish1.getValue(8,1),0.005);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               5.0, finish1.getValue(12,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               7.0, finish1.getValue(14,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               2.0, finish1.getValue(18,1));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, finish1.getValue(21,1),0.005);


  }


}; // end test




 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace


