// Implementation of the PDF object

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

bool pdf::findObject(string dname, string dvalue, object& obj)
{
  for(unsigned int i = 0; i < m_objects.size(); i++)
    {
      if(m_objects[i].hasDictItem(dname, dvalue)){
	 obj = m_objects[i];
	 return true;
      }
    }

  return false;
}
