
/* 
 *   Imp for the CEP temporary directory cleaner
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

#include "cepTmpClean.h"

#ifdef HAVE_PCRE
#include <pcre.h>
#endif

#define OVECCOUNT 30

cepTmpClean::cepTmpClean(string path, string pattern):
  m_path(path), m_pattern(pattern)
{
}

cepError cepTmpClean::execute(int& deleted, bool doDelete)
{
  // Go through the contents of the named directory, and delete files
  // which match the pattern
  
#ifdef HAVE_PCRE
  // Find directory
  struct stat sb;
  if(lstat(m_path.c_str(), &sb) < 0)
    return cepError("stat() error for temporary path \"" + m_path + "\"",
		    cepError::sevErrorRecoverable);
  
  if(S_ISDIR(sb.st_mode) == 0)
    return cepError("Temporary path does not point to a directory: \"" + 
	            m_path + "\"", cepError::sevErrorRecoverable);

  // Open the directory
  DIR *dp;
  struct dirent *dirp;
  
  if((dp = opendir(m_path.c_str())) == NULL)
    return cepError("Temporary path \"" + m_path + 
		    "\" is not a directory. This is possibly a permissions error.",
	    	    cepError::sevErrorRecoverable);

  // Prepare the regular expression
  pcre *re;
  const char *error;
  int erroffset;
  int ovector[OVECCOUNT];

  if((re = pcre_compile("cep.*$", 0, &error, &erroffset, NULL)) == NULL){
    return cepError("Could not compile the tmp cleanup regular expression: " + error,
		    cepError::sevErrorRecoverable);
  }

  while((dirp = readdir(dp)) != NULL){
    // We skip the . and .. entries
    if((strcmp(dirp->d_name, ".") == 0) ||
       (strcmp(dirp->d_name, "..") == 0)) continue;

    // Did the RE match?
    if(pcre_exec(re, NULL, dirp->d_name, (int) strlen(dirp->d_name),
		 0, 0, ovector, OVECCOUNT) > 0){
      // This is a posisble deletion candidate
      cepDebugPrint("Possible deletion candidate: " + m_path + "/" + 
		    string(dirp->d_name));
    }
  }

  // Cleanup
  if(closedir(dp) < 0)
    cepError("Could not close the directory pointer", 
	     cepError::sevErrorRecoverable);

  return cepError();
#else
  return cepError("PCRE was not found at compile time.", cepError::sevErrorRecoverable);
#endif
}
