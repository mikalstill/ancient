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

  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CARN_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_CARN_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CARN_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_CARN_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CARN_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_CARN_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CARN_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_CARN_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CARN_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_CARN_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CARN_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_CARN_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CARN_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_CARN_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CARN_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_CARN_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_CAS1_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_CAS1_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_CAS1_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_CAS1_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_CAS1_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_CAS1_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_CAS1_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_CAS1_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_CAS1_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_CAS1_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_CAS1_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_CAS1_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_CAS1_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_CAS1_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_CAS1_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CAS1_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_CAS1_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_CEDU_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_CEDU_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_CEDU_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_CEDU_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_CEDU_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_CEDU_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_CEDU_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_CEDU_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_CEDU_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_CEDU_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_CEDU_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_CEDU_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_CEDU_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_CEDU_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_CEDU_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CEDU_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_CEDU_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_CENI_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_CENI_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_CENI_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_CENI_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_CENI_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_CENI_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_CENI_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_CENI_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_CENI_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_CENI_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_CENI_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_CENI_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_CENI_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_CENI_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_CENI_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_CENI_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_CENI_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_COCO_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_COCO_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_COCO_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_COCO_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_COCO_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_COCO_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_COCO_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_COCO_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_COCO_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_COCO_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_COCO_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_COCO_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_COCO_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_COCO_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_COCO_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COCO_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_COCO_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_COFF_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_COFF_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_COFF_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_COFF_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_COFF_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_COFF_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_COFF_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_COFF_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_COFF_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_COFF_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_COFF_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_COFF_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_COFF_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_COFF_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_COFF_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COFF_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_COFF_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_COOK_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_COOK_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_COOK_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_COOK_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_COOK_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_COOK_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_COOK_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_COOK_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_COOK_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_COOK_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_COOK_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_COOK_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_COOK_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_COOK_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_COOK_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COOK_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_COOK_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_COVE_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_COVE_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_COVE_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_COVE_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_COVE_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_COVE_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_COVE_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_COVE_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_COVE_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_COVE_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_COVE_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_COVE_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_COVE_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_COVE_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_COVE_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_COVE_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_COVE_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_DARW_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_DARW_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_DARW_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_DARW_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_DARW_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_DARW_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_DARW_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_DARW_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_DARW_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_DARW_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_DARW_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_DARW_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_DARW_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_DARW_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_DARW_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DARW_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_DARW_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_DAV1_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_DAV1_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_DAV1_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_DAV1_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_DAV1_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_DAV1_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_DAV1_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_DAV1_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_DAV1_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_DAV1_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_DAV1_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_DAV1_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_DAV1_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_DAV1_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_DAV1_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DAV1_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_DAV1_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_DST1_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_DST1_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_DST1_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_DST1_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_DST1_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_DST1_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_DST1_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_DST1_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_DST1_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_DST1_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_DST1_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_DST1_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_DST1_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_DST1_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_DST1_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_DST1_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_DST1_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_EUCL_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_EUCL_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_EUCL_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_EUCL_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_EUCL_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_EUCL_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_EUCL_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_EUCL_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_EUCL_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_EUCL_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_EUCL_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_EUCL_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_EUCL_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_EUCL_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_EUCL_GPS_e_no_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_EUCL_GPS_e_no_x_no_y_no_z_no", &Test::cpt_mb_EUCL_GPS_e_no_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_yes_x_yes_y_yes_z_yes", &Test::cpt_mb_FLAG_GPS_e_yes_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_yes_x_yes_y_yes_z_no", &Test::cpt_mb_FLAG_GPS_e_yes_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_yes_x_yes_y_no_z_yes", &Test::cpt_mb_FLAG_GPS_e_yes_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_yes_x_yes_y_no_z_no", &Test::cpt_mb_FLAG_GPS_e_yes_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_yes_x_no_y_yes_z_yes", &Test::cpt_mb_FLAG_GPS_e_yes_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_yes_x_no_y_yes_z_no", &Test::cpt_mb_FLAG_GPS_e_yes_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_yes_x_no_y_no_z_yes", &Test::cpt_mb_FLAG_GPS_e_yes_x_no_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_yes_x_no_y_no_z_no", &Test::cpt_mb_FLAG_GPS_e_yes_x_no_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_no_x_yes_y_yes_z_yes", &Test::cpt_mb_FLAG_GPS_e_no_x_yes_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_no_x_yes_y_yes_z_no", &Test::cpt_mb_FLAG_GPS_e_no_x_yes_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_no_x_yes_y_no_z_yes", &Test::cpt_mb_FLAG_GPS_e_no_x_yes_y_no_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_no_x_yes_y_no_z_no", &Test::cpt_mb_FLAG_GPS_e_no_x_yes_y_no_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_no_x_no_y_yes_z_yes", &Test::cpt_mb_FLAG_GPS_e_no_x_no_y_yes_z_yes));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_no_x_no_y_yes_z_no", &Test::cpt_mb_FLAG_GPS_e_no_x_no_y_yes_z_no));
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("mb_FLAG_GPS_e_no_x_no_y_no_z_yes", &Test::cpt_mb_FLAG_GPS_e_no_x_no_y_no_z_yes));
    return suiteOfTests;
  }

protected:
void cpt_mb_CARN_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_CARN_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_CARN_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_CARN_GPS -e no -x yes -y yes -z no");}
void cpt_mb_CARN_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_CARN_GPS -e no -x yes -y no -z yes");}
void cpt_mb_CARN_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_CARN_GPS -e no -x yes -y no -z no");}
void cpt_mb_CARN_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_CARN_GPS -e no -x no -y yes -z yes");}
void cpt_mb_CARN_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_CARN_GPS -e no -x no -y yes -z no");}
void cpt_mb_CARN_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_CARN_GPS -e no -x no -y no -z yes");}
void cpt_mb_CARN_GPS_e_no_x_no_y_no_z_no (){runUI("mb_CARN_GPS -e no -x no -y no -z no");}
void cpt_mb_CAS1_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_CAS1_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_CAS1_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_CAS1_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_CAS1_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_CAS1_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_CAS1_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_CAS1_GPS -e yes -x yes -y no -z no");}
void cpt_mb_CAS1_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_CAS1_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_CAS1_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_CAS1_GPS -e yes -x no -y yes -z no");}
void cpt_mb_CAS1_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_CAS1_GPS -e yes -x no -y no -z yes");}
void cpt_mb_CAS1_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_CAS1_GPS -e yes -x no -y no -z no");}
void cpt_mb_CAS1_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_CAS1_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_CAS1_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_CAS1_GPS -e no -x yes -y yes -z no");}
void cpt_mb_CAS1_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_CAS1_GPS -e no -x yes -y no -z yes");}
void cpt_mb_CAS1_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_CAS1_GPS -e no -x yes -y no -z no");}
void cpt_mb_CAS1_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_CAS1_GPS -e no -x no -y yes -z yes");}
void cpt_mb_CAS1_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_CAS1_GPS -e no -x no -y yes -z no");}
void cpt_mb_CAS1_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_CAS1_GPS -e no -x no -y no -z yes");}
void cpt_mb_CAS1_GPS_e_no_x_no_y_no_z_no (){runUI("mb_CAS1_GPS -e no -x no -y no -z no");}
void cpt_mb_CEDU_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_CEDU_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_CEDU_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_CEDU_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_CEDU_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_CEDU_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_CEDU_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_CEDU_GPS -e yes -x yes -y no -z no");}
void cpt_mb_CEDU_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_CEDU_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_CEDU_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_CEDU_GPS -e yes -x no -y yes -z no");}
void cpt_mb_CEDU_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_CEDU_GPS -e yes -x no -y no -z yes");}
void cpt_mb_CEDU_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_CEDU_GPS -e yes -x no -y no -z no");}
void cpt_mb_CEDU_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_CEDU_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_CEDU_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_CEDU_GPS -e no -x yes -y yes -z no");}
void cpt_mb_CEDU_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_CEDU_GPS -e no -x yes -y no -z yes");}
void cpt_mb_CEDU_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_CEDU_GPS -e no -x yes -y no -z no");}
void cpt_mb_CEDU_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_CEDU_GPS -e no -x no -y yes -z yes");}
void cpt_mb_CEDU_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_CEDU_GPS -e no -x no -y yes -z no");}
void cpt_mb_CEDU_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_CEDU_GPS -e no -x no -y no -z yes");}
void cpt_mb_CEDU_GPS_e_no_x_no_y_no_z_no (){runUI("mb_CEDU_GPS -e no -x no -y no -z no");}
void cpt_mb_CENI_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_CENI_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_CENI_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_CENI_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_CENI_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_CENI_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_CENI_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_CENI_GPS -e yes -x yes -y no -z no");}
void cpt_mb_CENI_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_CENI_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_CENI_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_CENI_GPS -e yes -x no -y yes -z no");}
void cpt_mb_CENI_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_CENI_GPS -e yes -x no -y no -z yes");}
void cpt_mb_CENI_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_CENI_GPS -e yes -x no -y no -z no");}
void cpt_mb_CENI_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_CENI_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_CENI_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_CENI_GPS -e no -x yes -y yes -z no");}
void cpt_mb_CENI_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_CENI_GPS -e no -x yes -y no -z yes");}
void cpt_mb_CENI_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_CENI_GPS -e no -x yes -y no -z no");}
void cpt_mb_CENI_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_CENI_GPS -e no -x no -y yes -z yes");}
void cpt_mb_CENI_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_CENI_GPS -e no -x no -y yes -z no");}
void cpt_mb_CENI_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_CENI_GPS -e no -x no -y no -z yes");}
void cpt_mb_CENI_GPS_e_no_x_no_y_no_z_no (){runUI("mb_CENI_GPS -e no -x no -y no -z no");}
void cpt_mb_COCO_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_COCO_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_COCO_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_COCO_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_COCO_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_COCO_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_COCO_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_COCO_GPS -e yes -x yes -y no -z no");}
void cpt_mb_COCO_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_COCO_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_COCO_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_COCO_GPS -e yes -x no -y yes -z no");}
void cpt_mb_COCO_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_COCO_GPS -e yes -x no -y no -z yes");}
void cpt_mb_COCO_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_COCO_GPS -e yes -x no -y no -z no");}
void cpt_mb_COCO_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_COCO_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_COCO_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_COCO_GPS -e no -x yes -y yes -z no");}
void cpt_mb_COCO_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_COCO_GPS -e no -x yes -y no -z yes");}
void cpt_mb_COCO_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_COCO_GPS -e no -x yes -y no -z no");}
void cpt_mb_COCO_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_COCO_GPS -e no -x no -y yes -z yes");}
void cpt_mb_COCO_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_COCO_GPS -e no -x no -y yes -z no");}
void cpt_mb_COCO_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_COCO_GPS -e no -x no -y no -z yes");}
void cpt_mb_COCO_GPS_e_no_x_no_y_no_z_no (){runUI("mb_COCO_GPS -e no -x no -y no -z no");}
void cpt_mb_COFF_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_COFF_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_COFF_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_COFF_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_COFF_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_COFF_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_COFF_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_COFF_GPS -e yes -x yes -y no -z no");}
void cpt_mb_COFF_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_COFF_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_COFF_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_COFF_GPS -e yes -x no -y yes -z no");}
void cpt_mb_COFF_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_COFF_GPS -e yes -x no -y no -z yes");}
void cpt_mb_COFF_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_COFF_GPS -e yes -x no -y no -z no");}
void cpt_mb_COFF_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_COFF_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_COFF_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_COFF_GPS -e no -x yes -y yes -z no");}
void cpt_mb_COFF_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_COFF_GPS -e no -x yes -y no -z yes");}
void cpt_mb_COFF_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_COFF_GPS -e no -x yes -y no -z no");}
void cpt_mb_COFF_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_COFF_GPS -e no -x no -y yes -z yes");}
void cpt_mb_COFF_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_COFF_GPS -e no -x no -y yes -z no");}
void cpt_mb_COFF_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_COFF_GPS -e no -x no -y no -z yes");}
void cpt_mb_COFF_GPS_e_no_x_no_y_no_z_no (){runUI("mb_COFF_GPS -e no -x no -y no -z no");}
void cpt_mb_COOK_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_COOK_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_COOK_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_COOK_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_COOK_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_COOK_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_COOK_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_COOK_GPS -e yes -x yes -y no -z no");}
void cpt_mb_COOK_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_COOK_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_COOK_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_COOK_GPS -e yes -x no -y yes -z no");}
void cpt_mb_COOK_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_COOK_GPS -e yes -x no -y no -z yes");}
void cpt_mb_COOK_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_COOK_GPS -e yes -x no -y no -z no");}
void cpt_mb_COOK_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_COOK_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_COOK_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_COOK_GPS -e no -x yes -y yes -z no");}
void cpt_mb_COOK_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_COOK_GPS -e no -x yes -y no -z yes");}
void cpt_mb_COOK_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_COOK_GPS -e no -x yes -y no -z no");}
void cpt_mb_COOK_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_COOK_GPS -e no -x no -y yes -z yes");}
void cpt_mb_COOK_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_COOK_GPS -e no -x no -y yes -z no");}
void cpt_mb_COOK_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_COOK_GPS -e no -x no -y no -z yes");}
void cpt_mb_COOK_GPS_e_no_x_no_y_no_z_no (){runUI("mb_COOK_GPS -e no -x no -y no -z no");}
void cpt_mb_COVE_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_COVE_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_COVE_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_COVE_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_COVE_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_COVE_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_COVE_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_COVE_GPS -e yes -x yes -y no -z no");}
void cpt_mb_COVE_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_COVE_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_COVE_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_COVE_GPS -e yes -x no -y yes -z no");}
void cpt_mb_COVE_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_COVE_GPS -e yes -x no -y no -z yes");}
void cpt_mb_COVE_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_COVE_GPS -e yes -x no -y no -z no");}
void cpt_mb_COVE_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_COVE_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_COVE_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_COVE_GPS -e no -x yes -y yes -z no");}
void cpt_mb_COVE_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_COVE_GPS -e no -x yes -y no -z yes");}
void cpt_mb_COVE_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_COVE_GPS -e no -x yes -y no -z no");}
void cpt_mb_COVE_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_COVE_GPS -e no -x no -y yes -z yes");}
void cpt_mb_COVE_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_COVE_GPS -e no -x no -y yes -z no");}
void cpt_mb_COVE_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_COVE_GPS -e no -x no -y no -z yes");}
void cpt_mb_COVE_GPS_e_no_x_no_y_no_z_no (){runUI("mb_COVE_GPS -e no -x no -y no -z no");}
void cpt_mb_DARW_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_DARW_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_DARW_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_DARW_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_DARW_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_DARW_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_DARW_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_DARW_GPS -e yes -x yes -y no -z no");}
void cpt_mb_DARW_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_DARW_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_DARW_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_DARW_GPS -e yes -x no -y yes -z no");}
void cpt_mb_DARW_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_DARW_GPS -e yes -x no -y no -z yes");}
void cpt_mb_DARW_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_DARW_GPS -e yes -x no -y no -z no");}
void cpt_mb_DARW_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_DARW_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_DARW_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_DARW_GPS -e no -x yes -y yes -z no");}
void cpt_mb_DARW_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_DARW_GPS -e no -x yes -y no -z yes");}
void cpt_mb_DARW_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_DARW_GPS -e no -x yes -y no -z no");}
void cpt_mb_DARW_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_DARW_GPS -e no -x no -y yes -z yes");}
void cpt_mb_DARW_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_DARW_GPS -e no -x no -y yes -z no");}
void cpt_mb_DARW_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_DARW_GPS -e no -x no -y no -z yes");}
void cpt_mb_DARW_GPS_e_no_x_no_y_no_z_no (){runUI("mb_DARW_GPS -e no -x no -y no -z no");}
void cpt_mb_DAV1_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_DAV1_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_DAV1_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_DAV1_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_DAV1_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_DAV1_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_DAV1_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_DAV1_GPS -e yes -x yes -y no -z no");}
void cpt_mb_DAV1_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_DAV1_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_DAV1_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_DAV1_GPS -e yes -x no -y yes -z no");}
void cpt_mb_DAV1_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_DAV1_GPS -e yes -x no -y no -z yes");}
void cpt_mb_DAV1_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_DAV1_GPS -e yes -x no -y no -z no");}
void cpt_mb_DAV1_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_DAV1_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_DAV1_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_DAV1_GPS -e no -x yes -y yes -z no");}
void cpt_mb_DAV1_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_DAV1_GPS -e no -x yes -y no -z yes");}
void cpt_mb_DAV1_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_DAV1_GPS -e no -x yes -y no -z no");}
void cpt_mb_DAV1_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_DAV1_GPS -e no -x no -y yes -z yes");}
void cpt_mb_DAV1_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_DAV1_GPS -e no -x no -y yes -z no");}
void cpt_mb_DAV1_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_DAV1_GPS -e no -x no -y no -z yes");}
void cpt_mb_DAV1_GPS_e_no_x_no_y_no_z_no (){runUI("mb_DAV1_GPS -e no -x no -y no -z no");}
void cpt_mb_DST1_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_DST1_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_DST1_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_DST1_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_DST1_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_DST1_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_DST1_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_DST1_GPS -e yes -x yes -y no -z no");}
void cpt_mb_DST1_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_DST1_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_DST1_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_DST1_GPS -e yes -x no -y yes -z no");}
void cpt_mb_DST1_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_DST1_GPS -e yes -x no -y no -z yes");}
void cpt_mb_DST1_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_DST1_GPS -e yes -x no -y no -z no");}
void cpt_mb_DST1_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_DST1_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_DST1_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_DST1_GPS -e no -x yes -y yes -z no");}
void cpt_mb_DST1_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_DST1_GPS -e no -x yes -y no -z yes");}
void cpt_mb_DST1_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_DST1_GPS -e no -x yes -y no -z no");}
void cpt_mb_DST1_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_DST1_GPS -e no -x no -y yes -z yes");}
void cpt_mb_DST1_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_DST1_GPS -e no -x no -y yes -z no");}
void cpt_mb_DST1_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_DST1_GPS -e no -x no -y no -z yes");}
void cpt_mb_DST1_GPS_e_no_x_no_y_no_z_no (){runUI("mb_DST1_GPS -e no -x no -y no -z no");}
void cpt_mb_EUCL_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_EUCL_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_EUCL_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_EUCL_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_EUCL_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_EUCL_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_EUCL_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_EUCL_GPS -e yes -x yes -y no -z no");}
void cpt_mb_EUCL_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_EUCL_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_EUCL_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_EUCL_GPS -e yes -x no -y yes -z no");}
void cpt_mb_EUCL_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_EUCL_GPS -e yes -x no -y no -z yes");}
void cpt_mb_EUCL_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_EUCL_GPS -e yes -x no -y no -z no");}
void cpt_mb_EUCL_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_EUCL_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_EUCL_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_EUCL_GPS -e no -x yes -y yes -z no");}
void cpt_mb_EUCL_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_EUCL_GPS -e no -x yes -y no -z yes");}
void cpt_mb_EUCL_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_EUCL_GPS -e no -x yes -y no -z no");}
void cpt_mb_EUCL_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_EUCL_GPS -e no -x no -y yes -z yes");}
void cpt_mb_EUCL_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_EUCL_GPS -e no -x no -y yes -z no");}
void cpt_mb_EUCL_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_EUCL_GPS -e no -x no -y no -z yes");}
void cpt_mb_EUCL_GPS_e_no_x_no_y_no_z_no (){runUI("mb_EUCL_GPS -e no -x no -y no -z no");}
void cpt_mb_FLAG_GPS_e_yes_x_yes_y_yes_z_yes (){runUI("mb_FLAG_GPS -e yes -x yes -y yes -z yes");}
void cpt_mb_FLAG_GPS_e_yes_x_yes_y_yes_z_no (){runUI("mb_FLAG_GPS -e yes -x yes -y yes -z no");}
void cpt_mb_FLAG_GPS_e_yes_x_yes_y_no_z_yes (){runUI("mb_FLAG_GPS -e yes -x yes -y no -z yes");}
void cpt_mb_FLAG_GPS_e_yes_x_yes_y_no_z_no (){runUI("mb_FLAG_GPS -e yes -x yes -y no -z no");}
void cpt_mb_FLAG_GPS_e_yes_x_no_y_yes_z_yes (){runUI("mb_FLAG_GPS -e yes -x no -y yes -z yes");}
void cpt_mb_FLAG_GPS_e_yes_x_no_y_yes_z_no (){runUI("mb_FLAG_GPS -e yes -x no -y yes -z no");}
void cpt_mb_FLAG_GPS_e_yes_x_no_y_no_z_yes (){runUI("mb_FLAG_GPS -e yes -x no -y no -z yes");}
void cpt_mb_FLAG_GPS_e_yes_x_no_y_no_z_no (){runUI("mb_FLAG_GPS -e yes -x no -y no -z no");}
void cpt_mb_FLAG_GPS_e_no_x_yes_y_yes_z_yes (){runUI("mb_FLAG_GPS -e no -x yes -y yes -z yes");}
void cpt_mb_FLAG_GPS_e_no_x_yes_y_yes_z_no (){runUI("mb_FLAG_GPS -e no -x yes -y yes -z no");}
void cpt_mb_FLAG_GPS_e_no_x_yes_y_no_z_yes (){runUI("mb_FLAG_GPS -e no -x yes -y no -z yes");}
void cpt_mb_FLAG_GPS_e_no_x_yes_y_no_z_no (){runUI("mb_FLAG_GPS -e no -x yes -y no -z no");}
void cpt_mb_FLAG_GPS_e_no_x_no_y_yes_z_yes (){runUI("mb_FLAG_GPS -e no -x no -y yes -z yes");}
void cpt_mb_FLAG_GPS_e_no_x_no_y_yes_z_no (){runUI("mb_FLAG_GPS -e no -x no -y yes -z no");}
void cpt_mb_FLAG_GPS_e_no_x_no_y_no_z_yes (){runUI("mb_FLAG_GPS -e no -x no -y no -z yes");}

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
