
/* 
 *   Imp for the CEP temporary directory cleaner
 *
 *   This class owns the presentation of the data on disc... There are
 *   several presentation formats available -- pdf, tiff, gif...
 *
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "cepCore.h"

#ifndef CEP_TMPCLEAN_HEADER
#define CEP_TMPCLEAN_HEADER

class cepError;

/******************************************************************************
DOCBOOK START

FUNCTION cepTmpClean
PURPOSE cleanup temporary files created by <command>GDMS</command>

SYNOPSIS START
#include "cepTmpClean.h"

cepTmpClean::cepTmpClean(string path, string pattern);
cepError cepTmpClean::execute(int& deleted, bool doDelete);
SYNOPSIS END

DESCRIPTION START
<command>GDMS</command> creates temporary files for the various graphs that it draws. Normally these temporary files are stored in /tmp. Over only a few graph executions, it is possible to have a fairly large number of temporary files, which might cause the filesystem which /tmp is stored on to run out of disc space.</para>

<para>
This class automates the removal of these old files, and is called whenever the interactive version of <command>GDMS</command> starts up.
</para>

<para>
This class is extremely easy to use. Please refer to the example below for more details.
DESCRIPTION END

EXAMPLE START
#include "cepTmpClean.h";

cepTmpClean cleaner("/tmp", "cep*");
int deleted;
cleaner.execute(deleted, true);
EXAMPLE END
DOCBOOK END
******************************************************************************/

class cepTmpClean
{
public:
  cepTmpClean (string path, string pattern);
  cepError execute (int &deleted, bool doDelete);

private:
    string m_path, m_pattern;
  cepConfiguration *m_config;
};

#endif
