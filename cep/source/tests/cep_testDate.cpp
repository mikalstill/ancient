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
#include "../cepDate.h"

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
      new CppUnit::TestCaller<Test>( "dateTest1", &Test::dateTest1 ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "dateTest2", &Test::dateTest2 ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "dateTest3", &Test::dateTest3 ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "leapYear", &Test::leapYear ) );
  
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "nonLeapYear", &Test::nonLeapYear ) );

    return suiteOfTests;
  }

protected:
  
  /**
   * DEFINE YOUR TESTS HERE:
   * make your tests protected since you dont need to expose them
   */

  /** date test 1. Tests a simple date set */
  void dateTest1 ()
  { 
    cepDate date(2002.0027);
    string year("2002"), 
           month("01"), 
           day("01"), 
           shortDate("01/01/2002"), 
           longDate("01 January 2002");
        
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date.getLongDate());
    
  }
  
  /** date test 2. Tests a simple date */
  void dateTest2 ()
  { 
    cepDate date(2002.9855);
    string year("2002"), 
           month("12"), 
           day("31"), 
           shortDate("31/12/2002"), 
           longDate("31 December 2002");
   
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date.getLongDate());
    
  }

  /** date test 3. Tests a simple date */  
  void dateTest3 ()
  { 
    cepDate date(1973.3996);
    string year("1973"), 
           month("05"), 
           day("28"), 
           shortDate("28/05/1973"), 
           longDate("28 May 1973");
    
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date.getLongDate());
    
  }

  /** date leapYear. Tests for a leap year */  
  void leapYear ()
  { 
    cepDate date(2000.1620);
    string year("2000"), 
           month("02"), 
           day("29"), 
           shortDate("29/02/2000"), 
           longDate("29 February 2000");

    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date.getLongDate());
    
    cepDate date2(2004.1620);
    string year2("2004"), 
           month2("02"), 
           day2("29"), 
           shortDate2("29/02/2004"), 
           longDate2("29 February 2004");
    
   
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year2 == date2.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month2 == date2.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day2 == date2.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate2 == date2.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate2 == date2.getLongDate());
    
  }
  
  /** date nonleapYear. Tests to make sure leap years working */  
  void nonLeapYear ()
  { 
    cepDate date(1999.1620);
    string year("1999"), 
           month("03"), 
           day("01"), 
           shortDate("01/03/1999"), 
           longDate("01 March 1999");
    
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date.getLongDate());
    
    cepDate date2(3000.1620);
    string year2("3000"), 
           month2("03"), 
           day2("01"), 
           shortDate2("01/03/3000"), 
           longDate2("01 March 3000");
     cout << "short date " << date2.getShortDate() << endl;
   
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year2 == date2.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month2 == date2.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day2 == date2.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate2 == date2.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate2 == date2.getLongDate());
    
  }
  

}; // end Test

 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace
