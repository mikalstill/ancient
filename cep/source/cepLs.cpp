 /* 
  * Imp for the least squares implementation
  * Copyright (C) Kristy Van Der Vlist             2002
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


#include "cepLs.h"

cepLs::cepLs()
{
}

const cepLs & cepLs::cepDoVCV(cepMatrix<double> &data, cepMatrix<double> &matP)
{
  cepMatrix<double> matA, matL;
  
  matA = makeA(data);
  if(m_error.isReal() == true)
  {
    return *this;
  }

  matL = makeL(data);
  if(m_error.isReal() == true)
  {
    return *this;
  }

  sanityCheck(matA, matP);
  if(m_error.isReal() == true)
  {
    return *this;
  }

  calcVCV(matA, matP, matL);
  return *this;
}

const cepLs & cepLs::cepDoVCV(cepMatrix<double> &data)
{

  cepMatrix<double> matA, matL, matP, lastResid;
  
  matA = makeA(data);
  if(m_error.isReal() == true)
  {
    return *this;
  }
  
  matL = makeL(data);
  if(m_error.isReal() == true)
  {
    return *this;
  }
  
  matP = makeP(data);
  if(m_error.isReal() == true)
  {
    return *this;
  }
  
  lastResid = initResiduals(data);
  if(m_error.isReal() == true)
  {
    return *this;
  }
   
  sanityCheck(matA, matP);
  if(m_error.isReal() == true)
  {
    return *this;
  }
  
  cout << "dimsions data " << data.getNumRows() << " " << data.getNumCols() << endl;
  cout << "dimsions A " << matA.getNumRows() << " " << matA.getNumCols() << endl;
  cout << "dimsions P " << matP.getNumRows() << " " << matP.getNumCols() << endl;
  cout << "dimsions L " << matL.getNumRows() << " " << matL.getNumCols() << endl;
  
  calcVCV(matA, matP, matL);
  if(m_error.isReal() == true)
  {
    return *this;
  }
  
  int i = 0;
  while(m_residual != lastResid)
  {
    i++;

    cout << endl << "iteration num " << i << endl;

    cout << "mean: " << getB1() << "Y intercept: " << getB2() << endl;
    cout << "residuals:" << endl;

/*   for(int i = 0; i < m_residual.getNumRows(); i ++){
      cout << m_residual.getValue(i,0) << " ";
      if(m_residual.getError().isReal() == true)
      {
        m_error.setError(m_residual.getError().getMessage(), cepError::sevErrorRecoverable);
        return *this;
      }
    }
    cout << endl;
*/  
    lastResid = m_residual;
    if(lastResid.getError().isReal() == true)
    {
      m_error.setError(lastResid.getError().getMessage(), cepError::sevErrorRecoverable);
      return *this;
    }
    
    matP = reweightVCV(matP);

/*    cout << "reweight: " << endl;
    for(int i = 0; i < matP.getNumRows(); i ++){
      for(int j = 0; j < matP.getNumCols(); j ++){
        cout << matP.getValue(i,j) << " ";
      }
      cout << endl;
    }
*/               
    calcVCV(matA, matP, matL);
    if(m_error.isReal() == true)
    {
      return *this;
    }
//    char blah;
//    cin >> blah;
  }
  return *this;
}

const cepLs & cepLs::cepDoRW(cepMatrix<double> &data, cepMatrix<double> &matP)
{

  cepMatrix<double> matA, matL;

  matA = makeA(data);
  if(m_error.isReal() == true)
  {
    return *this;
  }

  matL = makeL(data);
  if(m_error.isReal() == true)
  {
    return *this;
  }

  sanityCheck(matA, matP);
  if(m_error.isReal() == true)
  {
    return *this;
  }
  calcRW(matA, matP, matL);
  return *this;
}

const cepMatrix<double> &cepLs::getResidual()
{
  return m_residual;
}

double cepLs::getB1()
{
  return m_matX.getValue(0,0);
}

double cepLs::getB2()
{
  return m_matX.getValue(1,0);
}

cepError cepLs::getError()
{
  return m_error;
}

const cepMatrix<double> cepLs::initResiduals(cepMatrix<double> &data)
{
  cepMatrix<double> initResid(data.getNumRows(),1);
  m_error.init();
  
  for(int i = 0; i < initResid.getNumRows(); i ++)
  {
    initResid.setValue(i,0,0);
    if(initResid.getError().isReal() == true)
    {
      m_error.setError(initResid.getError().getMessage(), cepError::sevErrorRecoverable);
      return initResid;
    }
  }

  return initResid;
}

const cepMatrix<double> cepLs::makeP(cepMatrix<double> &data)
{
  cepMatrix<double>matP(data.getNumRows(), data.getNumRows());
  m_error.init();
  
  for(int i = 0; i < matP.getNumRows(); i++)
  {
    for(int j = 0; j < matP.getNumCols(); j ++)
    {
      if(i == j)
      {
        matP.setValue(i,j, 1);
        if(matP.getError().isReal() == true)
        {
          m_error.setError(matP.getError().getMessage(), cepError::sevErrorRecoverable);
          return matP;
        }  
      }
      else
      {
        matP.setValue(i,j, 0);
        if(matP.getError().isReal() == true)
        {
          m_error.setError(matP.getError().getMessage(), cepError::sevErrorRecoverable);
          return matP;
        }
      }
    }
  }
  return matP;      
}

const cepMatrix<double> cepLs::makeA(cepMatrix<double> &data)
{
  cepMatrix<double> matA(data.getNumRows(), 2);
  double scalar = data.getValue(0,0);   //holds the value that A matrix is to be
                                        //scaled by.

  m_error.init();

  for(int i = 0; i < matA.getNumRows(); i++)
  {
      matA.setValue(i,0, data.getValue(i,0) - scalar);
      if(matA.getError().isReal() == true)
      {
        m_error.setError(matA.getError().getMessage(), cepError::sevErrorRecoverable);
        return matA;
      }
      matA.setValue(i,1,1);
      if(matA.getError().isReal() == true)
      {
        m_error.setError(matA.getError().getMessage(), cepError::sevErrorRecoverable);
        return matA;
      }
  }

  return matA;
}

const cepMatrix<double> cepLs::makeL(cepMatrix<double> &data)
{
  cepMatrix<double> matL(data.getNumRows(), 1);

  m_error.init();

  for(int i = 0; i < matL.getNumRows(); i++)
  {
      matL.setValue(i,0, data.getValue(i,1));
      if(matL.getError().isReal() == true)
      {
        m_error.setError(matL.getError().getMessage(), cepError::sevErrorRecoverable);
        return matL;
      }
  }

  return matL;
}

void cepLs::calcVCV(cepMatrix<double> &matA, cepMatrix<double> &matP, cepMatrix<double> &matL)
{
  cepMatrix<double> Atrans, AtransP, AtPInv;

  m_error.init();
  
  Atrans = matA;
  if(Atrans.getError().isReal() == true)
  {
    m_error.setError(Atrans.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  Atrans.transpose();
  if(Atrans.getError().isReal() == true)
  {
    m_error.setError(Atrans.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }
  
  //if matrix is diagonal
  if(matP.isDiagonal() == true)
  {
    AtransP = mulDiag(Atrans, matP);

    if(AtransP.getError().isReal() == true)
    {
      m_error.setError(Atrans.getError().getMessage(), cepError::sevErrorRecoverable);
      goto END;
    }
  
  }
  else
  {
    m_error.setError("Weighting matrix is not strictly diagonal", cepError::sevErrorRecoverable);
    goto END;
  }

  //caluclate (A^TPA)^-1
  AtPInv = mulA(AtransP, matA);
  if(AtPInv.getError().isReal() == true)
  {
    m_error.setError(AtPInv.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }
  
  AtPInv = inverse(AtPInv);
  if(AtPInv.getError().isReal() == true)
  {
    m_error.setError(AtPInv.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  //calcualte A^TPL
  AtransP *= matL;
  if(AtransP.getError().isReal() == true)
  {
    m_error.setError(AtransP.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  AtPInv *= AtransP;
  if(AtPInv.getError().isReal() == true)
  {
    m_error.setError(AtPInv.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  m_matX = AtPInv;

  if(AtPInv.getError().isReal() == true)
  {
    m_error.setError(AtPInv.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  calcResiduals(matA, matL);

END: cout <<"";
}

void cepLs::calcRW(cepMatrix<double> &matA, cepMatrix<double> &matP, cepMatrix<double> &matL)
{
  cepMatrix<double> Atrans, AtransP, AtPInv;

  m_error.init();
  
  Atrans = matA;
  if(Atrans.getError().isReal() == true)
  {
    m_error.setError(Atrans.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  Atrans.transpose();
  if(Atrans.getError().isReal() == true)
  {
    m_error.setError(Atrans.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  AtransP = Amul(Atrans, matP);
  if(AtransP.getError().isReal() == true)
  {
    m_error.setError(Atrans.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  //caluclate (A^TPA)^-1
  AtPInv = mulA(AtransP, matA);
  if(AtPInv.getError().isReal() == true)
  {
    m_error.setError(AtPInv.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  AtPInv = inverse(AtPInv);
  if(AtPInv.getError().isReal() == true)
  {
    m_error.setError(AtPInv.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  //calcualte A^TPL
  AtransP *= matL;
  if(AtransP.getError().isReal() == true)
  {
    m_error.setError(Atrans.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  AtPInv *= AtransP;
  if(AtPInv.getError().isReal() == true)
  {
    m_error.setError(AtPInv.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  m_matX = AtPInv;
  if(AtPInv.getError().isReal() == true)
  {
    m_error.setError(AtPInv.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  calcResiduals(matA, matL);

END: cout <<"";
}

const cepMatrix<double> cepLs::reweightVCV(cepMatrix<double> &matP)
{
  cepMatrix<double> tempResidual, newP(m_residual.getNumRows(), m_residual.getNumRows());
  int numSwap = -1, median;
  double temp, per25, per75, upperQ, lowerQ;
  tempResidual = m_residual;

  m_error.init();
  
  //calculate tempResidual^T * matP
  //this removes any previously detected outliers from the calculations
  for(int i = 0; i < tempResidual.getNumRows(); i++)
  {
    if(matP.getValue(i,i) == 0)
    {
      tempResidual.setValue(i,0,0);
    }
  }
  
  
  //sort residuals in ascending order
  for(int i = 0; i < tempResidual.getNumRows(); i ++){
    if(numSwap == 0)
    {
      break;
    }

    numSwap = 0;
    for(int j = 0; j < tempResidual.getNumRows() - 1 - i; j ++){
      if(tempResidual.getValue(j,0) > tempResidual.getValue(j+1, 0))
      {
        temp = tempResidual.getValue(j,0);
        if(tempResidual.getError().isReal() == true)
        {
          m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
          return newP;
        }

        tempResidual.setValue(j,0,tempResidual.getValue(j+1,0));
        if(tempResidual.getError().isReal() == true)
        {
          m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
          return newP;
        }

        tempResidual.setValue(j+1, 0, temp);
        if(tempResidual.getError().isReal() == true)
        {
          m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
          return newP;
        }
        numSwap++;
      }
    }
  }

  //calc lower & upper quartiles
  if((tempResidual.getNumRows() % 2) == 0)
  {
    median = tempResidual.getNumRows()/2;
  
    if(median %2 == 0)
    {
      lowerQ = (tempResidual.getValue(median/2 - 1,0) + tempResidual.getValue((median/2),0))/2;
      if(tempResidual.getError().isReal() == true)
      {
        m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
        return newP;
      }
      upperQ = (tempResidual.getValue(median/2 + median - 1,0) + tempResidual.getValue((median/2) + median,0))/2;
      if(tempResidual.getError().isReal() == true)
      {
        m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
        return newP;
      }
    }
    else
    {
      lowerQ = tempResidual.getValue(median/2,0);      
      if(tempResidual.getError().isReal() == true)
      {
        m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
        return newP;
      }
      upperQ = tempResidual.getValue(median/2 + median,0);
      if(tempResidual.getError().isReal() == true)
      {
        m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
        return newP;
      }
    }
  }
  else
  {
    median = tempResidual.getNumRows()/2;

    if(median %2 == 0)
    {
      lowerQ = tempResidual.getValue(median/2,0);
      if(tempResidual.getError().isReal() == true)
      {
        m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
        return newP;
      }
      upperQ = tempResidual.getValue((median/2) + median,0);
      if(tempResidual.getError().isReal() == true)
      {
        m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
        return newP;
      }
    }
    else
    {
      lowerQ = (tempResidual.getValue(median/2,0) + tempResidual.getValue(median/2 +1,0))/2;
      if(tempResidual.getError().isReal() == true)
      {
        m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
        return newP;
      }
      upperQ = (tempResidual.getValue(median/2 + median,0) + tempResidual.getValue(median/2 + 1 + median,0))/2;
      if(tempResidual.getError().isReal() == true)
      {
        m_error.setError(tempResidual.getError().getMessage(), cepError::sevErrorRecoverable);
        return newP;
      }
    }
  }  

  per25 = lowerQ - ((upperQ-lowerQ) * 1.5);
  per75 = upperQ + ((upperQ-lowerQ) * 1.5);

  //create new P matrix
  for(int i = 0; i < newP.getNumRows(); i ++)
  {
    for(int j = 0; j < newP.getNumCols(); j ++)
    {
      if(i != j)
      {
        newP.setValue(i,j,0);
        if(newP.getError().isReal() == true)
        {
          m_error.setError(newP.getError().getMessage(), cepError::sevErrorRecoverable);
          return newP;
        }
      }
      else
      {
        if((m_residual.getValue(i,0) < per25) || (m_residual.getValue(i,0) > per75))
        {
          cout << "found outlier at: " << i << endl;
          newP.setValue(i, j, 0);
          if(newP.getError().isReal() == true)
          {
            m_error.setError(newP.getError().getMessage(), cepError::sevErrorRecoverable);
            return newP;
          }
        }
        else
        {
          newP.setValue(i,j, 1);
          if(newP.getError().isReal() == true)
          {
            m_error.setError(newP.getError().getMessage(), cepError::sevErrorRecoverable);
            return newP;
          }
        }
      }
    }
  }    
  return newP; 
}
const cepMatrix<double> cepLs::Amul(cepMatrix<double> &matA, cepMatrix<double> &matB)
{
  cepMatrix<double> ans(matA.getNumRows(), matB.getNumCols());

  m_error.init();
  
  for(int i = 0; i < ans.getNumRows(); i ++ )
  {
    for(int j = 0; j < ans.getNumCols(); j ++ )
    {
      ans.setValue(i, j, 0);
      if(ans.getError().isReal() == true)
      {
        m_error.setError(ans.getError().getMessage(), cepError::sevErrorRecoverable);
        return ans;
      }
    }                        
  }
      
  if(matA.getNumCols() != matB.getNumRows())
  {
    m_error.setError("Error, Invalid Matrix size", cepError::sevErrorRecoverable);
    return ans;
  }

  //do A*B
  for(int i = 0; i < ans.getNumCols(); i ++ )
  {
    for(int j = 0; j < matB.getNumCols(); j ++ )
    {
      ans.setValue(0, i, (ans.getValue(0, i) + matA.getValue(0,j) * matB.getValue(i,j)));           
      if(ans.getError().isReal() == true)
      {
        m_error.setError(ans.getError().getMessage(), cepError::sevErrorRecoverable);
        return ans;
      }
      ans.setValue(1, i, (ans.getValue(1, i) + matB.getValue(i,j)));
      if(ans.getError().isReal() == true)
      {
        m_error.setError(ans.getError().getMessage(), cepError::sevErrorRecoverable);
        return ans;
      }
    }
  }    

  return ans;
}

const cepMatrix<double> cepLs::mulA(cepMatrix<double> &matA, cepMatrix<double> &matB)
{
  cepMatrix<double> ans(matA.getNumRows(), matB.getNumCols());

  m_error.init();
  
  for(int i = 0; i < ans.getNumRows(); i ++ )
  {
    for(int j = 0; j < ans.getNumCols(); j ++ )
    {
      ans.setValue(i, j, 0);
      if(ans.getError().isReal() == true)
      {
        m_error.setError(ans.getError().getMessage(), cepError::sevErrorRecoverable);
        return ans;
      }
    }
  }
   
  if(matA.getNumCols() != matB.getNumRows())
  {
    m_error.setError("Invalid Matrix size", cepError::sevErrorRecoverable);
    return ans;
  }
  
  //do B*A
  for(int i = 0; i < ans.getNumCols(); i ++ )
  {
    for(int j = 0; j < matB.getNumRows(); j ++ )
    {
      ans.setValue(i, 0, (ans.getValue(i, 0) + matA.getValue(i,j) * matB.getValue(j,0)));           
      if(ans.getError().isReal() == true)
      {
        m_error.setError(ans.getError().getMessage(), cepError::sevErrorRecoverable);
        return ans;
      }
      ans.setValue(i, 1, (ans.getValue(i, 1) + matA.getValue(i,j)));
      if(ans.getError().isReal() == true)
      {
        m_error.setError(ans.getError().getMessage(), cepError::sevErrorRecoverable);
        return ans;
      }
    }
  }    
  
  return ans;
}

const cepMatrix<double> cepLs::mulDiag(cepMatrix<double> &matA, cepMatrix<double> &matB)
{
  cepMatrix<double> ans(matA.getNumRows(), matB.getNumCols());

  m_error.init();
  
  if(matA.getNumCols() != matB.getNumRows())
  {
    m_error.setError("Invalid Matrix size", cepError::sevErrorRecoverable);
    return ans;
  }

  //do A*B where B is diagonal
  for(int i = 0; i < ans.getNumRows(); i ++)
  {
    for(int j = 0; j < ans.getNumCols(); j ++)
    {
      ans.setValue(i, j, matA.getValue(i, j) * matB.getValue(j,j));
      if(ans.getError().isReal() == true)
      {
        m_error.setError(ans.getError().getMessage(), cepError::sevErrorRecoverable);
        return ans;
      }
    }
  }
  
  return ans;
}

const cepMatrix<double> cepLs::inverse(cepMatrix<double> &mat)
{
  cepMatrix<double> ans(mat.getNumRows(), mat.getNumCols());

  m_error.init();
  
  ans = mat;
  double mul;
  
  if ( ( mat.getNumCols() != 2 ) || ( mat.getNumRows() != 2 ) )
  {    
    m_error.setError("Invalid Matrix size", cepError::sevErrorRecoverable);
    return ans;
  }

  //calulate det mat
  mul = ( mat.getValue(0, 0) * mat.getValue(1, 1) - mat.getValue(0, 1) * mat.getValue(1, 0) );

  if(mul == 0)
  {
   m_error.setError("Matrix could not be inverted", cepError::sevErrorRecoverable);
    return ans;
  }

  //calculate matrix inverse
  ans.setValue(0,0, mat.getValue(1,1) * 1/mul);
  ans.setValue(0,1, -mat.getValue(0,1) * 1/mul);
  ans.setValue(1,0, -mat.getValue(1,0) * 1/mul);
  ans.setValue(1,1, mat.getValue(0,0) * 1/mul);
  
  return ans;
}

void cepLs::sanityCheck( cepMatrix<double> &matA, cepMatrix<double> &matP)
{
  m_error.init();
  
  //matP must be obs x obs 
  if((matP.getNumCols() != matP.getNumRows()) || (matP.getNumCols() != matA.getNumRows()))
  {             
    m_error.setError("Matrix P has the wrong dimesions", cepError::sevErrorRecoverable);
  }
}

void cepLs::calcResiduals(cepMatrix<double> &matA, cepMatrix<double> &matL)
{
  m_error.init();
  //calc rediuals= A*X - L
  m_residual = matA;
  if(m_residual.getError().isReal() == true)
  {
    m_error.setError(m_residual.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  m_residual *= m_matX;
  if(m_residual.getError().isReal() == true)
  {
    m_error.setError(m_residual.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

  m_residual -= matL;
  if(m_residual.getError().isReal() == true)
  {
    m_error.setError(m_residual.getError().getMessage(), cepError::sevErrorRecoverable);
    goto END;
  }

END: cout <<"";  
}
