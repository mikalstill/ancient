
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
string genToString (int number);
string genToString (long number);
string genToString (double number);
string genToString (float number, bool suppressTailingZeros = false);
string genToString (bool val);
string genToString (char val);
string genToString (size_t val);

string genToLower (string in);
string genToUpper (string in);

// Evaluation methods
bool genIsBlank (char chr);
bool genIsNumeric (char chr);

// Mathematical methods
int genMax (int a, int b);
int genMin (int a, int b);
int genAbs (int a);

#endif
