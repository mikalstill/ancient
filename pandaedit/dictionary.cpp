// This is the implementation of the PDF object model

#include "objectmodel.h"

void dictionary::add(dictitem item)
{
  m_items.push_back(item);
}

unsigned int dictionary::size()
{
  return m_items.size();
}

dictitem dictionary::operator[](unsigned int index)
{
  return m_items[index];
}

bool dictionary::findItem(string dname, dictitem& item)
{
  for(unsigned int i = 0; i < m_items.size(); i++)
    if(m_items[i].isNamed(dname)){
      item = m_items[i];
      return true;
    }

  return false;
}
