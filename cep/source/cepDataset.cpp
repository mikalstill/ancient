
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

#include "cepDataset.h"
#include "cepStringArray.h"

cepDataset::cepDataset():
  m_filename(""), m_procHistory(""), m_ready(false), m_wellformed(false), m_frequencyData(false)
{
    for (int i = 0; i < dirUnknown; i++) {
        m_data[i] = NULL;
        m_offset[i] = "";
        m_offsetFloat[i] = 0.0;
        m_b1[i] = -1.0;
        m_b2[i] = -1.0;
        m_haveLs[i] = false;
    }

    m_progress = NULL;
}

cepDataset::cepDataset(cepDatasetProgressCB callback):
m_filename(""), m_procHistory(""), m_ready(false), m_wellformed(false), m_frequencyData(false)
{
    for (int i = 0; i < dirUnknown; i++) {
        m_data[i] = NULL;
        m_offset[i] = "";
        m_offsetFloat[i] = 0.0;
        m_b1[i] = -1.0;
        m_b2[i] = -1.0;
        m_haveLs[i] = false;
    }

    m_progress = callback;
}

cepDataset::cepDataset(cepMatrix < double >*data0,
                       cepMatrix < double >*data1,
                       cepMatrix < double >*data2, string offset0,
                       string offset1, string offset2, string procHistory,
                       string header0, string header1, string header2,
                       double b1_0, double b1_1, double b1_2, double b2_0,
                       double b2_1, double b2_2, bool haveLs0,
                       bool haveLs1, bool haveLs2):m_filename(""),
m_procHistory(procHistory), m_ready(true), m_wellformed(true), m_frequencyData(false)
{
    m_data[0] = data0;
    m_data[1] = data1;
    m_data[2] = data2;

    m_offset[0] = offset0;
    m_offsetFloat[0] = atof(offset0.c_str());
    m_offset[1] = offset1;
    m_offsetFloat[1] = atof(offset1.c_str());
    m_offset[2] = offset2;
    m_offsetFloat[2] = atof(offset2.c_str());

    m_header[0] = header0;
    m_header[1] = header1;
    m_header[2] = header2;

    m_b1[0] = b1_0;
    m_b1[1] = b1_1;
    m_b1[2] = b1_2;

    m_b2[0] = b2_0;
    m_b2[1] = b2_1;
    m_b2[2] = b2_2;

    cepDebugPrint("New dataset line of best fit: " + cepToString(haveLs0) + " " + cepToString(haveLs1) +
		  " " + cepToString(haveLs2));
    m_haveLs[0] = haveLs0;
    m_haveLs[1] = haveLs1;
    m_haveLs[2] = haveLs2;
}

cepError cepDataset::read(const string & filename)
{
    m_filename = filename;
    m_frequencyData = false;

    // Step One: ensure that all of the files we need are actually there,
    // it would be bad to read two of the three files, and _then_ report
    // an error...
    fstream files[3];
    long numLines;
    char c, prevc;
    string thisLine;
    // BS - added color and initialisation
    double rowdate = 0.0, rowsample = 0.0, rowerror = 0.0, rowcolor = 0.0;
    double lastRowdate = -1,
        lastRowsample = -1, lastRowerror = -1, lastRowcolor = -1;

    files[0].open(string(m_filename + ".dat1").c_str(), ios::in);
    files[1].open(string(m_filename + ".dat2").c_str(), ios::in);
    files[2].open(string(m_filename + ".dat3").c_str(), ios::in);
    cepDebugPrint("Opened the dataset files");

    // Check they opened ok
    string errString;

    for (int i = 0; i < 3; i++) {
        // File is NULL if it couldn't be opened
        if (!files[i].is_open()) {
            if (errString != "")
                errString += ";";
            errString += " " + m_filename + ".dat" + cepToString(i + 1);
        }
    }

    if (errString != "") {
        m_ready = true;
        return
            cepError
            ("File IO error for this dataset. Could not open the file(s):"
             + errString + ".");
    }
    cepDebugPrint("All dataset files exist");


    // Read the file
    for (int i = 0; i < 3; i++) {
        if (m_progress)
            m_progress(i + 1, 0);


        // this will hold all of the data read from file
        vector < cepVector4D > data;
        data.push_back(cepVector4D());

        c = 0;
        prevc = '\n';
        thisLine = "";
        numLines = 1;

        lastRowdate = -1;
        lastRowsample = -1;
        lastRowerror = -1;
        lastRowcolor = -1;

        cepDebugPrint("File read initialization complete");

        while (!files[i].eof()) {
            files[i].read(&c, 1);

            // Squelch repeated whitespace
            if (cepIsBlank(c) == true) {
                if (cepIsBlank(prevc) != true) {
                    thisLine += " ";
                }
            } else {
                thisLine += c;
            }

            // End of line?                                    colColorHint
            // have we read a full line?
            if ((c == '\n') && (thisLine.size() > 0 )) {
//                cout << thisLine << endl;
                cepDebugPrint("Processing line: '" + thisLine + "'");

                // If this is a data bearing line
                if (cepIsNumeric(thisLine.c_str()[0])) {
                    if (m_progress)
                        m_progress(i + 1, numLines);
                    numLines++;

                    // Break the line into it's columns, I prefer this to
                    // the strtok method we used to use...
                    cepStringArray sa(thisLine, " ");
                    rowdate = atof(sa[0].c_str());
                    rowsample =
                        atof(applyOffset((direction) i, sa[1]).c_str()) -
                        m_offsetFloat[i];
                    rowerror = atof(sa[2].c_str());
                    // if we have a color colum, read it in
                    rowcolor =
                        ((sa.size() > 3) ? atof(sa[2].c_str()) : 0.0);

                    // if we have a row containing 3 consecutive delimiters then we add a new window
                    if (rowdate == delim || rowsample == delim || rowerror == delim) {
//                        cout << "("<<rowdate<<","<<rowsample<<","<<rowerror<<","<<rowcolor<<") "
//                             << "found delimiters on line " << numLines << endl;
                        data.push_back(cepVector4D());

                        lastRowdate = rowdate;
                        lastRowsample = rowsample;
                        lastRowerror = rowerror;
                        lastRowcolor = rowcolor;
                        thisLine="";
                        continue;
                    }

                    if (lastRowdate == -1) {
                        data[ (int)data.size() - 1 ].push_back(rowdate,
                                                                   rowsample,
                                                                   rowerror,
                                                                   rowcolor);

                        lastRowdate = rowdate;
                        lastRowsample = rowsample;
                        lastRowerror = rowerror;
                        lastRowcolor = rowcolor;
                    } else {
                        if (rowdate < lastRowdate && lastRowdate != delim) {
                            m_ready = true;
                            cepDebugPrint
                                ("dataset not in date order");
                            cepDebugPrint
                                ("last date="+cepToString(lastRowdate));
                            cepDebugPrint
                                ("this date="+cepToString(rowdate));
                            return cepError("dataset: " + m_filename +
                                            ".dat" + cepToString(i + 1) +
                                            " is not in date order! At line "
                                            + cepToString(numLines),
                                            cepError::sevErrorRecoverable);
                        } else {
                            if (rowdate == lastRowdate) {
                                m_ready = true;
                                return cepError("dataset: " + m_filename +
                                                ".dat" + cepToString(i +
                                                                     1) +
                                                " contains repeated values at line "
                                                + cepToString(numLines),
                                                cepError::
                                                sevErrorRecoverable);
                            } else {
                                // if we have a row containing 3 consecutive delimiters then we add a new window
                                if (rowdate == delim && rowsample == delim
                                    && rowerror == delim) {
                                    data.push_back(cepVector4D());
                                } else {
                                    try {
                                        data[ data.size()-1 ].push_back(rowdate,
                                                                        rowsample,
                                                                        rowerror,
                                                                        rowcolor);
                                    }
                                    catch(...) {
                                        cepDebugPrint
                                            ("exceeded bounds of data vector");
                                        return cepError
                                            ("exceeded bounds of data vector @ __FILE__, __LINE__");
                                    }

                                    lastRowdate = rowdate;
                                    lastRowsample = rowsample;
                                    lastRowerror = rowerror;
                                    lastRowcolor = rowcolor;
                                }
                            }
                        }
                    }
                } else {
                    // This is a header / textual line -- perhaps it's
                    // even an offset line
                     if (numLines == 1) {
                        cepStringArray sa(thisLine, " ");

                        // Is this an offset line?
                        if (cepIsNumeric(sa[6].c_str()[0])) {
                            m_header[i] += thisLine;
                            m_offset[i] = sa[6];
                            m_offsetFloat[i] = atof(m_offset[i].c_str());

			    if(sa[8] == "LSEqn"){
			      m_b1[i] = atof(sa[9].c_str());
			      m_b2[i] = atof(sa[10].c_str());
			      m_haveLs[i] = true;
			    }
                        }
                        // Well, then it must be a processing statement
                        // line
                        else if (i == 0) {
			  if(thisLine.substr(0, 2) == "FD"){
			    m_frequencyData = true;
			    cepDebugPrint("Dataset is in the freqency domain");
			  }

			  m_procHistory = thisLine;
                        }
                    }
                }
                thisLine = "";
            }
            prevc = c;
        }

        // Copy the vectors into the matrix
        if (data.size() == 1) {
            int elements = data[0].size();
            m_data[i] = new cepMatrix < double >(elements, 4);
            for (int vcount = 0; vcount < elements; vcount++) {
                m_data[i]->setValue(vcount, colDate, data[0].Xat(vcount));
                if (m_data[i]->getError().isReal())
                    return m_data[i]->getError();

                m_data[i]->setValue(vcount, colSample, data[0].Yat(vcount));
                if (m_data[i]->getError().isReal())
                    return m_data[i]->getError();

                m_data[i]->setValue(vcount, colError, data[0].Zat(vcount));
                if (m_data[i]->getError().isReal())
                    return m_data[i]->getError();

                m_data[i]->setValue(vcount, colColourHint, data[0].Cat(vcount));
                if (m_data[i]->getError().isReal())
                    return m_data[i]->getError();
            }
        } else {
            m_data[i] = new cepMatrix < double >(data[0].size(), 4, data.size());
            // for each window
            for( int wcount = 0; wcount<(int)data.size(); ++wcount ) {
                // for each window element
                for( int vcount = 0; vcount<(int)data[0].size(); ++vcount ) {
                    m_data[i]->setValue(vcount, colDate, wcount, data[wcount].Xat(vcount));
                    if (m_data[i]->getError().isReal())
                        return m_data[i]->getError();
                        
                    m_data[i]->setValue(vcount, colSample, wcount, data[wcount].Yat(vcount));
                    if (m_data[i]->getError().isReal())
                        return m_data[i]->getError();
                        
                    m_data[i]->setValue(vcount, colError, wcount, data[wcount].Zat(vcount));
                    if (m_data[i]->getError().isReal())
                        return m_data[i]->getError();
                        
                    m_data[i]->setValue(vcount, colColourHint, wcount, data[wcount].Cat(vcount));
                    if (m_data[i]->getError().isReal())
                        return m_data[i]->getError();
                }
            }
        }

        files[i].close();
    }

    // Are the files over the same period?
    // todo: check other tables
    cepDebugPrint("Check the value ranges in the dataset");
    for(int tno = 0; tno < 1 /*m_data[0]->getNumTables()*/; tno++){
      cepDebugPrint("Integrity check on table " + cepToString(tno) + " of " + 
		    cepToString(m_data[0]->getNumTables()));

      // Do the start values for the table match?
      if(m_data[0]->getValue(0, 0, tno) != m_data[1]->getValue(0, 0, tno)){
	if (m_data[0]->getError().isReal())
	  return m_data[0]->getError();
	if (m_data[1]->getError().isReal())
	  return m_data[1]->getError();
	if (m_data[2]->getError().isReal())
	  return m_data[2]->getError();
	
	return cepError("The start date for the North (" + cepToString(m_data[0]->getValue(0, 0, tno)) + 
			") and East (" + cepToString(m_data[1]->getValue(0, 0, tno)) + 
			") directions differ for table " + cepToString(tno), 
			cepError::sevErrorRecoverable);
      }
      if(m_data[0]->getValue(0, 0, tno) != m_data[2]->getValue(0, 0, tno)){
	if (m_data[0]->getError().isReal())
	  return m_data[0]->getError();
	if (m_data[1]->getError().isReal())
	  return m_data[1]->getError();
	if (m_data[2]->getError().isReal())
	  return m_data[2]->getError();
	
	return cepError("The start date for the North (" + cepToString(m_data[0]->getValue(0, 0, tno)) + 
			") and Up (" + cepToString(m_data[2]->getValue(0, 0, tno)) + 
			") directions differ for table " + cepToString(tno), 
			cepError::sevErrorRecoverable);
      }

      // What about the end values?
      if(m_data[0]->getValue(m_data[0]->getNumRows() - 1, 0, tno) != 
	 m_data[1]->getValue(m_data[1]->getNumRows() - 1, 0, tno)){
	if (m_data[0]->getError().isReal())
	  return m_data[0]->getError();
	if (m_data[1]->getError().isReal())
	  return m_data[1]->getError();
	if (m_data[2]->getError().isReal())
	  return m_data[2]->getError();
	
	return cepError("The final date for the North (" + 
			cepToString(m_data[0]->getValue(m_data[0]->getNumRows() - 1, 0, tno)) + 
			") and East (" + 
			cepToString(m_data[1]->getValue(m_data[1]->getNumRows() - 1, 0, tno)) + 
			") directions differ for table " + cepToString(tno), 
			cepError::sevErrorRecoverable);
      }
      if(m_data[0]->getValue(m_data[0]->getNumRows() - 1, 0, tno) != 
	 m_data[2]->getValue(m_data[2]->getNumRows() - 1, 0, tno)){
	if (m_data[0]->getError().isReal())
	  return m_data[0]->getError();
	if (m_data[1]->getError().isReal())
	  return m_data[1]->getError();
	if (m_data[2]->getError().isReal())
	  return m_data[2]->getError();

	return cepError("The final date for the North (" + 
			cepToString(m_data[0]->getValue(m_data[0]->getNumRows() - 1, 0, tno)) + 
			") and Up (" + 
			cepToString(m_data[2]->getValue(m_data[2]->getNumRows() - 1, 0, tno)) + 
			") directions differ for table " + cepToString(tno), 
			cepError::sevErrorRecoverable);
      }
    }
      
    if (m_data[0]->getError().isReal())
      return m_data[0]->getError();
    if (m_data[1]->getError().isReal())
      return m_data[1]->getError();
    if (m_data[2]->getError().isReal())
      return m_data[2]->getError();

    m_ready = true;
    m_wellformed = true;
    return cepError();
}

cepError cepDataset::write(const string & filename)
{
    m_filename = filename;

    // Step One: ensure that all of the files we need are actually there,
    // it would be bad to read two of the three files, and _then_ report
    // an error...
    fstream files[3];

    files[0].open(string(m_filename + ".dat1").c_str(), ios::out);
    files[1].open(string(m_filename + ".dat2").c_str(), ios::out);
    files[2].open(string(m_filename + ".dat3").c_str(), ios::out);
    cepDebugPrint("Opened the dataset files");

    // Check they opened ok
    string errString;

    for (int i = 0; i < 3; i++) {
        // File is NULL if it couldn't be opened
        if (!files[i].is_open()) {
            if (errString != "")
                errString += ";";
            errString += " " + m_filename + ".dat" + cepToString(i + 1);
        }
    }

    if (errString != "") {
        m_ready = true;
        return
            cepError
            ("File IO error for this dataset. Could not open the file(s):"
             + errString + ".");
    }
    cepDebugPrint("All dataset files exist");

    for (int i = 0; i < 3; i++) {
        if(m_frequencyData && (m_procHistory.substr(0, 2) != "FD"))
	  files[i] << "FD ";
	
        files[i] << m_procHistory << endl;
        files[i] << m_header[i];
      
	if(m_haveLs[i]){
	  cepDebugPrint("Saving the LS line of best fit");
	  files[i] << " LSEqn " << m_b1[i] << " " << m_b2[i];
	}
	files[i] << endl << endl;

        cepDebugPrint("Writing dataset with offset of " + m_offset[i] +
                      " in direction " + cepToString(i));

        // Iterate through windows
        for (int wcount = 0; wcount < m_data[i]->getNumTables(); wcount++ ) {
            // iterate through elements of each window
            if( wcount > 0 && wcount < m_data[i]->getNumTables() ) {
                delimitWindow(files[i]);
            }
            for (int vcount = 0; vcount < m_data[i]->getNumRows(); vcount++) {
                ostringstream line;
		line << " " << cepToString(m_data[i]->getValue(vcount, colDate, wcount));
                if (m_data[i]->getError().isReal())
                    return m_data[i]->getError();

		line << " " << reverseOffset((direction) i,
					     cepToString(m_data[i]->getValue(vcount,colSample, 
									     wcount)+
							 m_offsetFloat[i]));
                if (m_data[i]->getError().isReal())
                    return m_data[i]->getError();

		line << " " << cepToString(m_data[i]->getValue(vcount, colError, wcount));
                if (m_data[i]->getError().isReal())
                    return m_data[i]->getError();

                // if we have more than 3 fields, write real data, otherwise write 0
                if( m_data[i]->getNumCols() > 3 ) {
		  line << " " << cepToString(m_data[i]->getValue(vcount, colColourHint, wcount));
                } else {
		  line << " " << 0.0;
                }
                
                if (m_data[i]->getError().isReal())
                    return m_data[i]->getError();
                    
                files[i] << line.str() << endl;
            }
        }
//        cout << endl;

        files[i].close();
    }

    return cepError();
}

void cepDataset::delimitWindow( fstream &f ) {
    f << " " << delim << "  " << delim << "  " << delim << "  " << delim << endl;
}

cepMatrix < double >*cepDataset::getMatrix(direction dir)
{
    return m_data[dir];
}

bool cepDataset::isReady()
{
    return m_ready;
}

bool cepDataset::isWellFormed()
{
    return m_wellformed;
}

cepDataset::direction cepDataset::getDirectionFromName(string name)
{
    string lname = cepToLower(name);

    cepDebugPrint("Getting direction: " + lname);
    if (name == "x")
        return dirX;
    if (name == "y")
        return dirY;
    if (name == "z")
        return dirZ;

    cepError
        unknown("Unknown direction " + name + " requested of the dataset",
                cepError::sevErrorRecoverable);
    unknown.display();
    return dirX;
}

// The datasets have an offset which we need to apply. For instance:
// m_offset = 425634.1235
// value = 1.1234
// 
// should result in 425631.1234
string cepDataset::applyOffset(cepDataset::direction i, string value)
{
    // Remove the sign from the value
    string newvalue;
    if (value.substr(0, 1) == "-")
        newvalue = value.substr(1, value.length() - 1);
    else
        newvalue = value;

    // Find the decimal points
    cepStringArray offset(m_offset[i], ".");
    cepStringArray val(newvalue, ".");
    string retval;

    retval = offset[0].substr(0, offset[0].length() - val[0].length());
    retval += newvalue;

    cepDebugPrint("Offset calculation: offset = " + m_offset[i] +
                  " value = " + newvalue + " resultant value = " + retval);
    return retval;
}

string cepDataset::reverseOffset(cepDataset::direction i, string value)
{
    // Remove the sign from the value
    string sign("");
    if (value.substr(0, 1) == "-")
        sign = "-";

    int count = 1;
    while (value.substr(0, count) == m_offset[i].substr(0, count)) {
        if (value.substr(count, 1) == ".") {
            break;
        }

        count++;
    }

    cepDebugPrint("Deoffset calculation offset = " + m_offset[i] +
                  " value = " + value + " indent count = " +
                  cepToString(count + 1) + " resultant value = " + sign +
                  value.substr(count - 1, value.length()));
    return string(sign + value.substr(count - 1, value.length()));
}

// Return the root filename for the dataset
string cepDataset::getRootFilename()
{
    return m_filename;
}

// Return a filtered dataset
cepDataset cepDataset::filter(float low, float high)
{
    vector < double >dates;
    vector < double >samples;
    vector < double >errors;
    cepMatrix < double >*data[dirUnknown];

    for (int dir = 0; dir < dirUnknown; dir++) {
        dates.clear();
        samples.clear();
        errors.clear();

        for (int i = 0; i < m_data[dir]->getNumRows(); i++) {
            if ((m_data[dir]->getValue(i, colDate) >= low) &&
                (m_data[dir]->getValue(i, colDate) <= high)) {
                dates.push_back(m_data[dir]->getValue(i, colDate));
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }

                samples.push_back(m_data[dir]->getValue(i, colSample));
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }

                errors.push_back(m_data[dir]->getValue(i, colError));
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }
            }
        }

        if (dates.size() > 0) {
            // Copy the vectors into the matrix
            data[dir] = new cepMatrix < double >(dates.size(), 3);
            for (unsigned int vcount = 0; vcount < dates.size(); vcount++) {
                data[dir]->setValue(vcount, colDate, dates[vcount]);
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }

                data[dir]->setValue(vcount, colSample, samples[vcount]);
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }

                data[dir]->setValue(vcount, colError, errors[vcount]);
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }
            }
        } else {
            return cepDataset();
        }
    }

    return cepDataset(data[0], data[1], data[2], m_offset[0], m_offset[1],
                      m_offset[2], m_procHistory + ": Zoomed", m_header[0],
                      m_header[1], m_header[2]);
}

// Return a dataset with some values replaced
cepDataset cepDataset::replace(float low, float high, float sample)
{
    vector < double >dates;
    vector < double >samples;
    vector < double >errors;
    cepMatrix < double >*data[dirUnknown];

    for (int dir = 0; dir < dirUnknown; dir++) {
        dates.clear();
        samples.clear();
        errors.clear();

        for (int i = 0; i < m_data[dir]->getNumRows(); i++) {
            if ((m_data[dir]->getValue(i, colDate) >= low) &&
                (m_data[dir]->getValue(i, colDate) <= high)) {
                dates.push_back(m_data[dir]->getValue(i, colDate));
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }

                samples.push_back(sample);
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }

                errors.push_back(m_data[dir]->getValue(i, colError));
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }
            }
	    else{
	      dates.push_back(m_data[dir]->getValue(i, colDate));
	      if (m_data[dir]->getError().isReal()) {
		m_data[dir]->getError().display();
		return cepDataset();
	      }
	      
	      samples.push_back(m_data[dir]->getValue(i, colSample));
	      if (m_data[dir]->getError().isReal()) {
		m_data[dir]->getError().display();
		return cepDataset();
	      }
	      
	      errors.push_back(m_data[dir]->getValue(i, colError));
	      if (m_data[dir]->getError().isReal()) {
		m_data[dir]->getError().display();
		return cepDataset();
	      }
	    }
        }
	
        if (dates.size() > 0) {
            // Copy the vectors into the matrix
            data[dir] = new cepMatrix < double >(dates.size(), 3);
            for (unsigned int vcount = 0; vcount < dates.size(); vcount++) {
                data[dir]->setValue(vcount, colDate, dates[vcount]);
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }

                data[dir]->setValue(vcount, colSample, samples[vcount]);
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }

                data[dir]->setValue(vcount, colError, errors[vcount]);
                if (m_data[dir]->getError().isReal()) {
                    m_data[dir]->getError().display();
                    return cepDataset();
                }
            }
        } else {
            return cepDataset();
        }
    }

    return cepDataset(data[0], data[1], data[2], m_offset[0], m_offset[1],
                      m_offset[2], m_procHistory + ": Editted", m_header[0],
                      m_header[1], m_header[2]);
}

// The display name of the dataset
string cepDataset::getName()
{
    cepStringArray name(m_filename, "/~");
    return name[name.size() - 1];
}

// The processing history for this dataset
string cepDataset::getProcHistory()
{
    return m_procHistory;
}

// Return the offset applied to this dataset
string cepDataset::getOffset(direction i)
{
    return m_offset[i];
}

// Return the header line for a given direction
string cepDataset::getHeader(direction i)
{
    return m_header[i];
}

// Get the B1 LS line coefficient for a given direction
double cepDataset::getB1(direction i)
{
    return m_b1[i];
}

// Get the B2 LS line coefficient for a given direction
double cepDataset::getB2(direction i)
{
    return m_b2[i];
}

// Do we have a LS line?
bool cepDataset::getHaveLs(direction i)
{
    return m_haveLs[i];
}

// Is this data in the frequency domain?
bool cepDataset::isFreqDomain()
{
  return m_frequencyData;
}

// Set this data as being in the frequency domain
void cepDataset::setFreqDomain(bool isFreq){
  m_frequencyData = isFreq;
}

const double cepDataset::delim = -255.0;




cepVector4D::cepVector4D():
x(), y(), z(), c()
{
}
void cepVector4D::push_back(double xval, double yval, double zval,
                            double cval)
{
    x.push_back(xval);
    y.push_back(yval);
    z.push_back(zval);
    c.push_back(cval);

}
double cepVector4D::Xat(int &i)
{
    return x[i];
}
double cepVector4D::Yat(int &i)
{
    return y[i];
}
double cepVector4D::Zat(int &i)
{
    return z[i];
}
double cepVector4D::Cat(int &i)
{
    return c[i];
}

int cepVector4D::size()
{
    // all 3 directions are the same size
    return x.size();
}
