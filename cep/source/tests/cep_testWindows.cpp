/***************************************************************************
 * Tests the data windowing framework
 *
 * (c) Blake Swadling, 2002
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
#include <iostream>

#include "../cepDataWindower.h"


/**
 * Tests the framework which has been set up to window the incoming data
 *
 * @author Blake Swadling
 * @version $Revision: 1.2 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.1  2002/08/28 14:28:11  u982087
 * initial revision
 *
 *
 */

namespace {

class MyDataWindower : public cepDataWindower {
public:
  MyDataWindower() : cepDataWindower() {}
  MyDataWindower( const cepDataWindower::windowType type, int size, int overlap )
      : cepDataWindower( type, size, overlap ) {}
  int countWindows( int samples, int winSize, int overlap ) {
    return cepDataWindower::countWindows( samples, winSize, overlap );
  }
};

class Test : public CppUnit::TestFixture {


protected:
  

  /** Test the rectangular window. This test is intended to
   * excercise the system withoput the additional concern of
   * windowing algorithms failing
   */
  void testRectangular ()
  {
    // cout << "testing window ... ";
    
    windower->setWindowType( cepDataWindower::WINDOW_RECTANGULAR, 10, 0 );

    int expected = 10;
    int num = windower->countWindows(  100, 10,  0 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("window number unexpected", expected, num);

    expected = 19;
    num = windower->countWindows(  100, 10,  5 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("window number unexpected", expected, num);

    expected = 6;
    num = windower->countWindows(   10,  5,  4 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("window number unexpected", expected, num);

    expected = 16;
    num = windower->countWindows( 1000, 90, 30 );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("window number unexpected", expected, num);

    // cout << "done" << endl;
    int size = 10;
    cepMatrix<double> result;
    windower->window( makeData( size ), result );

    CPPUNIT_ASSERT_MESSAGE( "incorrect row count",  result.getNumRows() != size );
    
    for( int i=0; i<result.getNumRows(); ++i ) {
      CPPUNIT_ASSERT_EQUAL_MESSAGE( "invalid scale value",  result.getValue(0, i, 0), (double)i );
      CPPUNIT_ASSERT_EQUAL_MESSAGE( "invalid data value",  result.getValue(0, i, 1),  (double)1 );
    }
    
  }

  /** Test the hamming window. Dumps the coefficients to a delimited text file
   * for viewing in your spread sheet of choice
   */
  void testHamming ()
  {
    int size = 100;
    windower->setWindowType( cepDataWindower::WINDOW_HAMMING, size, 0 );
    
    cepMatrix<double> result;
    windower->window( makeData( size ), result );

    ofstream f("windows.txt", ios::trunc);
    for( int i=0; i<result.getNumCols(); ++i ) {
      f << result.getValue(0, i, 0) << ' ' << result.getValue(0, i, 1) << endl;
    }
    f.close();

  }


  /** Test the hamming window. Dumps the coefficients to a delimited text file
   * for viewing in your spread sheet of choice
   */
  void testBlackman ()
  {
    int size = 100;
    windower->setWindowType( cepDataWindower::WINDOW_BLACKMAN, size, 0 );

    cepMatrix<double> result;
    windower->window( makeData( size ), result );

    ofstream f("windows.txt", ios::app);
    f << endl << endl;
    for( int i=0; i<result.getNumCols(); ++i ) {
      f << result.getValue(0, i, 0) << ' ' << result.getValue(0, i, 1) << endl;
    }
    f.close();

  }
  
private:
  MyDataWindower *windower;


  // makes a data set with linear scale (1->n) and data of all 1's
  const cepMatrix<double> & makeData( int size ) {
    cepMatrix<double> *data = new cepMatrix<double>( size, 2 );
    
    for( int i=0; i<size; ++i ) {
      data->setValue(i, 0, i);
      data->setValue(i, 1, 1);
    }
    return *data;
    
  }

public:
  /* default constructor */
  Test() : CppUnit::TestFixture() {
    windower = NULL;
  }

  /** setup - run prior to each test */
  void setUp ()
  {
    windower = new MyDataWindower();
  }

  /** teardown - run after each test */
  void tearDown ()
  {
    delete windower;
  }

  /**
   * constructs a test suite.
   * Add your tests to the suite by copying and editing the addTest call
   */
  static CppUnit::Test *suite()
  {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Test" );

    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testRectangular", &Test::testRectangular ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testHamming", &Test::testHamming ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testBlackman", &Test::testBlackman ) );
       
    return suiteOfTests;
  }

}; // end Test



 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );


  

} // end namespace
