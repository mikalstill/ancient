// Implementation of the PDF object

#include <stdio.h>

#include "objectmodel.h"

pdf::pdf(string filename):
  m_filename(filename)
{}

void pdf::setSpecVer(float version)
{
  m_specVer = version;
}

void pdf::addObject(object theObject)
{
  m_objects.push_back(theObject);
}

bool pdf::findObject(int number, int generation, object& obj)
{
  printf("DEBUG: Finding object %d %d\n", number, generation);
  for(unsigned int i = 0; i < m_objects.size(); i++)
    {
      if((m_objects[i].getNumber() == number) && 
	 (m_objects[i].getGeneration() == generation)){
	 obj = m_objects[i];
	 printf("DEBUG: Found\n");
	 return true;
      }
    }

  printf("DEBUG: Not found\n");
  return false;
}

bool pdf::findObject(dictitem::diType type, string dname, string dvalue, object& obj)
{
  for(unsigned int i = 0; i < m_objects.size(); i++)
    {
      if(m_objects[i].hasDictItem(type, dname, dvalue)){
	 obj = m_objects[i];
	 return true;
      }
    }

  return false;
}

vector<object>& pdf::getObjects()
{
  return m_objects;
}
