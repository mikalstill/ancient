/* 
   Imp for the CEP dataset
   Copyright (C) Michael Still                    2002
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef CEP_DATASET_HEADER
#define CEP_DATASET_HEADER

#include <vector>



class cepError;

typedef void (*cepDatasetProgressCB) (int plane, long lineno);

typedef struct cep_internal_datarow
{
  float date;
  float sample;
  float error;
} cep_datarow;

// This is used for the 'columns' in the windowing algorithm
typedef vector<cep_datarow> cep_datacol;

class cepDataset
{
public:
  // Filename is the "root filename" e.g. mb_ANKR_GPS
  // I append the .dat1, .dat2 and .dat3 myself...
  cepDataset (string filename);
  cepDataset (string filename, cepDatasetProgressCB callback);
  cepDataset (vector<cep_datacol> windowVector, int numWindows);
  cepDataset (double value, double weight);

  // Manipulations 
  cepDataset doWindow (cepDataset dir, double winSize, double overlap);
  cepDataset doHam (double datRow[3], double startWindow, double winSize);
  
  // Actually process the file
  cepError munch ();

  enum direction
  {
    x = 1,
    y,
    z
  };

  // Accessor methods
  vector<cep_datarow>& getDataPtr(direction);

private:
  string m_filename;
  cepDatasetProgressCB m_progress;
  vector<cep_datarow> m_datax, m_datay, m_dataz;
  vector<cep_datacol> m_windowVector; //vector of windowed data
  int m_numWindows; //number of windows in the windowed data
  double m_hamValue; //single haming value
  double m_hamWeight; //hamming weight
  

};

#endif
