
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

cepTmpClean::cepTmpClean(string path, string pattern):
  m_path(path), m_pattern(pattern)
{
}

cepError cepTmpClean::execute(int& deleted, bool doDelete)
{
  // Go through the contents of the named directory, and delete files
  // which match the pattern
  
  // Find directory
  struct stat sb;
  if(lstat(m_path.c_str(), &sb) < 0)
    return cepError("stat() error for temporary path \"" + path + "\"",
		    cepError::sevErrrorRecoverable);

  if(S_ISDIR(statbuf.st_mode) == 0)
    return cepError("Temporary path does not point to a directory: \"" + 
	            path + "\"",
  	            cepError::sevErrorRecoverable);

  // Open the directory
  DIR *dp;
  struct dirent *dirp;
  
  if((dp = opendir(path.c_str()) == NULL)
    return cepError("Temporary path \"" + path + "\" is not a directory. This is possibly a permissions error.",
	    	    cepError::sebvErrorRecoverable);

  while((dirp = readdir(dp))) != NULL){
    // We skip the . and .. entries
    if((strcmp(dirp->name, ".") == 0) ||
       (strcmp(dirp->name, "..") == 0)) continue;

    // This is a posisble deletion candidate
    cepDebugPrint("Possible deletion candidate: " + path + "/" + string(dirp->name));
  }

  // Cleanup
  if(closedir(dp) < 0)
    cepError("Could not close the directory pointer", cepError::sevErrorRecoverable);
}
