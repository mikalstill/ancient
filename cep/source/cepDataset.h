
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
#include <sstream>
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

class cepDataset {
  public:
    enum direction {
        dirX = 0,
        dirY,
        dirZ,
        dirUnknown
    };

    enum column {
        colDate = 0,
        colSample,
        colError,
        colColourHint
    };

    // Filename is the "root filename" e.g. mb_ANKR_GPS
    // I append the .dat1, .dat2 and .dat3 myself...
     cepDataset();
     cepDataset(const cepDatasetProgressCB callback);

    // 19 parameters. might this be overkill?
    // requires a rethink
     cepDataset(cepMatrix < double >*data0, cepMatrix < double >*data1,
                cepMatrix < double >*data2, string offset0,
                string offset1, string offset2, string procHistory,
                string header0, string header1, string header2,
                double b1_0 = 0.0, double b1_1 = 0.0, double b1_2 = 0.0, double b2_0 = 0.0,
                double b2_1 = 0.0, double b1_2 = 0.0, bool haveLs0 = false, bool haveLs1 = false,
                bool haveLs2 = false);

    // Actually process the file
    cepError read(const string & filename);
    cepError write(const string & filename);
    bool isReady();
    bool isWellFormed();

    // Accessor methods
     cepMatrix < double >*getMatrix(direction dir);
    double getB1(direction dir);
    double getB2(direction dir);

    // Return a filtered dataset
    cepDataset filter(float low, float high);

    // Utility methods
    direction getDirectionFromName(string name);
    string getRootFilename();
    string getName();
    string getProcHistory();
    string getOffset(direction i);
    string getHeader(direction i);
    bool getHaveLs(direction i);
    bool isFreqDomain();

  private:
    string applyOffset(direction i, string value);
    string reverseOffset(direction i, string value);
    void delimitWindow( fstream &f );

    string m_filename;
    string m_header[dirUnknown];
    string m_offset[dirUnknown];
    string m_procHistory;
    float m_offsetFloat[dirUnknown];

    cepDatasetProgressCB m_progress;
     cepMatrix < double >*m_data[dirUnknown];
    bool m_ready;
    bool m_wellformed;
    bool frequencyData;

    double m_b1[dirUnknown];
    double m_b2[dirUnknown];
    bool m_haveLs[dirUnknown];

    static const double delim;
};


class cepVector4D {
  public:
    cepVector4D();
    void push_back(double xval, double yval, double zval, double cval);
    int size();
    double Xat(int &index);
    double Yat(int &index);
    double Zat(int &index);
    double Cat(int &index);
  protected:
     vector < double >x;
     vector < double >y;
     vector < double >z;
     vector < double >c;
};

#endif
