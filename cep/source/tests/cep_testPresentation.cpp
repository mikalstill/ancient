/* 
 *   ---> This file is generated, don't bother editting it <---
 *
 *   Tests for the CEP data presentation
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
#include <exception>
#include <sys/types.h>
#include <unistd.h>

namespace {
class Test : public CppUnit::TestFixture {

public:
  Test() : CppUnit::TestFixture() {}
  void setUp (){}
  void tearDown (){}

  static CppUnit::Test *suite()
  {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Test" );

  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c", &Test::cpt_mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m", &Test::cpt_mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c", &Test::cpt_mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m", &Test::cpt_mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m));
    return suiteOfTests;
  }

protected:
void cpt_mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c ()
{
  switch(fork())
  {
    case -1:
      CPPUNIT_ASSERT_MESSAGE("fork failed", false);
      break;

    case 0:
      sleep(30);
      system("killall -9 ui");
      break;

    default:
      CPPUNIT_ASSERT_MESSAGE("Test mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c failed", system("../ui -d mb_ALBA_GPS -a yes -e yes -x yes -y yes -z yes -c") != 139);
      break;
  }
}

void cpt_mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m ()
{
  switch(fork())
  {
    case -1:
      CPPUNIT_ASSERT_MESSAGE("fork failed", false);
      break;

    case 0:
      sleep(30);
      system("killall -9 ui");
      break;

    default:
      CPPUNIT_ASSERT_MESSAGE("Test mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m failed", system("../ui -d mb_ALBA_GPS -a yes -e yes -x yes -y yes -z yes -m") != 139);
      break;
  }
}

void cpt_mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c ()
{
  switch(fork())
  {
    case -1:
      CPPUNIT_ASSERT_MESSAGE("fork failed", false);
      break;

    case 0:
      sleep(30);
      system("killall -9 ui");
      break;

    default:
      CPPUNIT_ASSERT_MESSAGE("Test mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c failed", system("../ui -d mb_ALBA_GPS -a yes -e yes -x yes -y yes -z no -c") != 139);
      break;
  }
}

void cpt_mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m ()
{
  switch(fork())
  {
    case -1:
      CPPUNIT_ASSERT_MESSAGE("fork failed", false);
      break;

    case 0:
      sleep(30);
      system("killall -9 ui");
      break;

    default:
      CPPUNIT_ASSERT_MESSAGE("Test mb_ALBA_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m failed", system("../ui -d mb_ALBA_GPS -a yes -e yes -x yes -y yes -z no -m") != 139);
      break;
  }
}


CPPUNIT_TEST_SUITE_REGISTRATION( Test );
};
} // end namespace
