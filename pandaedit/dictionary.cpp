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
  printf("DEBUG: Finding %s in a %d item dictionary\n", 
	 dname.c_str(), m_items.size());
  for(unsigned int i = 0; i < m_items.size(); i++){
    printf("DEBUG: Checking against the name %s\n", 
	   m_items[i].getName().c_str());

    if(m_items[i].getName() == dname){
      item = m_items[i];
      printf("DEBUG: Found\n");
      return true;
    }
  }

  printf("DEBUG: Not found\n");
  return false;
}

bool dictionary::getValue(string dname, pdf& thePDF, object& obj)
{
  for(unsigned int i = 0; i < m_items.size(); i++)
    if(m_items[i].getName() == dname){
      return thePDF.findObject(m_items[i].getIntValue(), m_items[i].getGeneration(), obj);
    }

  return false;
}

bool dictionary::getValue(string dname, string& value)
{
  for(unsigned int i = 0; i < m_items.size(); i++)
    if(m_items[i].getName() == dname){
      value = m_items[i].getStringValue();
      return true;
    }

  return false;
}
