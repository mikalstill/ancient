/***************************************************************************
 * File: alltest_template.cpp
 *
 * This template provides a machanism for running your tests using the auto
 * registration feature provided in CppUnit. this will runn all tests that
 * have been subscribed. If you wish to modularise your tests then ther are
 * other less eloquent methods. This one should be fine for our purposes.
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

 /**
 * A Sample test case to demonstrate CppUnit framework
 * This is lifted from the CppUnit cookbook document.
 *
 * @author  <your name here>
 * @version $Revision: 1.1 $ $Date: 2002-08-04 04:05:26 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
 */

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

 /**
  * This function when called will executed all tests that have been subscribed
  * to the registry
  */
int main ()
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  bool success = runner.run( "", false ); // "" indicates run all registered tests
  return success;


}
