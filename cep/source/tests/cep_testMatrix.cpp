/***************************************************************************
 * Testing Framework for cepMatrix
 *
 * (c) Kristy Van Der Vlist, Blake Swadling 2002 ..
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
 * @version $Revision: 1.5 $ $Date: 2002-08-12 10:08:44 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.3  2002/08/11 07:07:43  u983118
 * updated tests for cepMatrix
 *
 * Revision 1.2  2002/08/07 08:57:45  u983118
 * added more tests to cep_testMatrix. More to come
 *
 * Revision 1.1  2002/08/04 04:04:16  u982087
 * Initial revison
 *
 * simply tests assignment operator
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
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Test" );
    
    /* REGISTER YOUR TEST HERE */
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testAssign", &Test::testAssign ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testAdd", &Test::testAdd ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testSub", &Test::testSub ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testMulMat", &Test::testMulMat ) );
      
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testMulScalar", &Test::testMulScalar ) );

    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testEqu1", &Test::testEqu1 ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testEqu2", &Test::testEqu2 ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testDiag1", &Test::testDiag1 ) );
      
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testDiag2", &Test::testDiag2 ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testTranspose", &Test::testTranspose ) );
          
    return suiteOfTests;
  }

protected:
  /**
   * DEFINE YOUR TESTS HERE:
   * make your tests protected since you dont need to expose them
   */

  /** Tests assignment operator */
  void testAssign ()
  {
     int rows = 3, cols = 3;
    cepMatrix expected( rows, cols );
    cepMatrix actual( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        expected.setValue( i, j, 1 );
      }
    }
    
    expected = actual;
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "value not equal", expected.getValue( i, j ), actual.getValue( i, j ));
      }
    }
  }
  
  /** Tests += operator */
  void testAdd()
  {
    int rows = 3, cols = 3;
    double expected = 3.0;
    
    cepMatrix A( rows, cols ), B( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 1 );
      }
    }
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        B.setValue( i, j, 2 );
      }
    }
    
    A += B;
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "value not equal", expected, A.getValue( i, j ));
      }
    }
  }
  
  /** Tests -= operator */
  void testSub()
  {
    int rows = 3, cols = 3;
    double expected = 3.0;
    
    cepMatrix A( rows, cols ), B( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 10 );
      }
    }
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        B.setValue( i, j, 7 );
      }
    }
    
    A -= B;
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "value not equal", expected, A.getValue( i, j ));
      }
    }
  }

/** Tests *=  matrix operator */
  void testMulMat()
  {
    int aRows = 3, aCols = 3;
    int bRows = 3, bCols = 2;
    double expected = 84.42;
    
    cepMatrix A( aRows, aCols ), B( bRows, bCols );
    
    for( int i=0; i<aRows; i++ ) {
      for( int j=0; j<aCols; j++ ) {
        A.setValue( i, j, 1.4 );
      }
    }
    
    for( int i=0; i<bRows; i++ ) {
      for( int j=0; j<bCols; j++ ) {
        B.setValue( i, j, 20.1 );
      }
    }
    
    A *= B;
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong row size", 3, A.getNumRows());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong col size", 2, A.getNumCols());
    
    for( int i=0; i<A.getNumRows(); i++ ) {
      for( int j=0; j<A.getNumCols(); j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", expected, A.getValue( i, j ));
      }
    }    
  }

/** Tests *=  scalar operator */
  void testMulScalar()
  {
    int rows = 3, cols = 3;
    double s = 1.23, expected = 6.43782;    
    
    cepMatrix A( rows, cols ), B( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 5.234 );
      }
    }
    
    
    A *= s;
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "value not equal", expected, A.getValue( i, j ));
      }
    }
  }
  
  /** Tests == operator  returns true*/
  void testEqu1 ()
  {
    int rows = 3, cols = 3;
    cepMatrix A( rows, cols );
    cepMatrix B( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 1 );
      }
    }
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        B.setValue( i, j, 1 );
      }
    }
    
    CPPUNIT_ASSERT_MESSAGE( "A should be equal to B",A==B );

  }
 
  /** Tests == operator  returns false*/ 
  void testEqu2 ()
  {
    int rows = 3, cols = 3;
    cepMatrix A( rows, cols );
    cepMatrix B( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 1 );
      }
    }
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        B.setValue( i, j, 2 );
      }
    }
    
    CPPUNIT_ASSERT_MESSAGE( "A should not be equal to B", !(A==B));

  }
  
   /** Tests isDiagonal() function  returns true*/
  void testDiag1 ()
  {
    int rows = 3, cols = 3;
    cepMatrix A( rows, cols );
    
    A.setValue(0,0,1);
    A.setValue(0,1,0);
    A.setValue(0,2,0);
    
    A.setValue(1,0,0);
    A.setValue(1,1,1);
    A.setValue(1,2,0);
    
    A.setValue(2,0,0);
    A.setValue(2,1,0);
    A.setValue(2,2,1);
    
    CPPUNIT_ASSERT_MESSAGE( "A should be Diagonal", A.isDiagonal());

  }

/** Tests isDiagonal() function  returns false*/  
  void testDiag2 ()
  {
    int rows = 3, cols = 3;
    cepMatrix A( rows, cols );
    
    A.setValue(0,0,1);
    A.setValue(0,1,0);
    A.setValue(0,2,0);
    
    A.setValue(1,0,0);
    A.setValue(1,1,1);
    A.setValue(1,2,1);
    
    A.setValue(2,0,0);
    A.setValue(2,1,0);
    A.setValue(2,2,1);
    
    CPPUNIT_ASSERT_MESSAGE( "A should not be diagonal", !(A.isDiagonal()));

  } 
  
  /** Tests transpose() function */
  void testTranspose ()
  {
    int rows = 3, cols = 2;
    cepMatrix A( rows, cols );
    
    A.setValue(0,0,1);
    A.setValue(0,1,2);
    
    A.setValue(1,0,3);
    A.setValue(1,1,4);
    
    A.setValue(2,0,5);
    A.setValue(2,1,6);
    
    A.transpose();
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong row size", cols, A.getNumRows());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong col size", rows, A.getNumCols());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1.0, A.getValue(0,0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 3.0, A.getValue(0,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.0, A.getValue(0,2));
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 2.0, A.getValue(1,0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 4.0, A.getValue(1,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 6.0, A.getValue(1,2));
  } 
}; // end Test

 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace
\n
