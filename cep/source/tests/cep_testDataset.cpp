/***************************************************************************
 * A Sample test case to demonstrate CppUnit framework
 * This is lifted from the CppUnit cookbook document.
 *
 * (c) Kristy Van Der Vlist, 2002 ..
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
#include <exception>
#include <string>

#include "../cepMatrix.h"
#include "../cepDataset.h"
#include "../cepError.h"

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
      new CppUnit::TestCaller<Test>( "testGetMatrix", &Test::testGetMatrix ) );
    return suiteOfTests;
  }

protected:

  /**
   * DEFINE YOUR TESTS HERE:
   * make your tests protected since you dont need to expose them
   */

  /** simple test 1. uses a generic assert true macro */
  void testGetMatrix ()
  { 
    cepMatrix <double> matX, matY, matZ;
    string testSet = "../../datasets/mb_CAIR_GPS";
    cepDataset data(testSet);
    
    data.munch();
    
    matX = data.getMatrix(cepDataset::dirX);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 4, matX.getNumRows()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 3, matX.getNumCols());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5274, matX.getValue(0,0)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5301, matX.getValue(1,0)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5329, matX.getValue(2,0)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5356, matX.getValue(3,0)); 
 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", -2.9751, matX.getValue(0,1)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", -2.9769, matX.getValue(1,1)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", -2.9749, matX.getValue(2,1)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", -2.9768, matX.getValue(3,1)); 
  
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0016, matX.getValue(0,2)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0016, matX.getValue(1,2)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0016, matX.getValue(2,2)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0016, matX.getValue(3,2)); 
    
    matY = data.getMatrix(cepDataset::dirY);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 4, matY.getNumRows()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 3, matY.getNumCols());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5274, matY.getValue(0,0)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5301, matY.getValue(1,0)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5329, matY.getValue(2,0)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5356, matY.getValue(3,0)); 
 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.2986, matY.getValue(0,1)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.2990, matY.getValue(1,1)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.2987, matY.getValue(2,1)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.3014, matY.getValue(3,1)); 
  
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0027, matY.getValue(0,2)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0024, matY.getValue(1,2)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0023, matY.getValue(2,2)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0024, matY.getValue(3,2)); 
    
    matZ = data.getMatrix(cepDataset::dirZ);
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 4, matZ.getNumRows()); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 3, matZ.getNumCols());
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5274, matZ.getValue(0,0)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5301, matZ.getValue(1,0)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5329, matZ.getValue(2,0)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 1999.5356, matZ.getValue(3,0)); 
 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.4898, matZ.getValue(0,1)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.4839, matZ.getValue(1,1)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.4870, matZ.getValue(2,1)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 5.4944, matZ.getValue(3,1)); 
  
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0090, matZ.getValue(0,2)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0085, matZ.getValue(1,2)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0082, matZ.getValue(2,2)); 
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "wrong value", 0.0082, matZ.getValue(3,2)); 
    
  }
  
}; // end Test

 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace
