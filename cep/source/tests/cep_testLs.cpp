/***************************************************************************
 * Testing Framework for cepLs
 *
 * (c) Kristy Van Der Vlist 2002 ..
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

//#include "../cepMatrix.h"
#include "../cepLs.h"

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
 * @version $Revision: 1.1 $ $Date: 2002-08-11 07:09:40 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
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
      new CppUnit::TestCaller<Test>( "testLs", &Test::testLs ) );
              
    return suiteOfTests;
  }

protected:
  /**
   * DEFINE YOUR TESTS HERE:
   * make your tests protected since you dont need to expose them
   */

  /** Tests Ls */
  void testLs()
  {
    cepMatrix A(4,2), P(4,4), L(4,1);
    cepLs ans;
    
    //define the A matrix
    A.setValue(0,0,2.1);
    A.setValue(1,0,3.7);
    A.setValue(2,0,0.5);
    A.setValue(3,0,1.3);
    
    A.setValue(0,1,1.0);
    A.setValue(1,1,1.0);
    A.setValue(2,1,1.0);
    A.setValue(3,1,1.0);
    
    //define P matrix
    P.setValue(0,0,1);
    P.setValue(1,0,0);
    P.setValue(2,0,0);
    P.setValue(3,0,0);
    
    P.setValue(0,1,0);
    P.setValue(1,1,1);
    P.setValue(2,1,0);
    P.setValue(3,1,0);
    
    P.setValue(0,2,0);
    P.setValue(1,2,0);
    P.setValue(2,2,1);
    P.setValue(3,2,0);
    
    P.setValue(0,3,0);
    P.setValue(1,3,0);
    P.setValue(2,3,0);
    P.setValue(3,3,1);
    
    //define L matrix
    
    L.setValue(0,0,0.5);
    L.setValue(1,0,0.1);
    L.setValue(2,0,1.2);
    L.setValue(3,0,3.1);
    
    ans.cepDoLeastSquares(A, P, L);
    
    //tests for B1 and B2
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.582142857, ans.getB1(), 0.0000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.331071492, ans.getB2(), 0.0000001 );
    
    //tests for residuals 
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.608571429, ans.getResidual(0,0), 0.0000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.277142857, ans.getResidual(1,0), 0.0000001 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.24, ans.getResidual(2,0), 0.0000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.674285714, ans.getResidual(3,0), 0.0000001 );
  }  
}; // end Test

 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace
