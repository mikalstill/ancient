
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

#include "cepDate.h"

cepDate::cepDate (double date)
{
  int monthNums[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int i = 0;
  
  m_year = m_day = m_month = 0;;
  
  m_year = (int)date;

  //check if leap year
  if(((m_year % 100 == 0) && (m_year % 400 == 0)) ||
    ((m_year % 100 != 0) && (m_year % 4 == 0)))
  {
    monthNums[1] ++;
  }

  //get the day of Year number (0-365 or 366)
  m_day = (int)((float)((date - m_year)/DAY_VAL));

  //get the day of month
  while ((i < 11) && (m_day - monthNums[i] > 0))
  {
    m_day -= monthNums[i];
    i++;
  }
  m_month = i;
}

const string cepDate::getDay()
{
  string day;

  //zero pad if nessisary
  if( m_day < 10)
  {
    day = "0";
    day += cepToString(m_day);
  }
  else
  {
    day = cepToString(m_day);
  }
  return day;
}

const string cepDate::getMonth()
{
  string month;

  //zero pad if nessisary
  if( m_month < 10)
  {
    month = "0";
    month += cepToString(m_month+1);
  }
  else
  {
    month = cepToString(m_month+1);
  }
  
  return month;
}

const string cepDate::getMonthName()
{
  return MONTH_NAMES[m_month];
}

const string cepDate::getYear()
{
  return (cepToString(m_year));
}

const string cepDate::getShortDate()
{
  string day, month;

  //zero pad if nessisary
  if( m_day < 10)
  {
    day = "0";
    day += cepToString(m_day);
  }
  else
  {
    day = cepToString(m_day);
  }

  if( m_month < 10)
  {
    month = "0";
    month += cepToString(m_month+1);
  }
  else
  {
    month = cepToString(m_month+1);
  }

  return (day + "/" + month + "/" + cepToString(m_year));
}
const string cepDate::getLongDate()
{
  string day, month;

  //zero pad if nessisary
  if( m_day < 10)
  {
    day = "0";
    day += cepToString(m_day);
  }
  else
  {
    day = cepToString(m_day);
  }

  return(day + " " + MONTH_NAMES[m_month] + " " + cepToString(m_year));
}
