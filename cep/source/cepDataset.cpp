
/* 
 *   Imp for the CEP dataset
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Daniel Fernandez                 2002
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

//#include "cepCore.h"

#include "cepDataset.h"

cepDataset::cepDataset (const string & filename):
  m_filename (filename),
  m_ready(false)
{
  m_progress = NULL;
}

cepDataset::cepDataset (const string & filename, cepDatasetProgressCB callback):
m_filename (filename),
  m_ready(false)
{
  m_progress = callback;
}


cepError cepDataset::munch ()
{
  // Step One: ensure that all of the files we need are actually there,
  // it would be crap to read two of the three files, and _then_ report
  // an error...
  fstream files[3];

  files[0].open (string (m_filename + ".dat1").c_str (), ios::in);
  files[1].open (string (m_filename + ".dat2").c_str (), ios::in);
  files[2].open (string (m_filename + ".dat3").c_str (), ios::in);

  // Check they opened ok
  string errString;

  for (int i = 0; i < 3; i++)
  {
    // File is NULL if it couldn't be opened
    if (!files[i])
    {
      if (errString != "")
        errString += ";";
      errString += " " + m_filename + ".dat" + cepToString (i + 1);
    }
  }

  if (errString != "")
  {
    return cepError("File IO error for this dataset. Could not open the file(s):" + errString + ".");
  }

  // Read the file
  for (int i = 0; i < 3; i++)
  {
    if (m_progress)
      m_progress (i + 1, 0);

    char c = 0, prevc = '\n';
    string thisLine ("");

    cep_datarow lastRow;
    lastRow.date = -1;
    lastRow.sample = -1;
    lastRow.error = -1;

    while (!files[i].eof ())
    {   
      files[i].read (&c, 1);

      //if the first non blank char in the line is a char then skip the whole line
      if((prevc == '\n') && ((c < 48) || (c > 57)) && (cepIsBlank(c) == false) && (!files[i].eof()))
      {
        while(c != '\n')
        {
          files[i].read(&c,1);
        }
      }
      else
      {
        if(cepIsBlank(c) == true)
        {
          if(cepIsBlank(prevc) != true)
          {
            thisLine += " ";
          }
        }
        else
        {
          thisLine += c;
        }
      }
      if ((c == '\n') && (thisLine != ""))
      {
        cep_datarow row;
        char * line = strdup(thisLine.c_str());

        row.date = atof(strtok(line, " "));
        row.sample = atof(strtok(NULL, " "));
        row.error = atof(strtok(NULL, " "));

        if((isnan(row.date) == 0) && (isnan(row.sample) == 0) && (isnan(row.error) == 0))
        {
          if(lastRow.date == -1)
          {
            getData((cepDataset::direction) i).push_back(row);
            lastRow = row;
          }
          else
          {
            if(row.date < lastRow.date)
            {
              #ifdef debug
                cout << "not in date order" << endl;
              #endif
              return cepError("dataset: " + m_filename + " is not in date order!", cepError::sevErrorRecoverable);
            }
            else
            {
              if(row.date == lastRow.date)
              {
                #ifdef debug
                  cout << "is repeat values" << endl;
                #endif
                return cepError("dataset: " + m_filename + " contains repeated values!",cepError::sevErrorRecoverable);
              }
              else
              {
                getData((cepDataset::direction) i).push_back(row);
                lastRow = row;
              }
            }
          }
        }
        else
        {
          #ifdef debug
            cout << "nan value found";
          #endif
        }  
      thisLine = "";
      }
      prevc = c;
    }
    files[i].close();
  }

  // Are the files over the same period??
  if (((m_datax.front().date != m_datay.front().date) || (m_datay.front().date != m_dataz.front().date)) || ((m_datax.back().date != m_datay.back().date) || (m_datay.back().date != m_dataz.back().date)))
  {
    #ifdef debug
      cout << "dataset not cover same time period";
    #endif
    return cepError("The data set values do not represent the same time period",cepError::sevErrorRecoverable);
  }
    
  m_ready = true;
  return cepError ();
}

vector < cep_datarow > &cepDataset::getData (direction dir)
{
  switch (dir)
  {
  case dirX:
    return m_datax;

  case dirY:
    return m_datay;

  case dirZ:
    return m_dataz;

  default:
    cepError oor ("The data direction requested was out of range",
                  cepError::sevErrorFatal);
    oor.display ();
  }
  return m_datax;
}

cepMatrix < double > cepDataset::getMatrix(direction dir)
{
  vector < cep_datarow > vec = getData(dir);
  cepMatrix < double > mat((signed)vec.size(), 3);

  for(int i = 0; i < (signed)vec.size(); i ++)
  {
    mat.setValue(i, 0, vec[i].date);
    mat.setValue(i, 1, vec[i].sample);
    mat.setValue(i, 2, vec[i].error);
  }
  return mat;
}
bool cepDataset::isReady()
{
  return m_ready;
}
