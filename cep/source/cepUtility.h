
/* 
 *   Utility methods for CEP
 *   Copyright (C) Michael Still                    2002
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

#ifndef CEP_UTILITY_HEADER
#define CEP_UTILITY_HEADER
#include <string>
using namespace std;

// String methods
string cepToString (int number);
string cepToString (long number);
string cepToString (double number);
string cepToString (float number);
string cepToString (bool val);
string cepToString (char val);
string cepToString (size_t val);

string cepToLower (string in);
string cepToUpper (string in);

// Evaluation methods
bool cepIsBlank (char chr);
bool cepIsNumeric(char chr);

// Mathematical methods
int cepMax (int a, int b);
int cepMin (int a, int b);
int cepAbs (int a);

#endif
