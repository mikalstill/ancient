// This is the implementation of the PDF object model

#include "objectmodel.h"
#include "verbosity.h"
#include "utility.h"

dictionary::dictionary ()
{
}

dictionary::dictionary (vector < dictitem > subdict)
{
  m_items = subdict;
}

dictionary::dictionary(object other)
{
  m_items = other.getDict().getItems();
}

void
dictionary::add (dictitem item)
{
  m_items.push_back (item);
}

unsigned int
dictionary::size ()
{
  return m_items.size ();
}

dictitem
dictionary::operator[] (unsigned int index)
{
  return m_items[index];
}

bool
dictionary::findItem (string dname, dictitem & item)
{
  for (unsigned int i = 0; i < m_items.size (); i++)
    {
      if (m_items[i].getName () == dname)
	{
	  item = m_items[i];
	  return true;
	}
    }

  return false;
}

bool
dictionary::getValue (string dname, pdf & thePDF, object & obj)
{
  for (unsigned int i = 0; i < m_items.size (); i++){
    if (m_items[i].getName () == dname)
      {
	return thePDF.findObject (m_items[i].getIntValue (),
				  m_items[i].getGeneration (), obj);
      }
  }

  return false;
}

bool
dictionary::getValue (string dname, string & value)
{
  for (unsigned int i = 0; i < m_items.size (); i++){
    if (m_items[i].getName () == dname)
      {
	value = m_items[i].getStringValue ();
	return true;
      }
  }

  return false;
}

bool
dictionary::getValue (string dname, int &value)
{
  for (unsigned int i = 0; i < m_items.size (); i++){
    if (m_items[i].getName () == dname)
      {
	value = m_items[i].getIntValue ();
	return true;
      }
  }

  return false;
}

bool
dictionary::getValue (string dname, dictionary & value)
{
  for (unsigned int i = 0; i < m_items.size (); i++){
    if (m_items[i].getName () == dname)
      {
	value = m_items[i].getDictionaryValue ();
	return true;
      }
  }

  return false;
}

bool
dictionary::getValue (string dname, pdf &thePDF, objectlist & objs)
{
  for (unsigned int i = 0; i < m_items.size (); i++){
    if (m_items[i].getName () == dname)
      {
	switch(m_items[i].getType()){
	case dictitem::diTypeObjectReference:
	  objs.push_back(m_items[i].getObjectReferenceValue(), &thePDF);
	  break;
	  
	case dictitem::diTypeString:
	  objs.push_back(m_items[i].getStringValue(), &thePDF);
	  break;
	  
	default:
	  debug(dlError, "Cannot convert type to an objectlist");
	  return false;
	}
	
	return true;
      }
  }

  return false;
}

vector < dictitem > dictionary::getItems ()
{
  return m_items;
}

bool dictionary::setValue(string dname, objectlist& value)
{
  dictitem target;
  if(!findItem(dname, target)){
    debug(dlError, "Set of non-existant value not currently supported");
    return false;
  }
  
  target.setValue(value);
  return true;
}
