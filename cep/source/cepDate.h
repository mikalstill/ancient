
/* 
 *   Imp for the CEP date
 *   Copyright (C) Michael Still                    2002
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

#ifndef CEP_DATE_HEADER
#define CEP_DATE_HEADER

#include <string>
#include "cepUtility.h"
#include "cepError.h"

/******************************************************************************
DOCBOOK START

FUNCTION cepDate


PURPOSE Converts a decimal date in the format of YYYY.DDDD, in which 1 day is
equal to 0.0027 to a standard Gregorian date.

SYNOPSIS START
The follwing is an example of how to create a cepDate object.

cepDate date(value);

where value is the decimal date to be converted.

SYNOPSIS END

DESCRIPTION START
An implementation of the <command>cepDate</command> class
converts a decimal date to a Gegorian date.
</para>

<para>
<command>const string getDay()</command>
returns day of month in the format of 01...31
</para>

<para>
<command>const string getMonth()</command>
returns month in the format of 01...12
</para>

<para>
<command>const string getMonthName()</command>
returns month as a string in the format of January...December
</para>

<para>
<command>const string getYear()</command>
returns year in the format of YYYY
</para>

<para>
<command>const string getShortDate()</command>
returns date in short date in the format of DD/MM/YY
</para>

<para>
<command>const string getLongDate()</command>
returns date in long date date in the format of DD MMMMMM YY

DESCRIPTION END

DOCBOOK END
******************************************************************************/

const string MONTH_NAMES[12] = { "January", "February", "March", "April", "May",
				 "June", "July", "August", "September", "October",
				 "November", "December"
};

const string SHORTMONTH_NAMES[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
				      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

class cepError;

class cepDate
{
public:
  cepDate (double decimal);

  // Returns day of month 01...31
  const string getDay();

  // Returns month 01...12
  const string getMonth();

  // Returns month as a string January...December
  const string getMonthName();

  // Returns month as a string Jan...Dec
  const string getShortMonthName();

  // Returns year in format YYYY
  const string getYear();

  // Returns date in short date format DD/MM/YY
  const string getShortDate();

  // Returns date in long date format DD MMMMMM YY
  const string getLongDate();

private:
  int m_month,  
    m_day,
    m_year;
  double m_dayVal;
};

#endif
