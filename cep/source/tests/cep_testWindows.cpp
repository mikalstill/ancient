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
#include "../cepConsoleErrorHandler.h"


/**
 * Tests the framework which has been set up to window the incoming data
 *
 * @author Blake Swadling
 * @version $Revision: 1.10 $
 */

namespace {

class MyDataWindower : public cepDataWindower {
public:
  MyDataWindower() : cepDataWindower() {}
  MyDataWindower( const cepWindow type, int size, int overlap )
      : cepDataWindower() {
        setWindowType( type, size, overlap );
      }
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
//    cout << "testing rectangular ... " << endl;
    
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

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "incorrect row count",  size, result.getNumRows() );
    
    for( int i=0; i<result.getNumRows(); ++i ) {

//      cout << "scale=" << result.getValue(i, 0, 0)
//           << "   value=" << result.getValue(i, 1, 0)
//           << "   color=" << result.getValue(i, 2, 0)
//           << endl;
           
      CPPUNIT_ASSERT_EQUAL_MESSAGE( "invalid scale value", (double)i, result.getValue(i, 0, 0) );
      CPPUNIT_ASSERT_EQUAL_MESSAGE( "invalid data value", (double)1, result.getValue(i, 1, 0) );
    }
    
  }

  /** Test the hamming window. Dumps the coefficients to a delimited text file
   * for viewing in your spread sheet of choice
   */
  void testHamming ()
  {
//    cout << "testing hamming ... " << endl;
    int size = 100;
    windower->setWindowType( cepDataWindower::WINDOW_HAMMING, size, 0 );
    
    cepMatrix<double> result;
    windower->window( makeData( size ), result );

    ofstream f("hamming.txt", ios::trunc);
    for( int i=0; i<result.getNumRows(); ++i ) {
      f << result.getValue(i, 0, 0) << ' ' << result.getValue(i, 1, 0) << endl;
    }
    f.close();

  }


  /** Test the hamming window. Dumps the coefficients to a delimited text file
   * for viewing in your spread sheet of choice
   */
  void testBlackman ()
  {
//    cout << "testing blackman ... " << endl;
    int size = 100;
    windower->setWindowType( cepDataWindower::WINDOW_BLACKMAN, size, 0 );

    cepMatrix<double> result;
    windower->window( makeData( size ), result );

    ofstream f("blackman.txt", ios::trunc);
    for( int i=0; i<result.getNumRows(); ++i ) {
      f << result.getValue(i, 0, 0) << ' ' << result.getValue(i, 1, 0) << endl;
    }
    f.close();

  }




  /** Test the hamming window. Dumps the coefficients to a delimited text file
   * for viewing in your spread sheet of choice
   */
  void testChebyshev ()
  {
//    cout << "testing chebyshev ... " << endl;
    int size = 99;
    windower->setWindowType( cepDataWindower::WINDOW_CHEBYSHEV, size, 0 );
    cepMatrix<double> result;
    cepMatrix<double> input = makeData( size );
    cepError err = windower->window( input , result );
    if( err.isReal() ) {
      err.display();
    } 

    ofstream f("chebyshev.txt", ios::trunc);
    for( int i=0; i<result.getNumRows(); ++i ) {
      f << result.getValue(i, 0, 0) << ' ' << result.getValue(i, 1, 0) << endl;
    }
    f.close();

  }

  void testChebyshev2 ()
  {
//    cout << "testing chebyshev ... " << endl;
    int size = 100;
    windower->setWindowType( cepDataWindower::WINDOW_CHEBYSHEV, size, 0 );
    cepMatrix<double> result;
    cepMatrix<double> input = makeData( size );
    cepError err = windower->window( input , result );
    if( err.isReal() ) {
      err.display();
    }

    ofstream f("chebyshev.txt", ios::app);
    f << endl << endl;
    for( int i=0; i<result.getNumCols(); ++i ) {
      f << result.getValue(i, 0, 0) << ' ' << result.getValue(i, 1, 0) << endl;
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
    cepErrorHandler *eh = new cepConsoleErrorHandler();
    cepError::addErrorHandler( *eh );
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
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testChebyshev", &Test::testChebyshev ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testChebyshev2", &Test::testChebyshev2 ) );
    
       
    return suiteOfTests;
  }

}; // end Test



 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );


  

} // end namespace
