
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


PURPOSE Converts a decimal date in the format of YYYY.DDDD, where YYYY is a four
digit year and DDDD is the number of days into that year as a decimal
to a standard Gregorian date. <command>cepDate<command> will also convert a date
in the form of DD MMMMMM YYYY to a decimal date, where DD is the day of the month -
1..31 MMMMMM is the name of the month - either Jan...Dec or January...December and
YYYY is a four Digit year. 

SYNOPSIS START
To create a cepDate object to convert a decimal date to a Gregorian date use:-

cepDate date(value);

<para><itemizedlist>
  <listitem><para>value:- The decimal value to be converted. It must be of the form
    YYYY.DDDD where YYYY is the four digit year and DDDD is the decimal represenation
    of the number of days into the year</para></listitem>
</itemizedlist></para>

To create a cepDate object to convert a Gregorian date to a decimal date use:-

cepDate date(day, month, year);

<para><itemizedlist>
  <listitem><para>day:- interger value of the day of the month from 1..31</para></listitem>
  <listitem><para>month:- the name of the month. This can be either Jan...Dec or
    January...December</para></listitem>
  <listitem><para>year:- interger value of the year</para></listitem>
</itemizedlist></para>

</para>


SYNOPSIS END

DESCRIPTION START
An implementation of the <command>cepDate</command> class
converts a decimal date to a Gegorian date or a Gregorian date to a decimal.
</para>

<para>
<command>const string getDay()</command>
returns day of month in the format of 01...31. A return value of -1 indicates
an error because the date entered was invalid.
</para>

<para>
<command>const string getMonth()</command>
returns month in the format of 01...12. A return value of -1 indicates
an error because the date entered was invalid.
</para>

<para>
<command>const string getMonthName()</command>
returns month as a string in the format of January...December
</para>

<para>
<command>const string getShortMonthName()</command>
returns month as a string in the format of Jan...Dec
</para>

<para>
<command>const string getYear()</command>
returns year in the format of YYYY. A return value of -1 indicates
an error because the date entered was invalid.
</para>

<para>
<command>const string getShortDate()</command>
returns date in short date in the format of DD/MM/YY
</para>

<para>
<command>const string getLongDate()</command>
returns date in long date date in the format of DD MMMMMM YY
</para>

<para>
<command>const string getDecimalDate()</command>
returns the date as a decimal date in the form YYYY.DDDD. A return value of -1 indicates
an error because the date entered was invalid.
</para>

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

const double LEAP_DAY_VAL = 0.002732240437;
const double DAY_VAL = 0.002739726027;
const double OFFSET = 0.0014;

class cepError;

class cepDate
{
public:

  cepDate (const double &decimal);

  cepDate (const int &day, string month, const int &year);
  
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

  // Returns a decimal date
  const double getDecimalDate();
private:
  int m_month,  
    m_day,
    m_year;
   double m_decimalDate;

  bool isLeap();
};

#endif
