/* 
 *   Imp for the CEP dataset
 *   Copyright (C) Michael Still                    2002
 *   
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *   
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <stdio.h>

#include "../cepUtility.h"

namespace {
class Test : public CppUnit::TestFixture {

public:
  Test() : CppUnit::TestFixture() {}
  void setUp (){}
  void tearDown (){}

  static CppUnit::Test *suite()
  {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Test" );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testNumeric", &Test::testNumeric ) );

    return suiteOfTests;
  }

protected:

  void testNumeric()
  {
    CPPUNIT_ASSERT_MESSAGE("0 is not numeric!", cepIsNumeric('0'));
    CPPUNIT_ASSERT_MESSAGE("1 is not numeric!", cepIsNumeric('1'));
    CPPUNIT_ASSERT_MESSAGE("2 is not numeric!", cepIsNumeric('2'));
    CPPUNIT_ASSERT_MESSAGE("3 is not numeric!", cepIsNumeric('3'));
    CPPUNIT_ASSERT_MESSAGE("4 is not numeric!", cepIsNumeric('4'));
    CPPUNIT_ASSERT_MESSAGE("5 is not numeric!", cepIsNumeric('5'));
    CPPUNIT_ASSERT_MESSAGE("6 is not numeric!", cepIsNumeric('6'));
    CPPUNIT_ASSERT_MESSAGE("7 is not numeric!", cepIsNumeric('7'));
    CPPUNIT_ASSERT_MESSAGE("8 is not numeric!", cepIsNumeric('8'));
    CPPUNIT_ASSERT_MESSAGE("9 is not numeric!", cepIsNumeric('9'));
    CPPUNIT_ASSERT_MESSAGE(". is not numeric!", cepIsNumeric('.'));
    CPPUNIT_ASSERT_MESSAGE("- is not numeric!", cepIsNumeric('-'));
    CPPUNIT_ASSERT_MESSAGE("A is numeric!", !cepIsNumeric('A'));
    CPPUNIT_ASSERT_MESSAGE("a is numeric!", !cepIsNumeric('a'));
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION( Test );
}
