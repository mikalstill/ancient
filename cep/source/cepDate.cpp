/* 
 *   Imp for the CEP date
 *
 *   Copyright (C) Daniel Fernandez                 2002
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Blake Swadling                   2002
 *   Copyright (C) Kristy Van Der Vlist             2002
 *   Copyright (C) Nick Wheatstone                  2002
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 675
 * Mass Ave, Cambridge, MA 02139, USA.
 */

#include "cepDate.h"
#include <iomanip>

cepDate::cepDate (const double &date)
{
  int monthNums[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  int i = 0;

  m_decimalDate = date;
  m_year = m_day = m_month = 0;

  m_year = (int) date;

  // Check if leap year
  if (isLeap () == true)
    {
      monthNums[1]++;
      m_day = (int) ((date - m_year) / LEAP_DAY_VAL);
    }
  else
    {
      m_day = (int) ((date - m_year) / DAY_VAL);
    }

  //to accont for the fact that 1 Jan is day 0
  m_day++;

  // Get the day of month
  while ((i < 11) && (m_day - monthNums[i] > 0))
    {
      m_day -= monthNums[i];
      i++;
    }


  m_month = i;
}

cepDate::cepDate (const int &day, string month, const int &year)
{
  int monthNums[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  double denom = 0.0;

  m_year = year;
  m_day = day - 1;
  m_month = -1;

  if (isLeap () == true)
    {
      monthNums[1]++;
    }

  if (month.length () > 3)
    {
      for (int i = 0; i < 12; i++)
	{
	  if (month == MONTH_NAMES[i])
	    {
	      m_month = i;
	    }
	}
      if (m_month == -1)
	{
	  cepError ("Month Name: " + month + " is an invalid month name ",
		    cepError::sevWarning);
	}
    }
  else
    {
      for (int i = 0; i < 12; i++)
	{
	  if (month == SHORTMONTH_NAMES[i])
	    {
	      m_month = i;
	    }
	}
      if (m_month == -1)
	{
	  cepError ("Month Name: " + month + " is an invalid month name ",
		    cepError::sevWarning).display ();
	  m_month = m_day = m_year = -1;
	  m_decimalDate = -1.0;
	}
    }

  //caculate the number of days into the year as a decimal
  if (m_month != -1)
    {
      if (m_day < monthNums[m_month])
	{
	  if ((m_day > 0) || (m_month > 0))
	    {
	      for (int i = 0; i < m_month; i++)
		{
		  denom += monthNums[i];
		}

	      denom += m_day;
	      if (isLeap () == true)
		{
		  denom *= LEAP_DAY_VAL;
		  denom += (double) LEAP_DAY_VAL / 2;

		  //round to 4 dp
		  denom = ((int) (denom * 10000 + 0.5)) / 10000.0;
		  m_decimalDate = m_year + denom;
		  m_day++;
		}
	      else
		{
		  denom *= DAY_VAL;
		  denom += (double) DAY_VAL / 2;
		  //rounding to 4 dp
		  denom = ((int) (denom * 10000 + 0.5)) / 10000.0;
		  m_decimalDate = m_year + denom;
		  //add an offset to the decimal date to avoid possible rounding errors
		  m_day++;
		}



	    }
	  else
	    {
	      //special case where day is 1 of January
	      if (m_day == 0)
		{
		  m_decimalDate = (double) m_year;
		  m_decimalDate += 0.0014;
		  m_day++;
		}
	      else
		{
		  cepError ("Day Value: " + cepToString (day) +
			    " is an invalid",
			    cepError::sevWarning).display ();
		  m_month = m_day = m_year = -1;
		  m_decimalDate = -1.0;
		}
	    }
	}
      else
	{
	  cepError ("Day: " + cepToString (day) + " of " + month +
		    " does not exist", cepError::sevWarning).display ();
	  m_month = m_day = m_year = -1;
	  m_decimalDate = -1.0;
	}
    }
  else
    {
      m_year = -1;
      m_day = -1;
    }
}

const string
cepDate::getDay ()
{
  string day;

  // Zero pad if nessisary
  if (m_day < 10)
    {
      day = "0";
      day += cepToString (m_day);
    }
  else
    {
      day = cepToString (m_day);
    }
  return day;
}

const string
cepDate::getMonth ()
{
  string month;

  // Zero pad if nessisary
  if (m_month < 9)
    {
      month = (m_month < 9 ? "0" : "") + cepToString (m_month + 1);
    }
  else
    {
      month = cepToString (m_month + 1);
    }

  return month;
}

const string
cepDate::getMonthName ()
{
  return MONTH_NAMES[m_month];
}

const string
cepDate::getShortMonthName ()
{
  return SHORTMONTH_NAMES[m_month];
}

const string
cepDate::getYear ()
{
  return (cepToString (m_year));
}

const string
cepDate::getShortDate ()
{
  string day, month;

  // Zero pad if nessisary
  if (m_day < 10)
    {
      day = "0";
      day += cepToString (m_day);
    }
  else
    {
      day = cepToString (m_day);
    }

  if (m_month < 9)
    {
      month = (m_month < 9 ? "0" : "") + cepToString (m_month + 1);
    }
  else
    {
      month = cepToString (m_month + 1);
    }

  return (day + "/" + month + "/" + cepToString (m_year));
}

const string
cepDate::getLongDate ()
{
  string day, month;

  // Zero pad if nessisary
  if (m_day < 10)
    {
      day = "0";
      day += cepToString (m_day);
    }
  else
    {
      day = cepToString (m_day);
    }

  return (day + " " + (m_month < 9 ? "0" : "") + MONTH_NAMES[m_month] + " " +
	  cepToString (m_year));
}

const double
cepDate::getDecimalDate ()
{
  return m_decimalDate;
}

bool cepDate::isLeap ()
{
  if (((m_year % 100 == 0) && (m_year % 400 == 0)) ||
      ((m_year % 100 != 0) && (m_year % 4 == 0)))
    {
      return true;
    }

  return false;
}

double
cepDate::yearsToJulian (double year)
{
  const int FEBRUARY = 1;
  int i, j;
  int leap_year, iyear, ijd, syr;
  double fract, days1, days2, fract1;

  int month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  days1 = 365.0;

  leap_year = (int) year % 4;	// leap_year = year mod 4
  if (leap_year == 0)
    {
      month[FEBRUARY]++;	// increase the number of days in february
      days1 = days1 + 1.0;
    }

  // get decimal portion of year. ie 0.0027
  fract = year - (double) ((int) year);
  // get whole number of years. ie 2002
  iyear = (int) year;

  // count how many day equal the required fraction
  days2 = 0.0;
  fract1 = days2 / days1;
  // ask peter about the different end loop conditions
  for (i = 0; (i < 12) && (fract1 < fract); i++)
    {
      for (j = 0; (j < month[i]) && (fract1 < fract); j++)
	{
	  days2 = days2 + 1.0;
	  fract1 = days2 / days1;
	}
    }

  // calculate years since 1900
  syr = iyear - 1900;
  // calculate integer julian day
  ijd = syr * 365 + (syr - 1) / 4 + (int) days2;


  return (double) ijd;
}

double
cepDate::julianToYears (double julian)
{
  int iyear, leap_year;
  double fract;

  // calculate number of whole years since 1900
  iyear = (int) (julian / 365.25);
  julian = julian - (double) (iyear * 365 + iyear / 4);

  // calculate year fraction
  leap_year = iyear % 4;
  if (leap_year == 0)
    fract = (julian - 0.5) / 366.0;
  else
    fract = (julian - 0.5) / 365.0;

  return 1900.0 + (double) iyear + fract;
}
