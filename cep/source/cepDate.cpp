/* 
   Imp for the CEP date
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

#include "core.h"

const string cMonthNames[12] =
  { "January", "February", "March", "April", "May",
  "June", "July", "August", "September", "October",
  "November", "December"
};

cepDate::cepDate (double decimal):
m_day (-1),
m_year (-1),
m_yearDays (-1),
m_dayOfYear (-1),
m_month (-1),
m_leap (false)
{
  m_decimal = decimal;
}

void
cepDate::popDayOfYear ()
{
  popYear ();
  popDaysInYear ();

  if (m_dayOfYear == -1)
    {
      m_dayOfYear = m_yearDays * (m_decimal - m_year);

      cepError dbg (cepDtoa (m_decimal) + " converts to " + cepItoa (m_dayOfYear) +
		    " days", cepError::sevDebug);
      dbg.display ();
    }
}

void
cepDate::popDaysInYear ()
{
  popYear ();

  if (m_yearDays == -1)
    {
      // This is Blake's fault -- he made me do it
      // todo_mikal: Change days in years
      m_yearDays = ((m_year % 100 == 0) && (m_year % 400 == 0)) ||
	((m_year % 100 != 0) && (m_year % 4 == 0)) ? 366 : 365;

      cepError dbg (cepDtoa (m_decimal) + " converts to " + cepItoa (m_yearDays) +
		    " days in the year", cepError::sevDebug);
      dbg.display ();

      if (m_yearDays == 366)
	m_leap = true;
    }
}

void
cepDate::popYear ()
{
  if (m_year == -1)
    {
      m_year = (int) m_decimal;

      cepError dbg (cepDtoa (m_decimal) + " converts to " + cepItoa (m_year) +
		    " year", cepError::sevDebug);
      dbg.display ();
    }
}

void
cepDate::popMonthAndDay ()
{
  popDayOfYear ();
  popYear ();
  popDaysInYear ();

  if (m_day == -1)
    {
      int dayOfYear = m_dayOfYear;
      int i = 0;

      int months[12] = { 31, m_leap ? 29 : 28, 31, 31, 31, 31, 31, 31, 31, 31,
	31, 31
      };

      while ((i < 11) && (dayOfYear - months[i] > 0))
	{
	  dayOfYear -= months[i];
	  i++;
	}
      m_day = dayOfYear;
      m_month = i;

      {
	cepError dbg (cepDtoa (m_decimal) + " converts to " + cepItoa (m_day) +
		      " " + cMonthNames[m_month], cepError::sevDebug);
	dbg.display ();
      }
    }
}

string cepDate::toString ()
{
  popMonthAndDay ();
  popYear ();

  return string (cepItoa (m_day) + " " + cMonthNames[m_month] + " " +
		 cepItoa (m_year));
}
