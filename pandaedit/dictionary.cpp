// This is the implementation of the PDF object model

#include "objectmodel.h"
#include <stdio.h>

void dictionary::add(dictitem item)
{
  m_items.push_back(item);
  printf("DEBUG: Added a dictionary item\n");
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

bool dictionary::getValue(string dname, pdf& thePDF, object& obj)
{
  for(unsigned int i = 0; i < m_items.size(); i++)
    if(m_items[i].isNamed(dname)){
      return thePDF.findObject(m_items[i].getIntValue(), m_items[i].getGeneration(), obj);
    }

  return false;
}

bool dictionary::getValue(string dname, string& value)
{
  for(unsigned int i = 0; i < m_items.size(); i++)
    if(m_items[i].isNamed(dname)){
      value = m_items[i].getStringValue();
      return true;
    }

  return false;
}
