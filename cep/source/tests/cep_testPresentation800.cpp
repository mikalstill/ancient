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

  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_PKEM_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_PKEM_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_PKEM_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_PKEM_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_PKEM_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_PKEM_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_PKEM_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_PKEM_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_PKEM_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_PKEM_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_PKEM_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_PKEM_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_PKEM_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_PKEM_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_PKEM_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PKEM_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_PKEM_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_PLIN_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_PLIN_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_PLIN_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_PLIN_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_PLIN_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_PLIN_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_PLIN_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_PLIN_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_PLIN_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_PLIN_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_PLIN_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_PLIN_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_PLIN_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_PLIN_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_PLIN_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLIN_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_PLIN_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_PLON_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_PLON_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_PLON_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_PLON_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_PLON_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_PLON_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_PLON_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_PLON_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_PLON_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_PLON_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_PLON_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_PLON_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_PLON_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_PLON_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_PLON_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PLON_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_PLON_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_PMAC_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_PMAC_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_PMAC_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_PMAC_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_PMAC_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_PMAC_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_PMAC_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_PMAC_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_PMAC_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_PMAC_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_PMAC_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_PMAC_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_PMAC_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_PMAC_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_PMAC_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PMAC_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_PMAC_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_PTAR_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_PTAR_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_PTAR_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_PTAR_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_PTAR_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_PTAR_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_PTAR_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_PTAR_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_PTAR_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_PTAR_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_PTAR_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_PTAR_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_PTAR_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_PTAR_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_PTAR_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_PTAR_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_PTAR_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_SBAY_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_SBAY_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_SBAY_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_SBAY_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_SBAY_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_SBAY_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_SBAY_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_SBAY_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_SBAY_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_SBAY_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_SBAY_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_SBAY_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_SBAY_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_SBAY_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_SBAY_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_SBAY_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_SBAY_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_STPT_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_STPT_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_STPT_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_STPT_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_STPT_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_STPT_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_STPT_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_STPT_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_STPT_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_STPT_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_STPT_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_STPT_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_STPT_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_STPT_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_STPT_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STPT_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_STPT_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_STR1_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_STR1_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_STR1_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_STR1_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_STR1_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_STR1_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_STR1_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_STR1_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_STR1_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_STR1_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_STR1_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_STR1_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_STR1_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_STR1_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_STR1_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_STR1_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_STR1_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_TEST_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_TEST_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_TEST_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_TEST_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_TEST_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_TEST_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_TEST_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_TEST_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_TEST_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_TEST_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_TEST_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_TEST_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_TEST_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_TEST_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_TEST_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TEST_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_TEST_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_TID1_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_TID1_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_TID1_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_TID1_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_TID1_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_TID1_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_TID1_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_TID1_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_TID1_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_TID1_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_TID1_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_TID1_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_TID1_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_TID1_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_TID1_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID1_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_TID1_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_TID2_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_TID2_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_TID2_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_TID2_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_TID2_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_TID2_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_TID2_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_TID2_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_TID2_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_TID2_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_TID2_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_TID2_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_TID2_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_TID2_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_TID2_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TID2_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_TID2_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_TIDB_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_TIDB_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_TIDB_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_TIDB_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_TIDB_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_TIDB_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_TIDB_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_TIDB_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_TIDB_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_TIDB_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_TIDB_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_TIDB_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_TIDB_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_TIDB_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_TIDB_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIDB_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_TIDB_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIER_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_TIER_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIER_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_TIER_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIER_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_TIER_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIER_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_TIER_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIER_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_TIER_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIER_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_TIER_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_TIER_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_TIER_GPS_e_yes_x_no_y_no_z_yes));
    return suiteOfTests;
  }

protected:
void cpt_mb_PKEM_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_PKEM_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_PKEM_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_PKEM_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_PKEM_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_PKEM_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_PKEM_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_PKEM_GPS -e yes -x yes -y no -z no");}
void cpt_mb_PKEM_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_PKEM_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_PKEM_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_PKEM_GPS -e yes -x no -y yes -z no");}
void cpt_mb_PKEM_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_PKEM_GPS -e yes -x no -y no -z yes");}
void cpt_mb_PKEM_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_PKEM_GPS -e yes -x no -y no -z no");}
void cpt_mb_PKEM_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_PKEM_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_PKEM_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_PKEM_GPS -e no -x yes -y yes -z no");}
void cpt_mb_PKEM_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_PKEM_GPS -e no -x yes -y no -z yes");}
void cpt_mb_PKEM_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_PKEM_GPS -e no -x yes -y no -z no");}
void cpt_mb_PKEM_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_PKEM_GPS -e no -x no -y yes -z yes");}
void cpt_mb_PKEM_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_PKEM_GPS -e no -x no -y yes -z no");}
void cpt_mb_PKEM_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_PKEM_GPS -e no -x no -y no -z yes");}
void cpt_mb_PKEM_GPS_e_no_x_no_y_no_z_no (){runUI("mb_PKEM_GPS -e no -x no -y no -z no");}
void cpt_mb_PLIN_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_PLIN_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_PLIN_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_PLIN_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_PLIN_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_PLIN_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_PLIN_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_PLIN_GPS -e yes -x yes -y no -z no");}
void cpt_mb_PLIN_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_PLIN_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_PLIN_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_PLIN_GPS -e yes -x no -y yes -z no");}
void cpt_mb_PLIN_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_PLIN_GPS -e yes -x no -y no -z yes");}
void cpt_mb_PLIN_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_PLIN_GPS -e yes -x no -y no -z no");}
void cpt_mb_PLIN_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_PLIN_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_PLIN_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_PLIN_GPS -e no -x yes -y yes -z no");}
void cpt_mb_PLIN_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_PLIN_GPS -e no -x yes -y no -z yes");}
void cpt_mb_PLIN_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_PLIN_GPS -e no -x yes -y no -z no");}
void cpt_mb_PLIN_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_PLIN_GPS -e no -x no -y yes -z yes");}
void cpt_mb_PLIN_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_PLIN_GPS -e no -x no -y yes -z no");}
void cpt_mb_PLIN_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_PLIN_GPS -e no -x no -y no -z yes");}
void cpt_mb_PLIN_GPS_e_no_x_no_y_no_z_no (){runUI("mb_PLIN_GPS -e no -x no -y no -z no");}
void cpt_mb_PLON_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_PLON_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_PLON_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_PLON_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_PLON_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_PLON_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_PLON_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_PLON_GPS -e yes -x yes -y no -z no");}
void cpt_mb_PLON_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_PLON_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_PLON_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_PLON_GPS -e yes -x no -y yes -z no");}
void cpt_mb_PLON_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_PLON_GPS -e yes -x no -y no -z yes");}
void cpt_mb_PLON_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_PLON_GPS -e yes -x no -y no -z no");}
void cpt_mb_PLON_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_PLON_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_PLON_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_PLON_GPS -e no -x yes -y yes -z no");}
void cpt_mb_PLON_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_PLON_GPS -e no -x yes -y no -z yes");}
void cpt_mb_PLON_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_PLON_GPS -e no -x yes -y no -z no");}
void cpt_mb_PLON_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_PLON_GPS -e no -x no -y yes -z yes");}
void cpt_mb_PLON_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_PLON_GPS -e no -x no -y yes -z no");}
void cpt_mb_PLON_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_PLON_GPS -e no -x no -y no -z yes");}
void cpt_mb_PLON_GPS_e_no_x_no_y_no_z_no (){runUI("mb_PLON_GPS -e no -x no -y no -z no");}
void cpt_mb_PMAC_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_PMAC_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_PMAC_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_PMAC_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_PMAC_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_PMAC_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_PMAC_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_PMAC_GPS -e yes -x yes -y no -z no");}
void cpt_mb_PMAC_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_PMAC_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_PMAC_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_PMAC_GPS -e yes -x no -y yes -z no");}
void cpt_mb_PMAC_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_PMAC_GPS -e yes -x no -y no -z yes");}
void cpt_mb_PMAC_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_PMAC_GPS -e yes -x no -y no -z no");}
void cpt_mb_PMAC_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_PMAC_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_PMAC_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_PMAC_GPS -e no -x yes -y yes -z no");}
void cpt_mb_PMAC_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_PMAC_GPS -e no -x yes -y no -z yes");}
void cpt_mb_PMAC_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_PMAC_GPS -e no -x yes -y no -z no");}
void cpt_mb_PMAC_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_PMAC_GPS -e no -x no -y yes -z yes");}
void cpt_mb_PMAC_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_PMAC_GPS -e no -x no -y yes -z no");}
void cpt_mb_PMAC_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_PMAC_GPS -e no -x no -y no -z yes");}
void cpt_mb_PMAC_GPS_e_no_x_no_y_no_z_no (){runUI("mb_PMAC_GPS -e no -x no -y no -z no");}
void cpt_mb_PTAR_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_PTAR_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_PTAR_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_PTAR_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_PTAR_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_PTAR_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_PTAR_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_PTAR_GPS -e yes -x yes -y no -z no");}
void cpt_mb_PTAR_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_PTAR_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_PTAR_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_PTAR_GPS -e yes -x no -y yes -z no");}
void cpt_mb_PTAR_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_PTAR_GPS -e yes -x no -y no -z yes");}
void cpt_mb_PTAR_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_PTAR_GPS -e yes -x no -y no -z no");}
void cpt_mb_PTAR_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_PTAR_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_PTAR_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_PTAR_GPS -e no -x yes -y yes -z no");}
void cpt_mb_PTAR_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_PTAR_GPS -e no -x yes -y no -z yes");}
void cpt_mb_PTAR_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_PTAR_GPS -e no -x yes -y no -z no");}
void cpt_mb_PTAR_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_PTAR_GPS -e no -x no -y yes -z yes");}
void cpt_mb_PTAR_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_PTAR_GPS -e no -x no -y yes -z no");}
void cpt_mb_PTAR_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_PTAR_GPS -e no -x no -y no -z yes");}
void cpt_mb_PTAR_GPS_e_no_x_no_y_no_z_no (){runUI("mb_PTAR_GPS -e no -x no -y no -z no");}
void cpt_mb_SBAY_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_SBAY_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_SBAY_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_SBAY_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_SBAY_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_SBAY_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_SBAY_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_SBAY_GPS -e yes -x yes -y no -z no");}
void cpt_mb_SBAY_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_SBAY_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_SBAY_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_SBAY_GPS -e yes -x no -y yes -z no");}
void cpt_mb_SBAY_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_SBAY_GPS -e yes -x no -y no -z yes");}
void cpt_mb_SBAY_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_SBAY_GPS -e yes -x no -y no -z no");}
void cpt_mb_SBAY_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_SBAY_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_SBAY_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_SBAY_GPS -e no -x yes -y yes -z no");}
void cpt_mb_SBAY_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_SBAY_GPS -e no -x yes -y no -z yes");}
void cpt_mb_SBAY_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_SBAY_GPS -e no -x yes -y no -z no");}
void cpt_mb_SBAY_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_SBAY_GPS -e no -x no -y yes -z yes");}
void cpt_mb_SBAY_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_SBAY_GPS -e no -x no -y yes -z no");}
void cpt_mb_SBAY_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_SBAY_GPS -e no -x no -y no -z yes");}
void cpt_mb_SBAY_GPS_e_no_x_no_y_no_z_no (){runUI("mb_SBAY_GPS -e no -x no -y no -z no");}
void cpt_mb_STPT_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_STPT_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_STPT_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_STPT_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_STPT_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_STPT_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_STPT_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_STPT_GPS -e yes -x yes -y no -z no");}
void cpt_mb_STPT_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_STPT_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_STPT_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_STPT_GPS -e yes -x no -y yes -z no");}
void cpt_mb_STPT_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_STPT_GPS -e yes -x no -y no -z yes");}
void cpt_mb_STPT_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_STPT_GPS -e yes -x no -y no -z no");}
void cpt_mb_STPT_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_STPT_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_STPT_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_STPT_GPS -e no -x yes -y yes -z no");}
void cpt_mb_STPT_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_STPT_GPS -e no -x yes -y no -z yes");}
void cpt_mb_STPT_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_STPT_GPS -e no -x yes -y no -z no");}
void cpt_mb_STPT_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_STPT_GPS -e no -x no -y yes -z yes");}
void cpt_mb_STPT_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_STPT_GPS -e no -x no -y yes -z no");}
void cpt_mb_STPT_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_STPT_GPS -e no -x no -y no -z yes");}
void cpt_mb_STPT_GPS_e_no_x_no_y_no_z_no (){runUI("mb_STPT_GPS -e no -x no -y no -z no");}
void cpt_mb_STR1_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_STR1_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_STR1_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_STR1_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_STR1_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_STR1_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_STR1_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_STR1_GPS -e yes -x yes -y no -z no");}
void cpt_mb_STR1_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_STR1_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_STR1_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_STR1_GPS -e yes -x no -y yes -z no");}
void cpt_mb_STR1_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_STR1_GPS -e yes -x no -y no -z yes");}
void cpt_mb_STR1_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_STR1_GPS -e yes -x no -y no -z no");}
void cpt_mb_STR1_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_STR1_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_STR1_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_STR1_GPS -e no -x yes -y yes -z no");}
void cpt_mb_STR1_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_STR1_GPS -e no -x yes -y no -z yes");}
void cpt_mb_STR1_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_STR1_GPS -e no -x yes -y no -z no");}
void cpt_mb_STR1_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_STR1_GPS -e no -x no -y yes -z yes");}
void cpt_mb_STR1_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_STR1_GPS -e no -x no -y yes -z no");}
void cpt_mb_STR1_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_STR1_GPS -e no -x no -y no -z yes");}
void cpt_mb_STR1_GPS_e_no_x_no_y_no_z_no (){runUI("mb_STR1_GPS -e no -x no -y no -z no");}
void cpt_mb_TEST_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_TEST_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_TEST_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_TEST_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_TEST_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_TEST_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_TEST_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_TEST_GPS -e yes -x yes -y no -z no");}
void cpt_mb_TEST_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_TEST_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_TEST_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_TEST_GPS -e yes -x no -y yes -z no");}
void cpt_mb_TEST_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_TEST_GPS -e yes -x no -y no -z yes");}
void cpt_mb_TEST_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_TEST_GPS -e yes -x no -y no -z no");}
void cpt_mb_TEST_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_TEST_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_TEST_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_TEST_GPS -e no -x yes -y yes -z no");}
void cpt_mb_TEST_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_TEST_GPS -e no -x yes -y no -z yes");}
void cpt_mb_TEST_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_TEST_GPS -e no -x yes -y no -z no");}
void cpt_mb_TEST_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_TEST_GPS -e no -x no -y yes -z yes");}
void cpt_mb_TEST_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_TEST_GPS -e no -x no -y yes -z no");}
void cpt_mb_TEST_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_TEST_GPS -e no -x no -y no -z yes");}
void cpt_mb_TEST_GPS_e_no_x_no_y_no_z_no (){runUI("mb_TEST_GPS -e no -x no -y no -z no");}
void cpt_mb_TID1_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_TID1_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_TID1_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_TID1_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_TID1_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_TID1_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_TID1_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_TID1_GPS -e yes -x yes -y no -z no");}
void cpt_mb_TID1_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_TID1_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_TID1_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_TID1_GPS -e yes -x no -y yes -z no");}
void cpt_mb_TID1_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_TID1_GPS -e yes -x no -y no -z yes");}
void cpt_mb_TID1_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_TID1_GPS -e yes -x no -y no -z no");}
void cpt_mb_TID1_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_TID1_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_TID1_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_TID1_GPS -e no -x yes -y yes -z no");}
void cpt_mb_TID1_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_TID1_GPS -e no -x yes -y no -z yes");}
void cpt_mb_TID1_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_TID1_GPS -e no -x yes -y no -z no");}
void cpt_mb_TID1_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_TID1_GPS -e no -x no -y yes -z yes");}
void cpt_mb_TID1_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_TID1_GPS -e no -x no -y yes -z no");}
void cpt_mb_TID1_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_TID1_GPS -e no -x no -y no -z yes");}
void cpt_mb_TID1_GPS_e_no_x_no_y_no_z_no (){runUI("mb_TID1_GPS -e no -x no -y no -z no");}
void cpt_mb_TID2_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_TID2_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_TID2_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_TID2_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_TID2_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_TID2_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_TID2_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_TID2_GPS -e yes -x yes -y no -z no");}
void cpt_mb_TID2_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_TID2_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_TID2_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_TID2_GPS -e yes -x no -y yes -z no");}
void cpt_mb_TID2_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_TID2_GPS -e yes -x no -y no -z yes");}
void cpt_mb_TID2_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_TID2_GPS -e yes -x no -y no -z no");}
void cpt_mb_TID2_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_TID2_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_TID2_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_TID2_GPS -e no -x yes -y yes -z no");}
void cpt_mb_TID2_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_TID2_GPS -e no -x yes -y no -z yes");}
void cpt_mb_TID2_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_TID2_GPS -e no -x yes -y no -z no");}
void cpt_mb_TID2_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_TID2_GPS -e no -x no -y yes -z yes");}
void cpt_mb_TID2_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_TID2_GPS -e no -x no -y yes -z no");}
void cpt_mb_TID2_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_TID2_GPS -e no -x no -y no -z yes");}
void cpt_mb_TID2_GPS_e_no_x_no_y_no_z_no (){runUI("mb_TID2_GPS -e no -x no -y no -z no");}
void cpt_mb_TIDB_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_TIDB_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_TIDB_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_TIDB_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_TIDB_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_TIDB_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_TIDB_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_TIDB_GPS -e yes -x yes -y no -z no");}
void cpt_mb_TIDB_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_TIDB_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_TIDB_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_TIDB_GPS -e yes -x no -y yes -z no");}
void cpt_mb_TIDB_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_TIDB_GPS -e yes -x no -y no -z yes");}
void cpt_mb_TIDB_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_TIDB_GPS -e yes -x no -y no -z no");}
void cpt_mb_TIDB_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_TIDB_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_TIDB_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_TIDB_GPS -e no -x yes -y yes -z no");}
void cpt_mb_TIDB_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_TIDB_GPS -e no -x yes -y no -z yes");}
void cpt_mb_TIDB_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_TIDB_GPS -e no -x yes -y no -z no");}
void cpt_mb_TIDB_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_TIDB_GPS -e no -x no -y yes -z yes");}
void cpt_mb_TIDB_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_TIDB_GPS -e no -x no -y yes -z no");}
void cpt_mb_TIDB_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_TIDB_GPS -e no -x no -y no -z yes");}
void cpt_mb_TIDB_GPS_e_no_x_no_y_no_z_no (){runUI("mb_TIDB_GPS -e no -x no -y no -z no");}
void cpt_mb_TIER_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_TIER_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_TIER_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_TIER_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_TIER_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_TIER_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_TIER_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_TIER_GPS -e yes -x yes -y no -z no");}
void cpt_mb_TIER_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_TIER_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_TIER_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_TIER_GPS -e yes -x no -y yes -z no");}
void cpt_mb_TIER_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_TIER_GPS -e yes -x no -y no -z yes");}

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
