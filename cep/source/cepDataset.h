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

typedef struct cep_internal_datadirection
{
  float date;
  float sample;
  float error;
} cep_datadirection;

typedef struct cep_internal_datarow
{
  cep_datadirection x, y, z;
} cep_datarow;

class cepDataset
{
public:
  // Filename is the "root filename" e.g. mb_ANKR_GPS
  // I append the .dat1, .dat2 and .dat3 myself...
  cepDataset (string filename);
  cepDataset (string filename, cepDatasetProgressCB callback);

  // Actually process the file
  cepError munch ();

private:
  string m_filename;
  cepDatasetProgressCB m_progress;
  vector<cep_datarow> m_data;
  vector<cep_datarow> windowVector; //vector of windowed data
  int m_numWindows; //number of windows in the windowed data
  double m_hamValue; //single haming value
  double m_hamWeight; //hamming weight

};

#endif
