/***************************************************************************
 * Testing Framework for cepLs
 *
 * (c) Kristy Van Der Vlist 2002 ..
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
#include <iomanip>

#include "../cepLs.h"
#include "../cepDataset.h"
#include "../cepError.h"
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
 * @version $Revision: 1.12 $ $Date: 2002-11-11 06:14:14 $
 *
 * Revision History
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.11  2002/11/10 23:43:28  u983118
 * added tests for new fuctionality
 *
 * Revision 1.10  2002/11/03 02:52:05  u983118
 * added tests for error stuff
 *
 * Revision 1.9  2002/10/24 07:58:06  u983118
 * added test for the buggy dataset
 *
 * Revision 1.8  2002/10/01 06:19:33  u983118
 * added some testing for auto re-weight VCV LS
 *
 * Revision 1.6  2002/09/08 05:50:27  u983118
 * Updated test for new way that the date, sample, error stuff is stored
 *
 * Revision 1.5  2002/08/24 01:51:02  u983118
 * new tests for cepMatrix template, cepLs core implementation
 *
 * Revision 1.4  2002/08/18 03:12:47  u983118
 * reworked tests for cepMatrix and cepLs
 *
 * Revision 1.3  2002/08/12 10:04:39  u983118
 * *** empty log message ***
 *
 *
 */

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
      new CppUnit::TestCaller<Test>( "testLsVCV", &Test::testLsVCV ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testLsVCVReweight", &Test::testLsVCVReweight ) );
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testLsVCVReweight2", &Test::testLsVCVReweight2 ) );
    
    suiteOfTests->addTest(
      new CppUnit::TestCaller<Test>( "testLsRW", &Test::testLsRW ) );
                  
    return suiteOfTests;
  }

protected:
  /**
   * DEFINE YOUR TESTS HERE:
   * make your tests protected since you dont need to expose them
   */

  /** Tests Ls */
  
  //test VCV LS
  void testLsVCV()
  {
    cepMatrix<double> data(5,4), P(5,5), residual, newData;
    cepLs ans;
    
    //define the data matrix
    //taken from the mb_PMAC_GPS.dat1
    data.setValue(0,0,2000.1589);
    data.setValue(1,0,2000.1626);
    data.setValue(2,0,2000.1653);
    data.setValue(3,0,2000.1680);
    data.setValue(4,0,2000.1708);
    
    data.setValue(0,1,-1.6239);
    data.setValue(1,1,-1.6259);
    data.setValue(2,1,-1.6255);
    data.setValue(3,1,-1.6234);
    data.setValue(4,1,-1.6242);
    
    data.setValue(0,2,0.0012);
    data.setValue(1,2,0.0014);
    data.setValue(2,2,0.0015);
    data.setValue(3,2,0.0015);
    data.setValue(4,2,0.0015);
    
    data.setValue(0,3,1.00);
    data.setValue(1,3,2.00);
    data.setValue(2,3,3.00);
    data.setValue(3,3,4.00);
    data.setValue(4,3,5.00);
    
    //define P matrix
    P.setValue(0,0,1);
    P.setValue(1,0,0);
    P.setValue(2,0,0);
    P.setValue(3,0,0);
    P.setValue(4,0,0);
    
    P.setValue(0,1,0);
    P.setValue(1,1,1);
    P.setValue(2,1,0);
    P.setValue(3,1,0);
    P.setValue(4,1,0);
    
    P.setValue(0,2,0);
    P.setValue(1,2,0);
    P.setValue(2,2,1);
    P.setValue(3,2,0);
    P.setValue(4,2,0);
    
    P.setValue(0,3,0);
    P.setValue(1,3,0);
    P.setValue(2,3,0);
    P.setValue(3,3,1);
    P.setValue(4,3,0);
    
    P.setValue(0,4,0);
    P.setValue(1,4,0);
    P.setValue(2,4,0);
    P.setValue(3,4,0);
    P.setValue(4,4,1);
    
    ans.cepDoVCV(data, P);
    residual = ans.getResidual();
    newData = ans.getDataset();
    
    
   //test for new data matrix
    for(int i = 0; i < newData.getNumRows(); i ++){
      for(int j = 0; j < 3; j++){
        CPPUNIT_ASSERT_DOUBLES_EQUAL( data.getValue(i,j), newData.getValue(i,j), 0.01 );
      }
    }
    
    //test for Colours
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(0,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(1,3), 0.01 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(2,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(3,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(4,3), 0.01 );

    //tests for B1 and B2
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.05241276, ans.getB1(), 0.00000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.624906007, ans.getB2(), 0.000000001 );
    
    
    //test for new residual dates and errors data matrix
    for(int i = 0; i < newData.getNumRows(); i ++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( data.getValue(i,0), residual.getValue(i,0), 0.01 );
      CPPUNIT_ASSERT_DOUBLES_EQUAL( data.getValue(i,2), residual.getValue(i,2), 0.01 );
    }  
    //tests for residuals 
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.001006007, residual.getValue(0,1), 0.000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.00118792021,  residual.getValue(1,1), 0.000000001 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.00092943466, residual.getValue(2,1), 0.000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.00102905088, residual.getValue(3,1), 0.000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.00008229516, residual.getValue(4,1), 0.000000001 );
    
    
    //test for Colours
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(0,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(1,3), 0.01 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(2,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(3,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(4,3), 0.01 );
  }
  
  //test the re-weighting of VCV
  void testLsVCVReweight()
  {
    cepMatrix<double> data(8,3), residual, newData;
    cepLs ans;

    data.setValue(0,0,1.0);
    data.setValue(1,0,2.0);
    data.setValue(2,0,3.0);
    data.setValue(3,0,4.0);
    data.setValue(4,0,5.0);
    data.setValue(5,0,6.0);
    data.setValue(6,0,7.0);
    data.setValue(7,0,8.0);
        
    data.setValue(0,1,0.5);
    data.setValue(1,1,2.0);
    data.setValue(2,1,-2.0);  //this is an outlier!
    data.setValue(3,1,5.0);
    data.setValue(4,1,6.5);
    data.setValue(5,1,8.0);
    data.setValue(6,1,9.5);
    data.setValue(7,1,11.0);
       
    data.setValue(0,2,0.0012);
    data.setValue(1,2,0.0014);
    data.setValue(2,2,0.0015);
    data.setValue(3,2,0.0015);
    data.setValue(4,2,0.0015);
    data.setValue(5,2,0.0015);
    data.setValue(6,2,0.0015);
    data.setValue(7,2,0.0015);
    
    ans.cepDoVCV(data);
    residual = ans.getResidual();
    newData = ans.getDataset();
          
    //tests for B1 and B2
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, ans.getB1(), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, ans.getB2(), 0.01 );
    
    cout << endl << "y=" << ans.getB1() <<"x + " << ans.getB2() << endl;
    
    cout << endl << "data" << endl;      
    //test for new data matrix
    for(int i = 0; i < newData.getNumRows(); i ++){
      for(int j = 0; j < newData.getNumCols(); j++){
        cout << newData.getValue(i,j) << " ";
      }
      cout << endl;
    }
    
    for(int i = 0; i < residual.getNumRows(); i ++){
      for(int j = 0; j < residual.getNumCols(); j++){
        cout << residual.getValue(i,j) << " ";
      }
      cout << endl;
    }
   //test for new data matrix
    for(int i = 0; i < newData.getNumRows(); i ++){
      for(int j = 0; j < 3; j++){
        CPPUNIT_ASSERT_DOUBLES_EQUAL( data.getValue(i,j), newData.getValue(i,j), 0.01 );
      }
    }
    
    //test for data Colours
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(0,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(1,3), 0.01 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.0, newData.getValue(2,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(3,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(4,3), 0.01 );


    //test for new residual dates and errors data matrix
    for(int i = 0; i < newData.getNumRows(); i ++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( data.getValue(i,0), residual.getValue(i,0), 0.01 );
      CPPUNIT_ASSERT_DOUBLES_EQUAL( data.getValue(i,2), residual.getValue(i,2), 0.01 );
    }  

    //test for residuals
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, residual.getValue(0,1), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, residual.getValue(1,1), 0.01 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, residual.getValue(2,1), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, residual.getValue(3,1), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, residual.getValue(4,1), 0.01 );
    
    //test for Colours
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(0,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(1,3), 0.01 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(2,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(3,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(4,3), 0.01 );    
  }
  
  void testLsVCVReweight2()
  {
    cepMatrix<double> *data, residual;
    cepDataset blah;
    cepLs ans;
    
    blah.read("../../datasets/mb_AUCK_GPS");
    
    data = blah.getMatrix(cepDataset::dirX);
    
    ans.cepDoVCV(*data);
    residual = ans.getResidual();
        
  }
  
  //test RW LS
  void testLsRW()
  {
    cepMatrix<double> data(5,3), P(5,5), residual, newData;
    cepLs ans;
    //define the data matrix
    //taken from the mb_PMAC_GPS.dat1
    data.setValue(0,0,2000.1589);
    data.setValue(1,0,2000.1626);
    data.setValue(2,0,2000.1653);
    data.setValue(3,0,2000.1680);
    data.setValue(4,0,2000.1708);
    
    data.setValue(0,1,-1.6239);
    data.setValue(1,1,-1.6259);
    data.setValue(2,1,-1.6255);
    data.setValue(3,1,-1.6234);
    data.setValue(4,1,-1.6242);
    
    data.setValue(0,2,0.0012);
    data.setValue(1,2,0.0014);
    data.setValue(2,2,0.0015);
    data.setValue(3,2,0.0015);
    data.setValue(4,2,0.0015);
    
    //define P matrix
    P.setValue(0,0,1);
    P.setValue(1,0,0);
    P.setValue(2,0,0);
    P.setValue(3,0,0);
    P.setValue(4,0,0);
    
    P.setValue(0,1,0);
    P.setValue(1,1,1);
    P.setValue(2,1,0);
    P.setValue(3,1,0);
    P.setValue(4,1,0);
    
    P.setValue(0,2,0);
    P.setValue(1,2,0);
    P.setValue(2,2,1);
    P.setValue(3,2,0);
    P.setValue(4,2,0);
    
    P.setValue(0,3,0);
    P.setValue(1,3,0);
    P.setValue(2,3,0);
    P.setValue(3,3,1);
    P.setValue(4,3,0);
    
    P.setValue(0,4,0);
    P.setValue(1,4,0);
    P.setValue(2,4,0);
    P.setValue(3,4,0);
    P.setValue(4,4,1);
    
    ans.cepDoVCV(data, P);
    residual = ans.getResidual();
    newData = ans.getDataset();
       
      
    //tests for B1 and B2
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.05241276, ans.getB1(), 0.00000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.624906007, ans.getB2(), 0.000000001 );
    
    //test for new data matrix
    for(int i = 0; i < newData.getNumRows(); i ++){
      for(int j = 0; j < 3; j++){
        CPPUNIT_ASSERT_DOUBLES_EQUAL( data.getValue(i,j), newData.getValue(i,j), 0.01 );
      }
    }
    
    //test for data Colours
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(0,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(1,3), 0.01 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(2,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(3,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, newData.getValue(4,3), 0.01 );

    //test for new residual dates and errors data matrix
    for(int i = 0; i < newData.getNumRows(); i ++){
      CPPUNIT_ASSERT_DOUBLES_EQUAL( data.getValue(i,0), residual.getValue(i,0), 0.01 );
      CPPUNIT_ASSERT_DOUBLES_EQUAL( data.getValue(i,2), residual.getValue(i,2), 0.01 );
    }  

    //tests for residuals 
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.001006007, residual.getValue(0,1), 0.000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.00118792021,  residual.getValue(1,1), 0.000000001 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.00092943466, residual.getValue(2,1), 0.000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.00102905088, residual.getValue(3,1), 0.000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.00008229516, residual.getValue(4,1), 0.000000001 );
    
    
    //test for Colours
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(0,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(1,3), 0.01 ); 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(2,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(3,3), 0.01 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, residual.getValue(4,3), 0.01 );
 
  }    
}; // end Test

 /**
  * Register the test immeditely after definition. This should probably
  * be done in the class header file for larger projects
  */
CPPUNIT_TEST_SUITE_REGISTRATION( Test );

} // end namespace
