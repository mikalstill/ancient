/*
 * Imp for the least squares implementation
 *
 *   Copyright (C) Kristy Van Der Vlist             2002
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 675
 * Mass Ave, Cambridge, MA 02139, USA.
 */


/******************************************************************************
DOCBOOK START

FUNCTION cepLs


PURPOSE Cacluates a Linear Least Squares transformation on a given dataset. It offers three
types of Least Squares transfomations:-

<para><itemizedlist>
  <listitem><para>Variance Co-Variance (VCV)</para></listitem>
  <listitem><para>Variance Co-Variance (with automated data re-weighting)</para></listitem>
  <listitem><para>Random Walk</para></listitem>
</itemizedlist></para>

SYNOPSIS START
The follwing is an example of how to create a least squares object.

cepLs ls;

SYNOPSIS END

DESCRIPTION START
An implementation of the <command>cepLs</command> class
which calculates a Linear Least Squares transformation.
</para>

<para>
<command>const cepLs & cepDoVCV(cepMatrix<double> &data, cepMatrix<double> &matP)</command>
Calculates one iteration of the VCV least squares algoritum.
<para><itemizedlist>
  <listitem><para>data:- The data matrix</para></listitem>
  <listitem><para>matP:- The given weighting matrix</para></listitem>
</itemizedlist></para>
</para>

<para>
<command>const cepLs & cepDoVCV(cepMatrix<double> &data)</command>
Caculates the VCV least squares algorithum, automatically re-weighting the
weighting matrix (P matrix) untill the residuals are stable.
<para><itemizedlist>
  <listitem><para>data:- The data matrix</para></listitem>
</itemizedlist></para>

</para>

<para>
<command>const cepLs & cepDoRW(cepMatrix<double> &matA, cepMatrix<double> &matP)</command>
Calculates one iteration of the Random Walk least squares algorithum
<para><itemizedlist>
  <listitem><para>data:- The data matrix</para></listitem>
  <listitem><para>matP:- The given weighting matrix</para></listitem>
</itemizedlist></para>
</para>

<para>
<command>const cepMatrix<double> &getResidual()</command>
Returns a cepMatrix which is the residuals for this caluclation.
</para>

<para>
<command>double getB1()</command>
Returns the value B1 in the least squares solution y=B1*x +B2.
</para>

<para>
<command>double getB2()</command>
Returns the value B2 in the solution y=B1*x +B2
</para>

<para>
<command>cepError getError()</command>
Returns any error that may have occoured

SEEALSO cepDataset
SEEALSO cepMatrix
SEEALSO cepError

DESCRIPTION END

DOCBOOK END
******************************************************************************/

#ifndef __CEPLS_H
#define __CEPLS_H

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>

#include  "cepMatrix.h"

class cepLs
{
public:

  cepLs ();

  //do one iteration of the least squares algoritum
  const cepLs & cepDoVCV (cepMatrix < double >&data,
			  cepMatrix < double >&matP);

  //iterate least squares algoritum until stable
  const cepLs & cepDoVCV (cepMatrix < double >&data);

  //do one iteration of the least squares algoritum
  const cepLs & cepDoRW (cepMatrix < double >&matA,
			 cepMatrix < double >&matP);

  //get the new residual data matrix   
  const cepMatrix < double >&getResidual ();

  //get the new dataset matrix
  const cepMatrix < double >&getDataset ();

  //get the value B1 in the least squares solution y=B1*x +B2  
  const double getB1 ();

  //get the value B2 in the solution y=B1*x +B2  
  const double getB2 ();

  //returns any error that may have occoured
  cepError getError ();
private:

    cepMatrix < double >m_residual,	//holds the value of the residuals 
    m_matX,			//holds the value of B1, B2
    m_residData, m_dataset;

  cepError m_error;
  //ensure that all values of the matrix A P and L are consistant with the
  //least squares alogrithum.
  void sanityCheck (cepMatrix < double >&matA, cepMatrix < double >&matP);

  //calculate the residuals of the least squares tranformation
  void calcResiduals (cepMatrix < double >&matA, cepMatrix < double >&matL);

  //make the intial P matrix
  const cepMatrix < double >initResiduals (cepMatrix < double >&data);

  //make the intial P matrix
  const cepMatrix < double >makeP (cepMatrix < double >&data);

  //make the A matrix
  const cepMatrix < double >makeA (cepMatrix < double >&data);

  //make the L matrix
  const cepMatrix < double >makeL (cepMatrix < double >&data);

  //calc least square for a VCV matrix
  void calcVCV (cepMatrix < double >&matA, cepMatrix < double >&matP,
		cepMatrix < double >&matL);

  //calc least squares for a RW matrix
  void calcRW (cepMatrix < double >&matA, cepMatrix < double >&matP,
	       cepMatrix < double >&matL);

  //re-caluclates the P weighting matrix
  void reweightVCV (cepMatrix < double >&matP);

  //calculate the matrix inverse
  const cepMatrix < double >inverse (cepMatrix < double >&mat);

  //calculate A*B where B is a diagonal
  const cepMatrix < double >mulDiag (cepMatrix < double >&matA,
				     cepMatrix < double >&matB);

  //calculates A*B where A is the design matrix in the least squares tranform
  const cepMatrix < double >Amul (cepMatrix < double >&matA,
				  cepMatrix < double >&matB);

  //calculates B*A where A is the design matrix in the least squares tranform
  const cepMatrix < double >mulA (cepMatrix < double >&matA,
				  cepMatrix < double >&matA);

  void makeDatasets (cepMatrix < double >&data, cepMatrix < double >&matP);
};

#endif //end __CEPLS_H
