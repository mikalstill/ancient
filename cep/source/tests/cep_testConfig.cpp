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
#include <functional>

#include "../cepConfiguration.cpp"


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
 * @version $Revision: 1.4 $ $Date: 2002-09-15 10:21:57 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.3  2002/08/28 14:28:58  u982087
 * updated. config test is slightly broken but has offending code removed
 *
 * Revision 1.2  2002/08/28 11:52:36  u982087
 * updated to use the singlton config class
 *
 * Revision 1.1  2002/08/09 14:28:58  u982087
 * An initial test that verifies config
 * This wold be exteded but config has been verified via use.
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
  Test() : CppUnit::TestFixture() {
    cepConfiguration::initialise("testcfg");
    cfg = &cepConfiguration::getInstance();
  }

  /** setup - run prior to each test */
  void setUp () {
  }

  /** teardown - run after each test */
  void tearDown () {
  }

  /**
   * constructs a test suite.
   */
  static CppUnit::Test *suite()
  {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Test" );
    
    /* REGISTER YOUR TEST HERE */
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testParseEntry", &Test::testParseEntry ) );

    return suiteOfTests;
  }

protected:
  typedef cepConfiguration config;
  config *cfg;

  /** tests the parsing of a single entry. sets an entry and reads the value back */
  void testParseEntry()
  {
    string key("name");
    string value("blake");
    string returned;

    
    pair< string, string > vals;

    if( cfg != NULL ) {
      cfg->setValue( key, value);
      cfg->getValue( key, "foo", returned);
    } else {
      cout << "cfg is not initialised??" << endl;
    }
    
    CPPUNIT_ASSERT( returned == value );
  }

}; // end Test

 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace
