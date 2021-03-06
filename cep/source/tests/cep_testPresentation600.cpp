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
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

namespace {
class Test : public CppUnit::TestFixture {

public:
  Test() : CppUnit::TestFixture() {}
  void setUp (){}
  void tearDown (){}

  static CppUnit::Test *suite()
  {
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Test" );

  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_KEMB_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_KEMB_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_KEMB_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_KEMB_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_KEMB_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_KEMB_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_KEMB_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_KEMB_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_KEMB_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_KEMB_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_KEMB_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_KEMB_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_KEMB_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_KEMB_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_KEMB_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_KEMB_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_LORN_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_LORN_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_LORN_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_LORN_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_LORN_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_LORN_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_LORN_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_LORN_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_LORN_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_LORN_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_LORN_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_LORN_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_LORN_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_LORN_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_LORN_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_LORN_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_LORN_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_MAC1_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_MAC1_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_MAC1_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_MAC1_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_MAC1_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_MAC1_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_MAC1_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_MAC1_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_MAC1_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_MAC1_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_MAC1_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_MAC1_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_MAC1_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_MAC1_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_MAC1_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAC1_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_MAC1_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_MACK_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_MACK_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_MACK_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_MACK_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_MACK_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_MACK_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_MACK_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_MACK_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_MACK_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_MACK_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_MACK_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_MACK_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_MACK_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_MACK_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_MACK_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MACK_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_MACK_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_MAW1_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_MAW1_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_MAW1_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_MAW1_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_MAW1_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_MAW1_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_MAW1_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_MAW1_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_MAW1_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_MAW1_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_MAW1_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_MAW1_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_MAW1_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_MAW1_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_MAW1_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MAW1_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_MAW1_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_MLY0_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_MLY0_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_MLY0_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_MLY0_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_MLY0_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_MLY0_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_MLY0_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_MLY0_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_MLY0_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_MLY0_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_MLY0_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_MLY0_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_MLY0_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_MLY0_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_MLY0_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY0_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_MLY0_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_MLY1_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_MLY1_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_MLY1_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_MLY1_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_MLY1_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_MLY1_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_MLY1_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_MLY1_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_MLY1_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_MLY1_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_MLY1_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_MLY1_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_MLY1_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_MLY1_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_MLY1_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MLY1_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_MLY1_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_MULA_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_MULA_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_MULA_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_MULA_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_MULA_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_MULA_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_MULA_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_MULA_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_MULA_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_MULA_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_MULA_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_MULA_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_MULA_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_MULA_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_MULA_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MULA_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_MULA_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_MURP_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_MURP_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_MURP_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_MURP_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_MURP_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_MURP_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_MURP_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_MURP_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_MURP_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_MURP_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_MURP_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_MURP_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_MURP_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_MURP_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_MURP_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_MURP_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_MURP_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_PERT_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_PERT_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_PERT_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_PERT_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_PERT_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_PERT_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_PERT_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_PERT_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_PERT_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_PERT_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_PERT_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_PERT_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_PERT_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_PERT_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_PERT_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PERT_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_PERT_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_PFAI_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_PFAI_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_PFAI_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_PFAI_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_PFAI_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_PFAI_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_PFAI_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_PFAI_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_PFAI_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_PFAI_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_PFAI_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_PFAI_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_PFAI_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_PFAI_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_PFAI_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PFAI_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_PFAI_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_PHED_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_PHED_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_PHED_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_PHED_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_PHED_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_PHED_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_PHED_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_PHED_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_PHED_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_PHED_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_PHED_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_PHED_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_PHED_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_PHED_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_PHED_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PHED_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_PHED_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_PILL_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_PILL_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_PILL_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_PILL_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_PILL_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_PILL_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_PILL_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_PILL_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_PILL_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_PILL_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_PILL_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_PILL_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_PILL_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_PILL_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PILL_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_PILL_GPS_e_no_x_no_y_no_z_yes));
    return suiteOfTests;
  }

protected:
void cpt_mb_KEMB_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_KEMB_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_KEMB_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_KEMB_GPS -e no -x yes -y yes -z no");}
void cpt_mb_KEMB_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_KEMB_GPS -e no -x yes -y no -z yes");}
void cpt_mb_KEMB_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_KEMB_GPS -e no -x yes -y no -z no");}
void cpt_mb_KEMB_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_KEMB_GPS -e no -x no -y yes -z yes");}
void cpt_mb_KEMB_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_KEMB_GPS -e no -x no -y yes -z no");}
void cpt_mb_KEMB_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_KEMB_GPS -e no -x no -y no -z yes");}
void cpt_mb_KEMB_GPS_e_no_x_no_y_no_z_no (){runUI("mb_KEMB_GPS -e no -x no -y no -z no");}
void cpt_mb_LORN_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_LORN_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_LORN_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_LORN_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_LORN_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_LORN_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_LORN_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_LORN_GPS -e yes -x yes -y no -z no");}
void cpt_mb_LORN_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_LORN_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_LORN_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_LORN_GPS -e yes -x no -y yes -z no");}
void cpt_mb_LORN_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_LORN_GPS -e yes -x no -y no -z yes");}
void cpt_mb_LORN_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_LORN_GPS -e yes -x no -y no -z no");}
void cpt_mb_LORN_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_LORN_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_LORN_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_LORN_GPS -e no -x yes -y yes -z no");}
void cpt_mb_LORN_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_LORN_GPS -e no -x yes -y no -z yes");}
void cpt_mb_LORN_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_LORN_GPS -e no -x yes -y no -z no");}
void cpt_mb_LORN_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_LORN_GPS -e no -x no -y yes -z yes");}
void cpt_mb_LORN_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_LORN_GPS -e no -x no -y yes -z no");}
void cpt_mb_LORN_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_LORN_GPS -e no -x no -y no -z yes");}
void cpt_mb_LORN_GPS_e_no_x_no_y_no_z_no (){runUI("mb_LORN_GPS -e no -x no -y no -z no");}
void cpt_mb_MAC1_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_MAC1_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_MAC1_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_MAC1_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_MAC1_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_MAC1_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_MAC1_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_MAC1_GPS -e yes -x yes -y no -z no");}
void cpt_mb_MAC1_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_MAC1_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_MAC1_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_MAC1_GPS -e yes -x no -y yes -z no");}
void cpt_mb_MAC1_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_MAC1_GPS -e yes -x no -y no -z yes");}
void cpt_mb_MAC1_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_MAC1_GPS -e yes -x no -y no -z no");}
void cpt_mb_MAC1_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_MAC1_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_MAC1_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_MAC1_GPS -e no -x yes -y yes -z no");}
void cpt_mb_MAC1_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_MAC1_GPS -e no -x yes -y no -z yes");}
void cpt_mb_MAC1_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_MAC1_GPS -e no -x yes -y no -z no");}
void cpt_mb_MAC1_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_MAC1_GPS -e no -x no -y yes -z yes");}
void cpt_mb_MAC1_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_MAC1_GPS -e no -x no -y yes -z no");}
void cpt_mb_MAC1_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_MAC1_GPS -e no -x no -y no -z yes");}
void cpt_mb_MAC1_GPS_e_no_x_no_y_no_z_no (){runUI("mb_MAC1_GPS -e no -x no -y no -z no");}
void cpt_mb_MACK_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_MACK_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_MACK_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_MACK_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_MACK_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_MACK_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_MACK_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_MACK_GPS -e yes -x yes -y no -z no");}
void cpt_mb_MACK_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_MACK_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_MACK_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_MACK_GPS -e yes -x no -y yes -z no");}
void cpt_mb_MACK_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_MACK_GPS -e yes -x no -y no -z yes");}
void cpt_mb_MACK_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_MACK_GPS -e yes -x no -y no -z no");}
void cpt_mb_MACK_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_MACK_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_MACK_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_MACK_GPS -e no -x yes -y yes -z no");}
void cpt_mb_MACK_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_MACK_GPS -e no -x yes -y no -z yes");}
void cpt_mb_MACK_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_MACK_GPS -e no -x yes -y no -z no");}
void cpt_mb_MACK_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_MACK_GPS -e no -x no -y yes -z yes");}
void cpt_mb_MACK_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_MACK_GPS -e no -x no -y yes -z no");}
void cpt_mb_MACK_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_MACK_GPS -e no -x no -y no -z yes");}
void cpt_mb_MACK_GPS_e_no_x_no_y_no_z_no (){runUI("mb_MACK_GPS -e no -x no -y no -z no");}
void cpt_mb_MAW1_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_MAW1_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_MAW1_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_MAW1_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_MAW1_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_MAW1_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_MAW1_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_MAW1_GPS -e yes -x yes -y no -z no");}
void cpt_mb_MAW1_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_MAW1_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_MAW1_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_MAW1_GPS -e yes -x no -y yes -z no");}
void cpt_mb_MAW1_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_MAW1_GPS -e yes -x no -y no -z yes");}
void cpt_mb_MAW1_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_MAW1_GPS -e yes -x no -y no -z no");}
void cpt_mb_MAW1_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_MAW1_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_MAW1_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_MAW1_GPS -e no -x yes -y yes -z no");}
void cpt_mb_MAW1_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_MAW1_GPS -e no -x yes -y no -z yes");}
void cpt_mb_MAW1_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_MAW1_GPS -e no -x yes -y no -z no");}
void cpt_mb_MAW1_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_MAW1_GPS -e no -x no -y yes -z yes");}
void cpt_mb_MAW1_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_MAW1_GPS -e no -x no -y yes -z no");}
void cpt_mb_MAW1_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_MAW1_GPS -e no -x no -y no -z yes");}
void cpt_mb_MAW1_GPS_e_no_x_no_y_no_z_no (){runUI("mb_MAW1_GPS -e no -x no -y no -z no");}
void cpt_mb_MLY0_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_MLY0_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_MLY0_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_MLY0_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_MLY0_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_MLY0_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_MLY0_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_MLY0_GPS -e yes -x yes -y no -z no");}
void cpt_mb_MLY0_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_MLY0_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_MLY0_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_MLY0_GPS -e yes -x no -y yes -z no");}
void cpt_mb_MLY0_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_MLY0_GPS -e yes -x no -y no -z yes");}
void cpt_mb_MLY0_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_MLY0_GPS -e yes -x no -y no -z no");}
void cpt_mb_MLY0_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_MLY0_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_MLY0_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_MLY0_GPS -e no -x yes -y yes -z no");}
void cpt_mb_MLY0_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_MLY0_GPS -e no -x yes -y no -z yes");}
void cpt_mb_MLY0_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_MLY0_GPS -e no -x yes -y no -z no");}
void cpt_mb_MLY0_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_MLY0_GPS -e no -x no -y yes -z yes");}
void cpt_mb_MLY0_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_MLY0_GPS -e no -x no -y yes -z no");}
void cpt_mb_MLY0_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_MLY0_GPS -e no -x no -y no -z yes");}
void cpt_mb_MLY0_GPS_e_no_x_no_y_no_z_no (){runUI("mb_MLY0_GPS -e no -x no -y no -z no");}
void cpt_mb_MLY1_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_MLY1_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_MLY1_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_MLY1_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_MLY1_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_MLY1_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_MLY1_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_MLY1_GPS -e yes -x yes -y no -z no");}
void cpt_mb_MLY1_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_MLY1_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_MLY1_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_MLY1_GPS -e yes -x no -y yes -z no");}
void cpt_mb_MLY1_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_MLY1_GPS -e yes -x no -y no -z yes");}
void cpt_mb_MLY1_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_MLY1_GPS -e yes -x no -y no -z no");}
void cpt_mb_MLY1_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_MLY1_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_MLY1_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_MLY1_GPS -e no -x yes -y yes -z no");}
void cpt_mb_MLY1_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_MLY1_GPS -e no -x yes -y no -z yes");}
void cpt_mb_MLY1_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_MLY1_GPS -e no -x yes -y no -z no");}
void cpt_mb_MLY1_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_MLY1_GPS -e no -x no -y yes -z yes");}
void cpt_mb_MLY1_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_MLY1_GPS -e no -x no -y yes -z no");}
void cpt_mb_MLY1_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_MLY1_GPS -e no -x no -y no -z yes");}
void cpt_mb_MLY1_GPS_e_no_x_no_y_no_z_no (){runUI("mb_MLY1_GPS -e no -x no -y no -z no");}
void cpt_mb_MULA_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_MULA_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_MULA_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_MULA_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_MULA_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_MULA_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_MULA_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_MULA_GPS -e yes -x yes -y no -z no");}
void cpt_mb_MULA_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_MULA_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_MULA_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_MULA_GPS -e yes -x no -y yes -z no");}
void cpt_mb_MULA_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_MULA_GPS -e yes -x no -y no -z yes");}
void cpt_mb_MULA_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_MULA_GPS -e yes -x no -y no -z no");}
void cpt_mb_MULA_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_MULA_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_MULA_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_MULA_GPS -e no -x yes -y yes -z no");}
void cpt_mb_MULA_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_MULA_GPS -e no -x yes -y no -z yes");}
void cpt_mb_MULA_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_MULA_GPS -e no -x yes -y no -z no");}
void cpt_mb_MULA_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_MULA_GPS -e no -x no -y yes -z yes");}
void cpt_mb_MULA_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_MULA_GPS -e no -x no -y yes -z no");}
void cpt_mb_MULA_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_MULA_GPS -e no -x no -y no -z yes");}
void cpt_mb_MULA_GPS_e_no_x_no_y_no_z_no (){runUI("mb_MULA_GPS -e no -x no -y no -z no");}
void cpt_mb_MURP_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_MURP_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_MURP_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_MURP_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_MURP_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_MURP_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_MURP_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_MURP_GPS -e yes -x yes -y no -z no");}
void cpt_mb_MURP_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_MURP_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_MURP_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_MURP_GPS -e yes -x no -y yes -z no");}
void cpt_mb_MURP_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_MURP_GPS -e yes -x no -y no -z yes");}
void cpt_mb_MURP_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_MURP_GPS -e yes -x no -y no -z no");}
void cpt_mb_MURP_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_MURP_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_MURP_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_MURP_GPS -e no -x yes -y yes -z no");}
void cpt_mb_MURP_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_MURP_GPS -e no -x yes -y no -z yes");}
void cpt_mb_MURP_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_MURP_GPS -e no -x yes -y no -z no");}
void cpt_mb_MURP_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_MURP_GPS -e no -x no -y yes -z yes");}
void cpt_mb_MURP_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_MURP_GPS -e no -x no -y yes -z no");}
void cpt_mb_MURP_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_MURP_GPS -e no -x no -y no -z yes");}
void cpt_mb_MURP_GPS_e_no_x_no_y_no_z_no (){runUI("mb_MURP_GPS -e no -x no -y no -z no");}
void cpt_mb_PERT_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_PERT_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_PERT_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_PERT_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_PERT_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_PERT_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_PERT_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_PERT_GPS -e yes -x yes -y no -z no");}
void cpt_mb_PERT_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_PERT_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_PERT_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_PERT_GPS -e yes -x no -y yes -z no");}
void cpt_mb_PERT_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_PERT_GPS -e yes -x no -y no -z yes");}
void cpt_mb_PERT_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_PERT_GPS -e yes -x no -y no -z no");}
void cpt_mb_PERT_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_PERT_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_PERT_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_PERT_GPS -e no -x yes -y yes -z no");}
void cpt_mb_PERT_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_PERT_GPS -e no -x yes -y no -z yes");}
void cpt_mb_PERT_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_PERT_GPS -e no -x yes -y no -z no");}
void cpt_mb_PERT_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_PERT_GPS -e no -x no -y yes -z yes");}
void cpt_mb_PERT_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_PERT_GPS -e no -x no -y yes -z no");}
void cpt_mb_PERT_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_PERT_GPS -e no -x no -y no -z yes");}
void cpt_mb_PERT_GPS_e_no_x_no_y_no_z_no (){runUI("mb_PERT_GPS -e no -x no -y no -z no");}
void cpt_mb_PFAI_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_PFAI_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_PFAI_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_PFAI_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_PFAI_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_PFAI_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_PFAI_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_PFAI_GPS -e yes -x yes -y no -z no");}
void cpt_mb_PFAI_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_PFAI_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_PFAI_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_PFAI_GPS -e yes -x no -y yes -z no");}
void cpt_mb_PFAI_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_PFAI_GPS -e yes -x no -y no -z yes");}
void cpt_mb_PFAI_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_PFAI_GPS -e yes -x no -y no -z no");}
void cpt_mb_PFAI_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_PFAI_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_PFAI_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_PFAI_GPS -e no -x yes -y yes -z no");}
void cpt_mb_PFAI_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_PFAI_GPS -e no -x yes -y no -z yes");}
void cpt_mb_PFAI_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_PFAI_GPS -e no -x yes -y no -z no");}
void cpt_mb_PFAI_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_PFAI_GPS -e no -x no -y yes -z yes");}
void cpt_mb_PFAI_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_PFAI_GPS -e no -x no -y yes -z no");}
void cpt_mb_PFAI_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_PFAI_GPS -e no -x no -y no -z yes");}
void cpt_mb_PFAI_GPS_e_no_x_no_y_no_z_no (){runUI("mb_PFAI_GPS -e no -x no -y no -z no");}
void cpt_mb_PHED_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_PHED_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_PHED_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_PHED_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_PHED_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_PHED_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_PHED_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_PHED_GPS -e yes -x yes -y no -z no");}
void cpt_mb_PHED_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_PHED_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_PHED_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_PHED_GPS -e yes -x no -y yes -z no");}
void cpt_mb_PHED_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_PHED_GPS -e yes -x no -y no -z yes");}
void cpt_mb_PHED_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_PHED_GPS -e yes -x no -y no -z no");}
void cpt_mb_PHED_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_PHED_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_PHED_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_PHED_GPS -e no -x yes -y yes -z no");}
void cpt_mb_PHED_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_PHED_GPS -e no -x yes -y no -z yes");}
void cpt_mb_PHED_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_PHED_GPS -e no -x yes -y no -z no");}
void cpt_mb_PHED_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_PHED_GPS -e no -x no -y yes -z yes");}
void cpt_mb_PHED_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_PHED_GPS -e no -x no -y yes -z no");}
void cpt_mb_PHED_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_PHED_GPS -e no -x no -y no -z yes");}
void cpt_mb_PHED_GPS_e_no_x_no_y_no_z_no (){runUI("mb_PHED_GPS -e no -x no -y no -z no");}
void cpt_mb_PILL_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_PILL_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_PILL_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_PILL_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_PILL_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_PILL_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_PILL_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_PILL_GPS -e yes -x yes -y no -z no");}
void cpt_mb_PILL_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_PILL_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_PILL_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_PILL_GPS -e yes -x no -y yes -z no");}
void cpt_mb_PILL_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_PILL_GPS -e yes -x no -y no -z yes");}
void cpt_mb_PILL_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_PILL_GPS -e yes -x no -y no -z no");}
void cpt_mb_PILL_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_PILL_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_PILL_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_PILL_GPS -e no -x yes -y yes -z no");}
void cpt_mb_PILL_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_PILL_GPS -e no -x yes -y no -z yes");}
void cpt_mb_PILL_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_PILL_GPS -e no -x yes -y no -z no");}
void cpt_mb_PILL_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_PILL_GPS -e no -x no -y yes -z yes");}
void cpt_mb_PILL_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_PILL_GPS -e no -x no -y yes -z no");}
void cpt_mb_PILL_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_PILL_GPS -e no -x no -y no -z yes");}

  // The testing function
  void runUI(string cmdline){
    int childPid, numRetries = 0;

retry:
    switch(childPid = fork())
    {
      case -1:
        if(numRetries < 5){
	  fprintf(stderr, "*");
	  fflush(stderr);
	  numRetries++;
	  sleep(60);
	  goto retry;
          }
        else
	  CPPUNIT_ASSERT_MESSAGE("Fork attempt repeatedly failed", false);
        break;

      case 0:
        sleep(5);
        system("killall -9 gdms");
        exit(0);
        break;

      default:
        int result = system(string("../gdms -d ../../datasets/" + cmdline).c_str());
        kill(childPid, 9);
	waitpid(childPid, NULL, 0);
        //printf("Result is %d signalled %s (%d)\n", WEXITSTATUS(result),
        //       WIFSIGNALED(result) ? "true" : "false",
        //       WTERMSIG(result));
        // WCOREDUMP would have been good, but glibc doesn't implement it
        CPPUNIT_ASSERT_MESSAGE(string("Test " + cmdline + " failed").c_str(), 
                               WTERMSIG(result) != SIGSEGV);
        break;
    }
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace
