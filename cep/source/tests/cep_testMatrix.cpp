/***************************************************************************
 * Testing Framework for cepMatrix
 *
 * (c) Kristy Van Der Vlist, Blake Swadling, Michael Still 2002
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
 * @version $Revision: 1.21 $ $Date: 2002-11-18 23:47:16 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.20  2002/11/13 06:19:02  u983118
 * added tests for new fuctions
 * cepMatrix is3D and the new methods with get and set of 3d matricies
 *
 * Revision 1.19  2002/11/03 02:52:05  u983118
 * added tests for error stuff
 *
 * Revision 1.18  2002/10/27 03:21:08  u983118
 * added tests for get min and get max fuctions
 *
 * Revision 1.17  2002/10/22 06:16:51  u983118
 * modified tests for new cepMatrix stuff
 *
 * Revision 1.16  2002/10/22 06:04:56  u983118
 * added new tests for resize matrix
 *
 * Revision 1.15  2002/10/20 05:25:26  u983118
 * removed debug output
 *
 * Revision 1.14  2002/10/20 04:36:48  u983118
 * fixed a few things in tests
 *
 * Revision 1.12  2002/10/01 06:19:53  u983118
 * added test for != fuction
 *
 * Revision 1.11  2002/09/18 07:32:54  u983118
 * cleaned some stuff up
 *
 * Revision 1.10  2002/09/09 13:22:52  u983118
 * added updated tests for constructr/destructor and some of the changed fuction defintions for set/get value fuctions
 *
 * Revision 1.9  2002/08/28 11:52:53  u982087
 * removed std output
 *
 * Revision 1.8  2002/08/27 09:07:26  u983118
 * added tests for 3D matrix
 *
 * Revision 1.7  2002/08/24 01:51:02  u983118
 * new tests for cepMatrix template, cepLs core implementation
 *
 * Revision 1.6  2002/08/18 03:12:46  u983118
 * reworked tests for cepMatrix and cepLs
 *
 * Revision 1.5  2002/08/12 10:08:44  u983118
 * fixed probs with previous revision
 *
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

class returnMatrix
{
public:
  returnMatrix() {}
  
  cepMatrix<double> mat3Double()
  {
    int rows = 2, cols = 2, tables = 3;
    cepMatrix<double> A(rows, cols, tables);
    
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,0,1.0);
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,1,2.0);
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,2,3.0);
      }
    }
    
    return A;
  }

  cepMatrix<char> mat3Char()
  {
    int rows = 2, cols = 2, tables = 3;
   
    cepMatrix<char> A(rows, cols, tables);
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,0,'a');
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,1,'b');
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,2,'c');
      }
    }
    
    return A;
  }
  
  cepMatrix<double> mat2Double()
  {
    int rows = 2, cols = 2;
   
    cepMatrix<double> A(rows, cols);
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,1.0);
      }
    }
    return A;
  }

  cepMatrix<char> mat2Char()
  {
    int rows = 2, cols = 2;
   
    cepMatrix<char> A(rows, cols);
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,'a');
      }
    }
    return A;
  }
  
 };
       
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

    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testMax", &Test::testMax ) );

   suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testMax3D", &Test::testMax3D ) );

   suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testMin", &Test::testMin ) );
     
   suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testMin3D", &Test::testMin3D ) );
     
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "test3D", &Test::test3D ) );

    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "test3D", &Test::test3DAssign ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testDestrct", &Test::testDestruct ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "test2DCopyConstructor", &Test::test2DCopyConstructor ) );

   suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "test3DCopyConstructor", &Test::test3DCopyConstructor ) );
   
   suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testResizeLarger", &Test::testResizeLarger ) );

   suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testResizeSmaller", &Test::testResizeSmaller ) );

   suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testIs3D", &Test::testIs3D ) );

   suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testCallForward", &Test::testCallForward ) );

  
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
    cepMatrix<double> expected( rows, cols );
    cepMatrix<double> actual( rows, cols );
    const double blah = 12.8;
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        actual.setValue( i, j, blah );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, actual.getError().isReal());
      }
    }
    
    expected = actual;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, expected.getError().isReal());
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "value not equal", expected.getValue( i, j ), actual.getValue(i,j));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, actual.getError().isReal());
      }
    }
  }
  
  /** Tests += operator */
  void testAdd()
  {
    int rows = 3, cols = 3;
    double expected = 3.0;
    
    cepMatrix<double> A( rows, cols ), B( rows, cols );
   
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 1 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        B.setValue( i, j, 2 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, B.getError().isReal());
      }
    }
    
    A += B;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "value not equal", expected, A.getValue( i, j ));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
  }
  
  /** Tests -= operator */
  void testSub()
  {
    int rows = 3, cols = 3;
    double expected = 3.0;
    
    cepMatrix<double> A( rows, cols ), B( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 10 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        B.setValue( i, j, 7 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, B.getError().isReal());
      }
    }
    
    A -= B;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());

    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "value not equal", expected, A.getValue( i, j ));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
  }

/** Tests *=  matrix operator */
  void testMulMat()
  {
    int aRows = 3, aCols = 3;
    int bRows = 3, bCols = 2;
    double expected = 84.42;
    
    cepMatrix<double> A( aRows, aCols ), B( bRows, bCols );
    
    for( int i=0; i<aRows; i++ ) {
      for( int j=0; j<aCols; j++ ) {
        A.setValue( i, j, 1.4 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for( int i=0; i<bRows; i++ ) {
      for( int j=0; j<bCols; j++ ) {
        B.setValue( i, j, 20.1 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, B.getError().isReal());
      }
    }
    
    A *= B;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong row size", 3, A.getNumRows());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong col size", 2, A.getNumCols());
    
    for( int i=0; i<A.getNumRows(); i++ ) {
      for( int j=0; j<A.getNumCols(); j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", expected, A.getValue( i, j ));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }    
  }

/** Tests *=  scalar operator */
  void testMulScalar()
  {
    int rows = 3, cols = 3;
    double s = 1.23, expected = 6.43782;    
    
    cepMatrix<double> A( rows, cols ), B( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 5.234 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    
    A *= s;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());

    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "value not equal", expected, A.getValue( i, j ));
      }
    }
  }
  
  /** Tests == and != operators  returns true*/
  void testEqu1 ()
  {
    int rows = 3, cols = 3;
    cepMatrix<double> A( rows, cols );
    cepMatrix<double> B( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 1 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        B.setValue( i, j, 1 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, B.getError().isReal());
      }
    }
    
    CPPUNIT_ASSERT_MESSAGE( "A should be equal to B",A==B );
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    CPPUNIT_ASSERT_MESSAGE( "A should be equal to B",!(A!=B));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal()); 
  }
 
  /** Tests == operator  returns false*/ 
  void testEqu2 ()
  {
    int rows = 3, cols = 3;
    cepMatrix<double> A( rows, cols );
    cepMatrix<double> B( rows, cols );
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        A.setValue( i, j, 1 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for( int i=0; i<rows; i++ ) {
      for( int j=0; j<cols; j++ ) {
        B.setValue( i, j, 2 );
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, B.getError().isReal());
      }
    }
    
    CPPUNIT_ASSERT_MESSAGE( "A should not be equal to B", !(A==B));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    CPPUNIT_ASSERT_MESSAGE( "A should not be equal to B", A!=B);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());

  }
  
   /** Tests isDiagonal() function  returns true*/
  void testDiag1 ()
  {
    int rows = 3, cols = 3;
    cepMatrix<double> A( rows, cols );
    
    A.setValue(0,0,1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    A.setValue(0,1,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    A.setValue(0,2,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    
    A.setValue(1,0,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    A.setValue(1,1,1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    A.setValue(1,2,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    
    A.setValue(2,0,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    A.setValue(2,1,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    A.setValue(2,2,1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    
    CPPUNIT_ASSERT_MESSAGE( "A should be Diagonal", A.isDiagonal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal()); 

  }

/** Tests isDiagonal() function  returns false*/  
  void testDiag2 ()
  {
    int rows = 3, cols = 3;
    cepMatrix<double> A( rows, cols );
    
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
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
  } 
  
  /** Tests transpose() function */
  void testTranspose ()
  {
    int rows = 3, cols = 2;
    cepMatrix<double> A( rows, cols );
    
    A.setValue(0,0,1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    A.setValue(0,1,2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    
    A.setValue(1,0,3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    A.setValue(1,1,4);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    
    A.setValue(2,0,5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    A.setValue(2,1,6);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    
    A.transpose();
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong row size", cols, A.getNumRows());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong col size", rows, A.getNumCols());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1.0, A.getValue(0,0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 3.0, A.getValue(0,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.0, A.getValue(0,2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 2.0, A.getValue(1,0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 4.0, A.getValue(1,1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 6.0, A.getValue(1,2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
  } 

  void testMax()
  {
    cepMatrix<double> data(5,3);
    
    //define the data matrix
    //taken from the mb_PMAC_GPS.dat1
    data.setValue(0,0,2000.1589);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,0,2000.1626);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,0,2000.1653);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,0,2000.1680);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,0,2000.1708);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    data.setValue(0,1,-1.6239);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,1,-1.6259);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,1,-1.6255);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,1,-1.6234);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,1,-1.6242);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    data.setValue(0,2,0.0012);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,2,0.0014);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,2,0.0015);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,2,0.0015);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,2,0.0015);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 2000.1708, data.getMaxValue(0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", -1.6234, data.getMaxValue(1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0015, data.getMaxValue(2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
  }

  void testMax3D()
  {
    cepMatrix<double> data(5, 3, 3);
    
    //define the data matrix
    data.setValue(0,0,0,2000.1589);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,0,0,2000.1626);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,0,0,2000.1653);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,0,0,2000.1680);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,0,0,2000.1708);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    data.setValue(0,0,1,1999.1589);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,0,1,1999.1626);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,0,1,1999.1653);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,0,1,1999.1680);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,0,1,1999.1708);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());

    data.setValue(0,0,2,1998.1589);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,0,2,1998.1626);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,0,2,1998.1653);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,0,2,1998.1680);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,0,2,1998.1708);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 2000.1708, data.getMaxValue(0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
  }
  
  void testMin()
  {
    cepMatrix<double> data(5,3);
    
    //define the data matrix
    //taken from the mb_PMAC_GPS.dat1
    data.setValue(0,0,2000.1589);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,0,2000.1626);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,0,2000.1653);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,0,2000.1680);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,0,2000.1708);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    data.setValue(0,1,-1.6239);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,1,-1.6259);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,1,-1.6255);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,1,-1.6234);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,1,-1.6242);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    data.setValue(0,2,0.0012);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,2,0.0014);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,2,0.0015);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,2,0.0015);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,2,0.0015);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 2000.1589, data.getMinValue(0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", -1.6259, data.getMinValue(1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0012, data.getMinValue(2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
  }     
                       
  void testMin3D()
  {
    cepMatrix<double> data(5, 3, 3);
    
    //define the data matrix
    data.setValue(0,0,0,2000.1589);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,0,0,2000.1626);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,0,0,2000.1653);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,0,0,2000.1680);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,0,0,2000.1708);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    data.setValue(0,0,1,1999.1589);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,0,1,1999.1626);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,0,1,1999.1653);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,0,1,1999.1680);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,0,1,1999.1708);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());

    data.setValue(0,0,2,1998.1589);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(1,0,2,1998.1626);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(2,0,2,1998.1653);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(3,0,2,1998.1680);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    data.setValue(4,0,2,1998.1708);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1998.1589, data.getMinValue(0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, data.getError().isReal());
  }

  void test3D()
  {
     
    int rows = 2, cols = 2, tables = 3;
   
    cepMatrix<double> A(rows, cols, tables);
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,0,1.0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,1,2.0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,2,3.0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1.0, A.getValue(i,j,0));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 2.0, A.getValue(i,j,1));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 3.0, A.getValue(i,j,2));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
  }

  void test3DAssign()
  {
    int rows = 2, cols = 2, tables = 3;
   
    cepMatrix<double> E, A(rows, cols, tables);
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,0,1.0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,1,2.0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        A.setValue(i,j,2,3.0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A.getError().isReal());
      }
    }
    
    E = A;
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, E.getError().isReal());

    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong rows", A.getNumRows(), E.getNumRows());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, E.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong cols", A.getNumCols(), E.getNumCols());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, E.getError().isReal());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong tabs", A.getNumTables(), E.getNumTables());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, E.getError().isReal());
        
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1.0, E.getValue(i,j,0));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, E.getError().isReal());
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 2.0, E.getValue(i,j,1));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, E.getError().isReal());
      }
    }
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 3.0, E.getValue(i,j,2));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, E.getError().isReal());
      }
    }
  }
  
  void testDestruct()
  {
    int rows = 3, cols = 3, frames = 2;
    cepMatrix<double> *A = new cepMatrix<double>( rows, cols );
    cepMatrix<double> *B = new cepMatrix<double>;
    cepMatrix<double> *C = new cepMatrix<double>( rows, cols, frames );
    cepMatrix<double> *D = new cepMatrix<double>;
    
    
    A->setValue(0,0,1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A->getError().isReal());
    A->setValue(0,1,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A->getError().isReal());
    A->setValue(0,2,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A->getError().isReal());
    
    A->setValue(1,0,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A->getError().isReal());
    A->setValue(1,1,1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A->getError().isReal());
    A->setValue(1,2,1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A->getError().isReal());
    
    A->setValue(2,0,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A->getError().isReal());
    A->setValue(2,1,0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A->getError().isReal());
    A->setValue(2,2,1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, A->getError().isReal());
    
    for(int i = 0; i < rows; i ++)
    {
      for(int j = 0; j < cols; j ++)
      {
        for(int k = 0; k < frames; k ++)
        {
          C->setValue(i,j,k,1.0);
          CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, C->getError().isReal());
        }
      }
    }
    delete A;
    delete B;
    delete C;
    delete D;
    
  }
  
  void test2DCopyConstructor()
  {
    returnMatrix ret;
    cepMatrix<char> matChar;
    cepMatrix<double> matDouble;
    
    matChar = ret.mat2Char();
    
    for(int i = 0; i < matChar.getNumRows(); i ++){
      for(int j = 0; j < matChar.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 'a', matChar.getValue(i,j));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, matChar.getError().isReal());
      }
    }
    
    matDouble = ret.mat2Double();
    
    for(int i = 0; i < matDouble.getNumRows(); i ++){
      for(int j = 0; j < matDouble.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1.0, matDouble.getValue(i,j));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, matDouble.getError().isReal());
      }
    }
  }
  
  void test3DCopyConstructor()
  {
    returnMatrix ret;
    cepMatrix<char> matChar;
    cepMatrix<double> matDouble;
    
    matChar = ret.mat3Char();
    
    for(int i = 0; i < matChar.getNumRows(); i ++){
      for(int j = 0; j < matChar.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 'a', matChar.getValue(i,j,0));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, matChar.getError().isReal());
      }
    }
    
    for(int i = 0; i < matChar.getNumRows(); i ++){
      for(int j = 0; j < matChar.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 'b', matChar.getValue(i,j,1));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, matChar.getError().isReal());
      }
    }
    
    for(int i = 0; i < matChar.getNumRows(); i ++){
      for(int j = 0; j < matChar.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 'c', matChar.getValue(i,j,2));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, matChar.getError().isReal());
      }
    }
    
    matDouble = ret.mat3Double();
    
    for(int i = 0; i < matDouble.getNumRows(); i ++){
      for(int j = 0; j < matDouble.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1.0, matDouble.getValue(i,j,0));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, matDouble.getError().isReal());
      }
    }
    
    for(int i = 0; i < matDouble.getNumRows(); i ++){
      for(int j = 0; j < matDouble.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 2.0, matDouble.getValue(i,j,1));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, matDouble.getError().isReal());
      }
    }
    
    for(int i = 0; i < matDouble.getNumRows(); i ++){
      for(int j = 0; j < matDouble.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 3.0, matDouble.getValue(i,j,2));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, matDouble.getError().isReal());
      }
    }
  }
  
  void testResizeLarger()
  {
    int rows = 4, cols = 3, newRows = 6;
    cepMatrix<double> actual(rows,cols);
    
    for(int i = 0; i < actual.getNumRows(); i ++){
      for(int j = 0; j < actual.getNumCols(); j ++){
        actual.setValue(i,j,1.0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, actual.getError().isReal());
      }
    }
    
    actual.resize(newRows);
        
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong number of cols", newRows, actual.getNumRows());
    
    for(int i = 0; i < actual.getNumRows(); i ++){
      for(int j = 0; j < actual.getNumCols(); j ++){
        if( i < rows){
          CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1.0, actual.getValue(i,j));
          CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, actual.getError().isReal());
        }
      }
    }
    
    for(int i = 0; i < actual.getNumRows(); i ++){
      for(int j = 0; j < actual.getNumCols(); j ++){
        actual.setValue(i,j,2.0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, actual.getError().isReal());
      }
    }
    
    for(int i = 0; i < actual.getNumRows(); i ++){
      for(int j = 0; j < actual.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 2.0, actual.getValue(i,j));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", false, actual.getError().isReal());
      }
    }
    
  }
  
  void testResizeSmaller()
  {
    int rows = 6, cols = 3, newRows = 4;
    cepMatrix<double> actual(rows,cols);
    
    actual.resize(newRows);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", true, actual.getError().isReal());    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "found error", string("Can not make a matrix smaller"), 
				  actual.getError().getMessage());    
  }
  
  void testIs3D()
  {
    int rows = 6, cols = 3, tables = 3;
    
    cepMatrix<double> mat3D(rows, cols, tables), mat2D(rows,cols);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "this is a 3d matrix", true, mat3D.is3D());
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "this is not a 3d matrix", false, mat2D.is3D());    
  } 
  
  //tests to ensure that you can use the 3D get/set methods on 2D matricies
  void testCallForward()
  {
    int rows = 3, cols = 3;
    
    cepMatrix<int> mat2D(rows,cols);
    
    for(int i = 0; i < mat2D.getNumRows(); i++){
      for(int j = 0; j < mat2D.getNumCols(); j ++){
        mat2D.setValue(i,j, 0, rows*cols);
        CPPUNIT_ASSERT_EQUAL_MESSAGE( mat2D.getError().getMessage(), false, mat2D.getError().isReal());
      }
    }
    
    for(int i = 0; i < mat2D.getNumRows(); i++){
      for(int j = 0; j < mat2D.getNumCols(); j ++){
        CPPUNIT_ASSERT_EQUAL_MESSAGE("wrong value", rows*cols, mat2D.getValue(i,j, 0));
        CPPUNIT_ASSERT_EQUAL_MESSAGE( mat2D.getError().getMessage(), false, mat2D.getError().isReal());
      }
    }
  }
  
}; // end Test

 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace


