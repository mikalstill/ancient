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

  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c", &Test::cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m", &Test::cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_no_c", &Test::cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_no_m", &Test::cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_yes_c", &Test::cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_yes_m", &Test::cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_no_c", &Test::cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_no_m", &Test::cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_yes_c", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_yes_m", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_no_c", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_no_m", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_yes_c", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_yes_m", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_no_c", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_no_m", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_yes_c", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_yes_m", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_no_c", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_no_m", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_yes_c", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_yes_m", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_no_c", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_no_m", &Test::cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_yes_c", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_yes_m", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_no_c", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_no_m", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_yes_c", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_yes_m", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_no_c", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_no_m", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_yes_c", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_yes_m", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_no_c", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_no_m", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_yes_c", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_yes_m", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_no_c", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_no_m", &Test::cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_yes_c", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_yes_m", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_no_c", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_no_m", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_yes_c", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_yes_m", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_no_c", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_no_m", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_yes_c", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_yes_m", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_no_c", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_no_m", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_no_y_no_z_yes_c", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_no_y_no_z_yes_m", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_no_y_no_z_no_c", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GROO_GPS_a_no_e_no_x_no_y_no_z_no_m", &Test::cpt_mb_GROO_GPS_a_no_e_no_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_no_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_no_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_no_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_no_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_no_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_no_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_no_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_no_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_no_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_no_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_no_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_no_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_no_c", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_no_m", &Test::cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_no_c", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_no_m", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_no_c", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_no_m", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_no_c", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_no_m", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_no_c", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_no_m", &Test::cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_no_c", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_no_m", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_no_c", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_no_m", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_no_c", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_no_m", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_yes_c", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_yes_m", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_no_c", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_no_m", &Test::cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_yes_c", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_yes_m", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_no_c", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_no_m", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_no_c", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_no_m", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_yes_c", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_yes_m", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_no_c", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_no_m", &Test::cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_yes_c", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_yes_m", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_no_c", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_no_m", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_yes_c", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_yes_m", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_no_c", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_no_m", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_yes_c", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_yes_m", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_no_c", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_no_m", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_yes_c", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_yes_m", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_no_c", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_no_m", &Test::cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_yes_c", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_yes_m", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_no_c", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_no_m", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_yes_c", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_yes_m", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_no_c", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_no_m", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_yes_c", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_yes_m", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_no_c", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_no_m", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_yes_c", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_yes_m", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_no_c", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_no_m", &Test::cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_yes_c", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_yes_m", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_no_c", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_no_m", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_yes_c", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_yes_m", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_no_c", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_no_m", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_yes_c", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_yes_m", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_no_c", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_no_m", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_no_y_no_z_yes_c", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_no_y_no_z_yes_m", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_no_y_no_z_no_c", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_no_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HENT_GPS_a_no_e_no_x_no_y_no_z_no_m", &Test::cpt_mb_HENT_GPS_a_no_e_no_x_no_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_yes_c", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_yes_m", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_no_c", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_no_m", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_no_c", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_no_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_no_m", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_no_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_no_y_no_z_yes_c", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_no_z_yes_c));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_no_y_no_z_yes_m", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_no_z_yes_m));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_HIL1_GPS_a_yes_e_yes_x_no_y_no_z_no_c", &Test::cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_no_z_no_c));
    return suiteOfTests;
  }

protected:
void cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c (){runUI("mb_GROO_GPS -a yes -e yes -x no -y yes -z yes -c");}
void cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m (){runUI("mb_GROO_GPS -a yes -e yes -x no -y yes -z yes -m");}
void cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_no_c (){runUI("mb_GROO_GPS -a yes -e yes -x no -y yes -z no -c");}
void cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_yes_z_no_m (){runUI("mb_GROO_GPS -a yes -e yes -x no -y yes -z no -m");}
void cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_yes_c (){runUI("mb_GROO_GPS -a yes -e yes -x no -y no -z yes -c");}
void cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_yes_m (){runUI("mb_GROO_GPS -a yes -e yes -x no -y no -z yes -m");}
void cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_no_c (){runUI("mb_GROO_GPS -a yes -e yes -x no -y no -z no -c");}
void cpt_mb_GROO_GPS_a_yes_e_yes_x_no_y_no_z_no_m (){runUI("mb_GROO_GPS -a yes -e yes -x no -y no -z no -m");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_yes_c (){runUI("mb_GROO_GPS -a yes -e no -x yes -y yes -z yes -c");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_yes_m (){runUI("mb_GROO_GPS -a yes -e no -x yes -y yes -z yes -m");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_no_c (){runUI("mb_GROO_GPS -a yes -e no -x yes -y yes -z no -c");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_yes_z_no_m (){runUI("mb_GROO_GPS -a yes -e no -x yes -y yes -z no -m");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_yes_c (){runUI("mb_GROO_GPS -a yes -e no -x yes -y no -z yes -c");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_yes_m (){runUI("mb_GROO_GPS -a yes -e no -x yes -y no -z yes -m");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_no_c (){runUI("mb_GROO_GPS -a yes -e no -x yes -y no -z no -c");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_yes_y_no_z_no_m (){runUI("mb_GROO_GPS -a yes -e no -x yes -y no -z no -m");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_yes_c (){runUI("mb_GROO_GPS -a yes -e no -x no -y yes -z yes -c");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_yes_m (){runUI("mb_GROO_GPS -a yes -e no -x no -y yes -z yes -m");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_no_c (){runUI("mb_GROO_GPS -a yes -e no -x no -y yes -z no -c");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_yes_z_no_m (){runUI("mb_GROO_GPS -a yes -e no -x no -y yes -z no -m");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_yes_c (){runUI("mb_GROO_GPS -a yes -e no -x no -y no -z yes -c");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_yes_m (){runUI("mb_GROO_GPS -a yes -e no -x no -y no -z yes -m");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_no_c (){runUI("mb_GROO_GPS -a yes -e no -x no -y no -z no -c");}
void cpt_mb_GROO_GPS_a_yes_e_no_x_no_y_no_z_no_m (){runUI("mb_GROO_GPS -a yes -e no -x no -y no -z no -m");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_yes_c (){runUI("mb_GROO_GPS -a no -e yes -x yes -y yes -z yes -c");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_yes_m (){runUI("mb_GROO_GPS -a no -e yes -x yes -y yes -z yes -m");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_no_c (){runUI("mb_GROO_GPS -a no -e yes -x yes -y yes -z no -c");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_yes_z_no_m (){runUI("mb_GROO_GPS -a no -e yes -x yes -y yes -z no -m");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_yes_c (){runUI("mb_GROO_GPS -a no -e yes -x yes -y no -z yes -c");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_yes_m (){runUI("mb_GROO_GPS -a no -e yes -x yes -y no -z yes -m");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_no_c (){runUI("mb_GROO_GPS -a no -e yes -x yes -y no -z no -c");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_yes_y_no_z_no_m (){runUI("mb_GROO_GPS -a no -e yes -x yes -y no -z no -m");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_yes_c (){runUI("mb_GROO_GPS -a no -e yes -x no -y yes -z yes -c");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_yes_m (){runUI("mb_GROO_GPS -a no -e yes -x no -y yes -z yes -m");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_no_c (){runUI("mb_GROO_GPS -a no -e yes -x no -y yes -z no -c");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_yes_z_no_m (){runUI("mb_GROO_GPS -a no -e yes -x no -y yes -z no -m");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_yes_c (){runUI("mb_GROO_GPS -a no -e yes -x no -y no -z yes -c");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_yes_m (){runUI("mb_GROO_GPS -a no -e yes -x no -y no -z yes -m");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_no_c (){runUI("mb_GROO_GPS -a no -e yes -x no -y no -z no -c");}
void cpt_mb_GROO_GPS_a_no_e_yes_x_no_y_no_z_no_m (){runUI("mb_GROO_GPS -a no -e yes -x no -y no -z no -m");}
void cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_yes_c (){runUI("mb_GROO_GPS -a no -e no -x yes -y yes -z yes -c");}
void cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_yes_m (){runUI("mb_GROO_GPS -a no -e no -x yes -y yes -z yes -m");}
void cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_no_c (){runUI("mb_GROO_GPS -a no -e no -x yes -y yes -z no -c");}
void cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_yes_z_no_m (){runUI("mb_GROO_GPS -a no -e no -x yes -y yes -z no -m");}
void cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_yes_c (){runUI("mb_GROO_GPS -a no -e no -x yes -y no -z yes -c");}
void cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_yes_m (){runUI("mb_GROO_GPS -a no -e no -x yes -y no -z yes -m");}
void cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_no_c (){runUI("mb_GROO_GPS -a no -e no -x yes -y no -z no -c");}
void cpt_mb_GROO_GPS_a_no_e_no_x_yes_y_no_z_no_m (){runUI("mb_GROO_GPS -a no -e no -x yes -y no -z no -m");}
void cpt_mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_yes_c (){runUI("mb_GROO_GPS -a no -e no -x no -y yes -z yes -c");}
void cpt_mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_yes_m (){runUI("mb_GROO_GPS -a no -e no -x no -y yes -z yes -m");}
void cpt_mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_no_c (){runUI("mb_GROO_GPS -a no -e no -x no -y yes -z no -c");}
void cpt_mb_GROO_GPS_a_no_e_no_x_no_y_yes_z_no_m (){runUI("mb_GROO_GPS -a no -e no -x no -y yes -z no -m");}
void cpt_mb_GROO_GPS_a_no_e_no_x_no_y_no_z_yes_c (){runUI("mb_GROO_GPS -a no -e no -x no -y no -z yes -c");}
void cpt_mb_GROO_GPS_a_no_e_no_x_no_y_no_z_yes_m (){runUI("mb_GROO_GPS -a no -e no -x no -y no -z yes -m");}
void cpt_mb_GROO_GPS_a_no_e_no_x_no_y_no_z_no_c (){runUI("mb_GROO_GPS -a no -e no -x no -y no -z no -c");}
void cpt_mb_GROO_GPS_a_no_e_no_x_no_y_no_z_no_m (){runUI("mb_GROO_GPS -a no -e no -x no -y no -z no -m");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c (){runUI("mb_GRTG_GPS -a yes -e yes -x yes -y yes -z yes -c");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m (){runUI("mb_GRTG_GPS -a yes -e yes -x yes -y yes -z yes -m");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c (){runUI("mb_GRTG_GPS -a yes -e yes -x yes -y yes -z no -c");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m (){runUI("mb_GRTG_GPS -a yes -e yes -x yes -y yes -z no -m");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_yes_c (){runUI("mb_GRTG_GPS -a yes -e yes -x yes -y no -z yes -c");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_yes_m (){runUI("mb_GRTG_GPS -a yes -e yes -x yes -y no -z yes -m");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_no_c (){runUI("mb_GRTG_GPS -a yes -e yes -x yes -y no -z no -c");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_yes_y_no_z_no_m (){runUI("mb_GRTG_GPS -a yes -e yes -x yes -y no -z no -m");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c (){runUI("mb_GRTG_GPS -a yes -e yes -x no -y yes -z yes -c");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m (){runUI("mb_GRTG_GPS -a yes -e yes -x no -y yes -z yes -m");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_no_c (){runUI("mb_GRTG_GPS -a yes -e yes -x no -y yes -z no -c");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_yes_z_no_m (){runUI("mb_GRTG_GPS -a yes -e yes -x no -y yes -z no -m");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_yes_c (){runUI("mb_GRTG_GPS -a yes -e yes -x no -y no -z yes -c");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_yes_m (){runUI("mb_GRTG_GPS -a yes -e yes -x no -y no -z yes -m");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_no_c (){runUI("mb_GRTG_GPS -a yes -e yes -x no -y no -z no -c");}
void cpt_mb_GRTG_GPS_a_yes_e_yes_x_no_y_no_z_no_m (){runUI("mb_GRTG_GPS -a yes -e yes -x no -y no -z no -m");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_yes_c (){runUI("mb_GRTG_GPS -a yes -e no -x yes -y yes -z yes -c");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_yes_m (){runUI("mb_GRTG_GPS -a yes -e no -x yes -y yes -z yes -m");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_no_c (){runUI("mb_GRTG_GPS -a yes -e no -x yes -y yes -z no -c");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_yes_z_no_m (){runUI("mb_GRTG_GPS -a yes -e no -x yes -y yes -z no -m");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_yes_c (){runUI("mb_GRTG_GPS -a yes -e no -x yes -y no -z yes -c");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_yes_m (){runUI("mb_GRTG_GPS -a yes -e no -x yes -y no -z yes -m");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_no_c (){runUI("mb_GRTG_GPS -a yes -e no -x yes -y no -z no -c");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_yes_y_no_z_no_m (){runUI("mb_GRTG_GPS -a yes -e no -x yes -y no -z no -m");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_yes_c (){runUI("mb_GRTG_GPS -a yes -e no -x no -y yes -z yes -c");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_yes_m (){runUI("mb_GRTG_GPS -a yes -e no -x no -y yes -z yes -m");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_no_c (){runUI("mb_GRTG_GPS -a yes -e no -x no -y yes -z no -c");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_yes_z_no_m (){runUI("mb_GRTG_GPS -a yes -e no -x no -y yes -z no -m");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_yes_c (){runUI("mb_GRTG_GPS -a yes -e no -x no -y no -z yes -c");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_yes_m (){runUI("mb_GRTG_GPS -a yes -e no -x no -y no -z yes -m");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_no_c (){runUI("mb_GRTG_GPS -a yes -e no -x no -y no -z no -c");}
void cpt_mb_GRTG_GPS_a_yes_e_no_x_no_y_no_z_no_m (){runUI("mb_GRTG_GPS -a yes -e no -x no -y no -z no -m");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_yes_c (){runUI("mb_GRTG_GPS -a no -e yes -x yes -y yes -z yes -c");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_yes_m (){runUI("mb_GRTG_GPS -a no -e yes -x yes -y yes -z yes -m");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_no_c (){runUI("mb_GRTG_GPS -a no -e yes -x yes -y yes -z no -c");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_yes_z_no_m (){runUI("mb_GRTG_GPS -a no -e yes -x yes -y yes -z no -m");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_yes_c (){runUI("mb_GRTG_GPS -a no -e yes -x yes -y no -z yes -c");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_yes_m (){runUI("mb_GRTG_GPS -a no -e yes -x yes -y no -z yes -m");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_no_c (){runUI("mb_GRTG_GPS -a no -e yes -x yes -y no -z no -c");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_yes_y_no_z_no_m (){runUI("mb_GRTG_GPS -a no -e yes -x yes -y no -z no -m");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_yes_c (){runUI("mb_GRTG_GPS -a no -e yes -x no -y yes -z yes -c");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_yes_m (){runUI("mb_GRTG_GPS -a no -e yes -x no -y yes -z yes -m");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_no_c (){runUI("mb_GRTG_GPS -a no -e yes -x no -y yes -z no -c");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_yes_z_no_m (){runUI("mb_GRTG_GPS -a no -e yes -x no -y yes -z no -m");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_yes_c (){runUI("mb_GRTG_GPS -a no -e yes -x no -y no -z yes -c");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_yes_m (){runUI("mb_GRTG_GPS -a no -e yes -x no -y no -z yes -m");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_no_c (){runUI("mb_GRTG_GPS -a no -e yes -x no -y no -z no -c");}
void cpt_mb_GRTG_GPS_a_no_e_yes_x_no_y_no_z_no_m (){runUI("mb_GRTG_GPS -a no -e yes -x no -y no -z no -m");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_yes_c (){runUI("mb_GRTG_GPS -a no -e no -x yes -y yes -z yes -c");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_yes_m (){runUI("mb_GRTG_GPS -a no -e no -x yes -y yes -z yes -m");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_no_c (){runUI("mb_GRTG_GPS -a no -e no -x yes -y yes -z no -c");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_yes_z_no_m (){runUI("mb_GRTG_GPS -a no -e no -x yes -y yes -z no -m");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_yes_c (){runUI("mb_GRTG_GPS -a no -e no -x yes -y no -z yes -c");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_yes_m (){runUI("mb_GRTG_GPS -a no -e no -x yes -y no -z yes -m");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_no_c (){runUI("mb_GRTG_GPS -a no -e no -x yes -y no -z no -c");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_yes_y_no_z_no_m (){runUI("mb_GRTG_GPS -a no -e no -x yes -y no -z no -m");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_yes_c (){runUI("mb_GRTG_GPS -a no -e no -x no -y yes -z yes -c");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_yes_m (){runUI("mb_GRTG_GPS -a no -e no -x no -y yes -z yes -m");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_no_c (){runUI("mb_GRTG_GPS -a no -e no -x no -y yes -z no -c");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_yes_z_no_m (){runUI("mb_GRTG_GPS -a no -e no -x no -y yes -z no -m");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_yes_c (){runUI("mb_GRTG_GPS -a no -e no -x no -y no -z yes -c");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_yes_m (){runUI("mb_GRTG_GPS -a no -e no -x no -y no -z yes -m");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_no_c (){runUI("mb_GRTG_GPS -a no -e no -x no -y no -z no -c");}
void cpt_mb_GRTG_GPS_a_no_e_no_x_no_y_no_z_no_m (){runUI("mb_GRTG_GPS -a no -e no -x no -y no -z no -m");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c (){runUI("mb_HENT_GPS -a yes -e yes -x yes -y yes -z yes -c");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m (){runUI("mb_HENT_GPS -a yes -e yes -x yes -y yes -z yes -m");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c (){runUI("mb_HENT_GPS -a yes -e yes -x yes -y yes -z no -c");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m (){runUI("mb_HENT_GPS -a yes -e yes -x yes -y yes -z no -m");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_yes_c (){runUI("mb_HENT_GPS -a yes -e yes -x yes -y no -z yes -c");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_yes_m (){runUI("mb_HENT_GPS -a yes -e yes -x yes -y no -z yes -m");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_no_c (){runUI("mb_HENT_GPS -a yes -e yes -x yes -y no -z no -c");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_yes_y_no_z_no_m (){runUI("mb_HENT_GPS -a yes -e yes -x yes -y no -z no -m");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c (){runUI("mb_HENT_GPS -a yes -e yes -x no -y yes -z yes -c");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m (){runUI("mb_HENT_GPS -a yes -e yes -x no -y yes -z yes -m");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_no_c (){runUI("mb_HENT_GPS -a yes -e yes -x no -y yes -z no -c");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_yes_z_no_m (){runUI("mb_HENT_GPS -a yes -e yes -x no -y yes -z no -m");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_yes_c (){runUI("mb_HENT_GPS -a yes -e yes -x no -y no -z yes -c");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_yes_m (){runUI("mb_HENT_GPS -a yes -e yes -x no -y no -z yes -m");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_no_c (){runUI("mb_HENT_GPS -a yes -e yes -x no -y no -z no -c");}
void cpt_mb_HENT_GPS_a_yes_e_yes_x_no_y_no_z_no_m (){runUI("mb_HENT_GPS -a yes -e yes -x no -y no -z no -m");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_yes_c (){runUI("mb_HENT_GPS -a yes -e no -x yes -y yes -z yes -c");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_yes_m (){runUI("mb_HENT_GPS -a yes -e no -x yes -y yes -z yes -m");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_no_c (){runUI("mb_HENT_GPS -a yes -e no -x yes -y yes -z no -c");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_yes_z_no_m (){runUI("mb_HENT_GPS -a yes -e no -x yes -y yes -z no -m");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_yes_c (){runUI("mb_HENT_GPS -a yes -e no -x yes -y no -z yes -c");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_yes_m (){runUI("mb_HENT_GPS -a yes -e no -x yes -y no -z yes -m");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_no_c (){runUI("mb_HENT_GPS -a yes -e no -x yes -y no -z no -c");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_yes_y_no_z_no_m (){runUI("mb_HENT_GPS -a yes -e no -x yes -y no -z no -m");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_yes_c (){runUI("mb_HENT_GPS -a yes -e no -x no -y yes -z yes -c");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_yes_m (){runUI("mb_HENT_GPS -a yes -e no -x no -y yes -z yes -m");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_no_c (){runUI("mb_HENT_GPS -a yes -e no -x no -y yes -z no -c");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_yes_z_no_m (){runUI("mb_HENT_GPS -a yes -e no -x no -y yes -z no -m");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_yes_c (){runUI("mb_HENT_GPS -a yes -e no -x no -y no -z yes -c");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_yes_m (){runUI("mb_HENT_GPS -a yes -e no -x no -y no -z yes -m");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_no_c (){runUI("mb_HENT_GPS -a yes -e no -x no -y no -z no -c");}
void cpt_mb_HENT_GPS_a_yes_e_no_x_no_y_no_z_no_m (){runUI("mb_HENT_GPS -a yes -e no -x no -y no -z no -m");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_yes_c (){runUI("mb_HENT_GPS -a no -e yes -x yes -y yes -z yes -c");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_yes_m (){runUI("mb_HENT_GPS -a no -e yes -x yes -y yes -z yes -m");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_no_c (){runUI("mb_HENT_GPS -a no -e yes -x yes -y yes -z no -c");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_yes_z_no_m (){runUI("mb_HENT_GPS -a no -e yes -x yes -y yes -z no -m");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_yes_c (){runUI("mb_HENT_GPS -a no -e yes -x yes -y no -z yes -c");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_yes_m (){runUI("mb_HENT_GPS -a no -e yes -x yes -y no -z yes -m");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_no_c (){runUI("mb_HENT_GPS -a no -e yes -x yes -y no -z no -c");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_yes_y_no_z_no_m (){runUI("mb_HENT_GPS -a no -e yes -x yes -y no -z no -m");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_yes_c (){runUI("mb_HENT_GPS -a no -e yes -x no -y yes -z yes -c");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_yes_m (){runUI("mb_HENT_GPS -a no -e yes -x no -y yes -z yes -m");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_no_c (){runUI("mb_HENT_GPS -a no -e yes -x no -y yes -z no -c");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_yes_z_no_m (){runUI("mb_HENT_GPS -a no -e yes -x no -y yes -z no -m");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_yes_c (){runUI("mb_HENT_GPS -a no -e yes -x no -y no -z yes -c");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_yes_m (){runUI("mb_HENT_GPS -a no -e yes -x no -y no -z yes -m");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_no_c (){runUI("mb_HENT_GPS -a no -e yes -x no -y no -z no -c");}
void cpt_mb_HENT_GPS_a_no_e_yes_x_no_y_no_z_no_m (){runUI("mb_HENT_GPS -a no -e yes -x no -y no -z no -m");}
void cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_yes_c (){runUI("mb_HENT_GPS -a no -e no -x yes -y yes -z yes -c");}
void cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_yes_m (){runUI("mb_HENT_GPS -a no -e no -x yes -y yes -z yes -m");}
void cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_no_c (){runUI("mb_HENT_GPS -a no -e no -x yes -y yes -z no -c");}
void cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_yes_z_no_m (){runUI("mb_HENT_GPS -a no -e no -x yes -y yes -z no -m");}
void cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_yes_c (){runUI("mb_HENT_GPS -a no -e no -x yes -y no -z yes -c");}
void cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_yes_m (){runUI("mb_HENT_GPS -a no -e no -x yes -y no -z yes -m");}
void cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_no_c (){runUI("mb_HENT_GPS -a no -e no -x yes -y no -z no -c");}
void cpt_mb_HENT_GPS_a_no_e_no_x_yes_y_no_z_no_m (){runUI("mb_HENT_GPS -a no -e no -x yes -y no -z no -m");}
void cpt_mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_yes_c (){runUI("mb_HENT_GPS -a no -e no -x no -y yes -z yes -c");}
void cpt_mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_yes_m (){runUI("mb_HENT_GPS -a no -e no -x no -y yes -z yes -m");}
void cpt_mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_no_c (){runUI("mb_HENT_GPS -a no -e no -x no -y yes -z no -c");}
void cpt_mb_HENT_GPS_a_no_e_no_x_no_y_yes_z_no_m (){runUI("mb_HENT_GPS -a no -e no -x no -y yes -z no -m");}
void cpt_mb_HENT_GPS_a_no_e_no_x_no_y_no_z_yes_c (){runUI("mb_HENT_GPS -a no -e no -x no -y no -z yes -c");}
void cpt_mb_HENT_GPS_a_no_e_no_x_no_y_no_z_yes_m (){runUI("mb_HENT_GPS -a no -e no -x no -y no -z yes -m");}
void cpt_mb_HENT_GPS_a_no_e_no_x_no_y_no_z_no_c (){runUI("mb_HENT_GPS -a no -e no -x no -y no -z no -c");}
void cpt_mb_HENT_GPS_a_no_e_no_x_no_y_no_z_no_m (){runUI("mb_HENT_GPS -a no -e no -x no -y no -z no -m");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_c (){runUI("mb_HIL1_GPS -a yes -e yes -x yes -y yes -z yes -c");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_yes_m (){runUI("mb_HIL1_GPS -a yes -e yes -x yes -y yes -z yes -m");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_no_c (){runUI("mb_HIL1_GPS -a yes -e yes -x yes -y yes -z no -c");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_yes_z_no_m (){runUI("mb_HIL1_GPS -a yes -e yes -x yes -y yes -z no -m");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_yes_c (){runUI("mb_HIL1_GPS -a yes -e yes -x yes -y no -z yes -c");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_yes_m (){runUI("mb_HIL1_GPS -a yes -e yes -x yes -y no -z yes -m");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_no_c (){runUI("mb_HIL1_GPS -a yes -e yes -x yes -y no -z no -c");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_yes_y_no_z_no_m (){runUI("mb_HIL1_GPS -a yes -e yes -x yes -y no -z no -m");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_yes_c (){runUI("mb_HIL1_GPS -a yes -e yes -x no -y yes -z yes -c");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_yes_m (){runUI("mb_HIL1_GPS -a yes -e yes -x no -y yes -z yes -m");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_no_c (){runUI("mb_HIL1_GPS -a yes -e yes -x no -y yes -z no -c");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_yes_z_no_m (){runUI("mb_HIL1_GPS -a yes -e yes -x no -y yes -z no -m");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_no_z_yes_c (){runUI("mb_HIL1_GPS -a yes -e yes -x no -y no -z yes -c");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_no_z_yes_m (){runUI("mb_HIL1_GPS -a yes -e yes -x no -y no -z yes -m");}
void cpt_mb_HIL1_GPS_a_yes_e_yes_x_no_y_no_z_no_c (){runUI("mb_HIL1_GPS -a yes -e yes -x no -y no -z no -c");}

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
        sleep(2);
        system("killall -9 ui");
        exit(0);
        break;

      default:
        int result = system(string("../ui -d ../../datasets/" + cmdline).c_str());
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
