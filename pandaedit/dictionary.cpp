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

void
dictionary::add (dictitem item)
{
  m_items.push_back (item);
  debug(dlTrace, "Added a dictionary item");
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
  debug(dlTrace, string("Finding ") + dname + string(" in a ") +
	toString(m_items.size()) + string(" item dictionary"));
  for (unsigned int i = 0; i < m_items.size (); i++)
    {
      debug(dlTrace, string("Checking against the name ") +
	      m_items[i].getName ());

      if (m_items[i].getName () == dname)
	{
	  item = m_items[i];
	  debug(dlTrace, "Found");
	  return true;
	}
    }

  debug(dlTrace, "Not found");
  return false;
}

bool
dictionary::getValue (string dname, pdf & thePDF, object & obj)
{
  debug(dlTrace, string("Get object named ") + dname + string(" from dictionary "));
  for (unsigned int i = 0; i < m_items.size (); i++)
    if (m_items[i].getName () == dname)
      {
	return thePDF.findObject (m_items[i].getIntValue (),
				  m_items[i].getGeneration (), obj);
      }

  return false;
}

bool
dictionary::getValue (string dname, string & value)
{
  debug(dlTrace, string("Get string named ") + dname + string(" from dictionary"));
  for (unsigned int i = 0; i < m_items.size (); i++)
    if (m_items[i].getName () == dname)
      {
	value = m_items[i].getStringValue ();
	return true;
      }

  return false;
}

bool
dictionary::getValue (string dname, int &value)
{
  debug(dlTrace, string("Get int named ") + dname + string(" from dictionary"));
  for (unsigned int i = 0; i < m_items.size (); i++)
    if (m_items[i].getName () == dname)
      {
	value = m_items[i].getIntValue ();
	return true;
      }

  return false;
}

bool
dictionary::getValue (string dname, dictionary & value)
{
  debug(dlTrace, string("Get subdictionary named ") + dname + string(" from dictionary"));
  for (unsigned int i = 0; i < m_items.size (); i++)
    if (m_items[i].getName () == dname)
      {
	value = m_items[i].getDictionaryValue ();
	return true;
      }

  return false;
}

vector < dictitem > dictionary::getItems ()
{
  return m_items;
}
