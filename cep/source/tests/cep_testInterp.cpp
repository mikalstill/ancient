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
 * @version $Revision: 1.2 $ $Date: 2002-10-22 06:06:37 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
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

  /** Same as linear test but final timescale is also generated */
  void testSampleRate()
  {
    int rows = 3, cols = 3;
    cepMatrix<double> start( rows, cols );
    cepMatrix<double> finish1( 2 * rows, 2 * cols );
    cepMatrix<double> finish2( 2 * rows -1 , cols );
		cepInterp interpolate;

    for( int i=0; i<rows; i++ )
      for( int j=0; j<cols; j++ )
        start.setValue( i, j, (double)i * 2 );


		// populate matrix with expected values
		for (int i=0; i<2*rows-1; i++)
		{
			for (int j=0; j<cols-1; j++)
				finish2.setValue(i,j, (double)i);
		}

		// generate linear interp
		finish1 = interpolate.doInterp(start, 1, LINEAR_INTERP);

		for (int i=0; i<finish1.getNumRows(); i++)
		{
			for (int j=0; j<finish1.getNumCols(); j++)
				cout << finish1.getValue(i,j);
			cout << '\n';
		}

    for( int i=0; i<2*rows-1; i++ )
      for( int j=0; j<cols-1; j++ )
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "values not equal",
				               finish2.getValue(i,j), finish1.getValue(i,j));


  }

}; // end test


 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace


