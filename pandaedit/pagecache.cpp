#include "pagecache.h"

string& pageCache::operator[](int pageno)
{
  // Make sure there are enough pages
  if(pageno + 1 > m_filenames.size())
    {
      while(pageno + 1 > m_filenames.size())
	m_filenames.push_back("");
    }

  return m_filenames[pageno];
}
