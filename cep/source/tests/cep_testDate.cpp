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
      new CppUnit::TestCaller<Test>( "dateTest4", &Test::dateTest4 ) );

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
    cepDate date(2002.0014);
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
    
    cepDate date1(01, "Jan", 2002);
    double expected = 2002.0014;
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date1.getDecimalDate(), 0.00001);     
    
    cout << "month is " << date1.getMonth() << endl;
    cout << "day is " << date1.getDay() << endl;
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date1.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date1.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date1.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date1.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date1.getLongDate());

    cepDate date2(01, "January", 2002);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date2.getDecimalDate(), 0.00001);     
   
  }
  
  /** date test 2. Tests a simple date */
  void dateTest2 ()
  { 
    cepDate date(2002.9986);
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

    cepDate date1(31, "Dec", 2002);
    double expected = 2002.9986;
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date1.getDecimalDate(), 0.0001);     

    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date1.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date1.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date1.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date1.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date1.getLongDate());

    cepDate date2(31, "December", 2002);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date2.getDecimalDate(), 0.0001);     
    
  }

  /** date test 3. Tests a simple date */  
  void dateTest3 ()
  { 
    cepDate date(1973.4041);
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

    cepDate date1(28, "May", 1973);
    double expected = 1973.4041;
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date1.getDecimalDate(), 0.0001);     

    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date1.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date1.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date1.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date1.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date1.getLongDate());

    cepDate date2(28, "May", 1973);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date2.getDecimalDate(), 0.0001);     
    
  }
  
  /** date test 4. Tests a simple date */  
  void dateTest4 ()
  { 
    cepDate date(1999.4480);
    string year("1999"), 
           month("06"), 
           day("13"), 
           shortDate("13/06/1999"), 
           longDate("13 June 1999");
    
    cout << "long date is " << date.getLongDate() << endl;
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date.getLongDate());

    cepDate date1(13, "Jun", 1999);
    double expected = 1999.4480;
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date1.getDecimalDate(), 0.0001);     

    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date1.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date1.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date1.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date1.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date1.getLongDate());

    cepDate date2(13, "June", 1999);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date2.getDecimalDate(), 0.0001);     
    
  }
  
  /** date leapYear. Tests for a leap year */  
  void leapYear ()
  { 
    cepDate date(2000.1626);
    string year("2000"), 
           month("02"), 
           day("29"), 
           shortDate("29/02/2000"), 
           longDate("29 February 2000");
    
    cout << "day is " << date.getDay();    
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate == date.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate == date.getLongDate());
    
    cepDate date1(29, "Feb", 2000);
    double expected = 2000.1626;
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date1.getDecimalDate(), 0.0001);     

    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date1.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date1.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date1.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short date1", shortDate == date1.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long date1", longDate == date1.getLongDate());

    cepDate date2(29, "February", 2000);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date2.getDecimalDate(), 0.0001); 
    
    cepDate date3(2004.1626);
    string year3("2004"), 
           month3("02"), 
           day3("29"), 
           shortDate3("29/02/2004"), 
           longDate3("29 February 2004");

    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year3 == date3.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month3 == date3.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day3 == date3.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate3 == date3.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate3 == date3.getLongDate());
   
    cepDate date4(29, "February", 2004);
    double expected3 = 2004.1626;
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected3, date4.getDecimalDate(), 0.0001);     

    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year3 == date4.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month3 == date4.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day3 == date4.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short date4", shortDate3 == date4.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long date4", longDate3 == date4.getLongDate());

    cepDate date5(29, "February", 2004);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected3, date5.getDecimalDate(), 0.0001); 
    
    cepDate date6(2004.4057);
    string year5("2004"), 
           month5("05"), 
           day5("28"), 
           shortDate5("28/05/2004"), 
           longDate5("28 May 2004");
    
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year5 == date6.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month5 == date6.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day5 == date6.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate5 == date6.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate5 == date6.getLongDate());
  
    cepDate date7(28, "May", 2004);
    double expected4 = 2004.4057;
 
   
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected4, date7.getDecimalDate(), 0.0001);     

    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year5 == date7.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month5 == date7.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day3 == date5.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short date7", shortDate5 == date7.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long date7", longDate5 == date7.getLongDate());      
        
  }
  
  /** date nonleapYear. Tests to make sure leap years working */  
  void nonLeapYear ()
  { 
    cepDate date(1999.1630);
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
    
    cepDate date1(01, "Mar", 1999);
    double expected = 1999.1630;
   cout << "########decimal date is : " << date1.getDecimalDate() << endl;
    
 
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date1.getDecimalDate(), 0.0001);     
    
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year == date1.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month == date1.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day == date1.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short date1", shortDate == date1.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long date1", longDate == date1.getLongDate());

    cepDate date2(01, "March", 1999);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected, date2.getDecimalDate(), 0.0001); 
    
    cepDate date3(3000.1630);
    string year3("3000"), 
           month3("03"), 
           day3("01"), 
           shortDate3("01/03/3000"), 
           longDate3("01 March 3000");
   
    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year3 == date3.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month3 == date3.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day3 == date3.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short Date", shortDate3 == date3.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long Date", longDate3 == date3.getLongDate());

    cepDate date4(01, "Mar", 3000);
    double expected2 = 3000.1630;
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected2, date4.getDecimalDate(), 0.0001);     

    CPPUNIT_ASSERT_MESSAGE ("wrong Year", year3 == date4.getYear());
    CPPUNIT_ASSERT_MESSAGE ("wrong Month", month3 == date4.getMonth());
    CPPUNIT_ASSERT_MESSAGE ("wrong Day", day3 == date4.getDay());
    CPPUNIT_ASSERT_MESSAGE ("wrong Short date4", shortDate3 == date4.getShortDate());
    CPPUNIT_ASSERT_MESSAGE ("wrong Long date4", longDate3 == date4.getLongDate());

    cepDate date5(01, "March", 3000);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expected2, date5.getDecimalDate(), 0.0001); 
   
  }
  

}; // end Test

 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace
