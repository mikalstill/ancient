/***************************************************************************
 * A Sample test case to demonstrate CppUnit framework
 * This is lifted from the CppUnit cookbook document.
 *
 * (c) Blake Swadling, 2002 ..
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
 * @version $Revision: 1.3 $ $Date: 2002-08-09 14:27:14 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2002/08/04 04:55:44  u982087
 * Added numerous tests to demonstrate the use of each type of assertion
 *
 * Revision 1.1  2002/08/04 04:05:26  u982087
 * tools to build the tests and a sample test
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
      new CppUnit::TestCaller<Test>( "sampleTest", &Test::sampleTest ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "sampleTestAssertTrue", &Test::sampleTestAssertTrue ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "sampleTestAssertTrueWithMessage", &Test::sampleTestAssertTrueWithMessage ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "sampleTestAssertEqual", &Test::sampleTestAssertEqual ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "sampleTestEqualWithMessage", &Test::sampleTestEqualWithMessage ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "sampleTestAssertDoublesEqual", &Test::sampleTestAssertDoublesEqual ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "sampleTestFail", &Test::sampleTestFail ) );

    return suiteOfTests;
  }

protected:


  void throwStdException() {
    throw std::exception();
  }
  
  /**
   * DEFINE YOUR TESTS HERE:
   * make your tests protected since you dont need to expose them
   */

  /** simple test 1. uses a generic assert true macro */
  void sampleTest ()
  { 
    char* foo = (char*)malloc(32);
    CPPUNIT_ASSERT_MESSAGE ("foo failed", foo != 0);
    free(foo);
  }
  
  /* removed
  void sampleTestThrowException ()
  {
    // Asserts that a specific exception was thrown.
    CPPUNIT_TEST_EXCEPTION( throwStdException, std::exception );
  }
  */

  void sampleTestAssertTrue () {
    // Assertions that a condition is true.
    CPPUNIT_ASSERT( 10 == 10);
  }

  void sampleTestAssertTrueWithMessage () {
    // Assertion with a user specified message.
    CPPUNIT_ASSERT_MESSAGE( "bugger, 10 is not equal to 10 :(", 10 == 10 );
  }
 
  void sampleTestAssertEqual () {
    // Asserts that two values are equals.
    CPPUNIT_ASSERT_EQUAL( 10, 10 );
  }
  
  void sampleTestEqualWithMessage () {
    // Asserts that two values are equals, provides additional message on failure.
    CPPUNIT_ASSERT_EQUAL_MESSAGE( "bugger, 10 is not equal to 10 :(" , 10, 10 );
  }

  void sampleTestAssertDoublesEqual () {
    // Macro for primitive value comparisons.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.1415926535789, 3.1415926535790, 0.0000000000001 );
  }

  void sampleTestFail () {
    // Fails with the specified message.
    // CPPUNIT_FAIL( "this test is doing as it is told and failing!" );
  }

}; // end Test

 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace
