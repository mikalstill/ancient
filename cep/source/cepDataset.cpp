
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

#include "cepCore.h"

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
  long lines[3];

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
    return
      cepError("File IO error for this dataset. Could not open the file(s):" +
       errString + ".");
  }

  // Read the file
  for (int i = 0; i < 3; i++)
  {
    if (m_progress)
      m_progress (i + 1, 0);

    // Skip the first three lines -- it seems that fstream has no equivalent
    // of fgets(), which will mind the newlines for me...
    char c = 0, prevc = '\n';

    lines[i] = 0;
    string thisLine ("");

    while (!files[i].eof ())
    {
      cep_datarow lastRow;
      lastRow.date = -1;
      lastRow.sample = -1;
      lastRow.error = -1;
      
      files[i].read (&c, 1);

      // We skip the first three lines of the file
      if ((lines[i] > 3) && (c != 'r') && (c != '\n'))
      {
        if (cepIsBlank (c))
        {
          if (!cepIsBlank (prevc))
          {
            thisLine += " ";
          }
        }
        else
        {
          thisLine += c;
        }
      }

      if (c == '\n')
      {
        lines[i]++;

        if ((lines[i] > 4) && (thisLine.length() > 5))
        {
          // Put the data into the dataset data thingies
          // todo_mikal: is there a more c++ way to tokenize a string?
          cep_datarow row;
          char *line;
          char *value;

          line = strdup (thisLine.c_str ());
          row.date = atof ((value = strtok (line, " ")) != NULL ? value : "-1");
          row.sample =
            atof ((value = strtok (NULL, " ")) != NULL ? value : "-1");
          row.error =
            atof ((value = strtok (NULL, " ")) != NULL ? value : "-1");
          if (line != NULL)
            free (line);

          if(lastRow.date == -1)
          {
            getData ((cepDataset::direction) i).push_back (row);
            lastRow = row;
          }
          else
          {
            if(row.date == lastRow.date)
            {
              return cepError("The file " + m_filename + " contains repeated values for date " + cepToString(row.date));
            }
            else
            {
              if(row.date < lastRow.date)
              {
                return cepError("The file " + m_filename + " is not in date order");
              }
              else
              {
                if((isnan(row.date) == 0) && (isnan(row.sample) == 0) && (isnan(row.error) == 0))
                {
                  getData ((cepDataset::direction) i).push_back (row);
                  lastRow = row;
                }
              }
            }
          }  

          thisLine = "";
        }
        else
          cepDebugPrint ("Dataset line from " + m_filename + "[" +
                         cepToString (i) + "] skipped...");

        if (m_progress)
          m_progress (i + 1, lines[i]);
      }

      prevc = c;
    }
  }

  // Are the files over the same period??
  if (((m_datax.front().date != m_datay.front().date) || (m_datay.front().date != m_dataz.front().date)) || ((m_datax.back().date != m_datay.back().date) || (m_datay.back().date != m_dataz.back().date)))
  {
    return cepError("The data set values do not represent the same time period");
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
  cepMatrix < double > mat(vec.size(), 3);

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
