
/* 
 *   The header we include everywhere to get access to interesting classes
 *   Copyright (C) Michael Still                    2002
 *   Copyright (C) Daniel Fernandez                 2002
 *   
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *   
 *   This program is distibuted in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef CEP_CORE_HEADER
#define CEP_CORE_HEADER

#ifdef GLOBALS_HERE
#define GLOBAL_THINGIE
#undef GLOBALS_HERE
#else
#define GLOBAL_THINGIE extern
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include "cepTSB.h"
#include "cepError.h"
#include "cepUtility.h"
#include "cepDataset.h"
#include "cepConfiguration.h"

GLOBAL_THINGIE fstream gLog;
GLOBAL_THINGIE cepTSB gDisplayParams[cepError::sevMax];

#endif
