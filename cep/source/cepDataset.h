
/* 
 *   Imp for the CEP dataset
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Daniel Fernandez                 2002
 *   Copyright (C) Kristy Van Der Vlist             2002
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

#ifndef CEP_DATASET_HEADER
#define CEP_DATASET_HEADER

#include <vector>
#include <string>
#include <fstream>
#include "cepMatrix.h"
#include "cepError.h"
#include "cepUtility.h"


class cepError;

/******************************************************************************
DOCBOOK START

FUNCTION cepDatasetProgressCB
PURPOSE report progress on dataset parsing to the calling method

SYNOPSIS START
typedef void (*cepDatasetProgressCB) (int plane, long lineno);
SYNOPSIS END

DESCRIPTION START
This type is used to define a callback function which will be called whenever a line has been read from the dataset files. <command>plane</command> will the plane that the line belongs to (for example: x, y, and z) and <command>lineno</command> is the line number for the current dataset file.
DESCRIPTION END

SEEALSO cepDataset
DOCBOOK END
******************************************************************************/

typedef void (*cepDatasetProgressCB) (int plane, long lineno);

typedef struct cep_internal_datarow
{
  double date;
  double sample;
  double error;
}
cep_datarow;

// This is used for the 'columns' in the windowing algorithm
typedef vector < cep_datarow > cep_datacol;

class cepDataset
{
public:
  enum direction
  {
    dirX = 0,
    dirY,
    dirZ
  };

  // Filename is the "root filename" e.g. mb_ANKR_GPS
  // I append the .dat1, .dat2 and .dat3 myself...
  cepDataset ();
  cepDataset (const cepDatasetProgressCB callback);

  // Actually process the file
  cepError read(const string& filename);
  bool isReady();
  bool isWellFormed();

  // Accessor methods
  vector < cep_datarow > &getData (direction dir);
  cepMatrix <double> *getMatrix(direction dir);

  // Utility methods
  direction getDirectionFromName(string name);

private:
    string m_filename;
    cepDatasetProgressCB m_progress;
    vector < cep_datarow > m_datax, m_datay, m_dataz;
    bool m_ready;
    bool m_wellformed;
};

#endif
