// Implementation of the PDF object
#include "objectmodel.h"
#include "verbosity.h"
#include "utility.h"

pdf::pdf ():
  m_filename(""),
  m_previousEnd(objNumNoSuch, objNumNoSuch)
{
}

pdf::pdf (string filename):
  m_filename (filename)
{
}

void
pdf::setSpecVer (float version)
{
  m_specVer = version;
}

void
pdf::addObject (object theObject)
{
  debug(dlTrace, string("Adding object ") + toString(theObject.getNumber()) +
	string(" ") + toString(theObject.getGeneration ()));
  m_objects.push_back (theObject);
}

bool
pdf::findObject (int number, int generation, object & obj)
{
  debug(dlTrace, string("Finding object ") + toString(number) +
	string(" ") + toString(generation));
  for (unsigned int i = 0; i < m_objects.size (); i++)
    {
      if ((m_objects[i].getNumber () == number) &&
	  (m_objects[i].getGeneration () == generation))
	{
	  obj = m_objects[i];
	  debug(dlTrace, "Found");
	  return true;
	}
    }

  debug(dlTrace, "Not found");
  return false;
}

bool
pdf::findObject (dictitem::diType type, string dname, string dvalue,
		 object & obj)
{
  debug(dlTrace, string("Finding object ") + dname);
  for (unsigned int i = 0; i < m_objects.size (); i++)
    {
      if (m_objects[i].hasDictItem (type, dname, dvalue))
	{
	    obj = m_objects[i];
	    debug(dlTrace, "Found");
	    return true;
	}
    }

  debug(dlTrace, "Not found");
  return false;
}

vector < object > &pdf::getObjects ()
{
  return m_objects;
}

objectlist
pdf::getPages ()
{
  try
  {
    object foo (objNumNoSuch, objNumNoSuch);

    // Find the catalog -- I could probably miss this step, but it seems like
    // a good idea for now...
    object & catalog = foo;
    if (!findObject (dictitem::diTypeName, "Type", "Catalog", catalog))
      {
 	debug(dlError, "Bad PDF: No catalog");
	return objectlist();
      }
    
    // Now find the pages object as refered to by the catalog
    debug(dlTrace, string("Catalog object is ") + 
	  toString(catalog.getNumber()) + string(" ") +
	  toString(catalog.getGeneration()));
    if (!catalog.hasDictItem (dictitem::diTypeObjectReference, "Pages"))
      {
	debug(dlError, "Bad PDF: No pages object refered to in catalog");
	return objectlist();
      }
    object & pages = foo;
    if (!catalog.getDict ().getValue ("Pages", *this, pages))
      {
	debug(dlTrace, "Bad PDF: Could not get pages object, but the catalog references it!");
	return objectlist();
      }
    
    // Now find all the page objects referenced in the pages object
    string kids;
    if (!pages.getDict ().getValue ("Kids", kids))
      {
	debug(dlError, "Bad PDF: No pages found in PDF");
	return objectlist();
      }
    
    // Find the pages, and then display just the first page
    debug(dlTrace, string("Kids = ") + kids);
    debug(dlTrace, string("Kids length = ") + toString(kids.length()));
    
    // todo_mikal: I shouldn't need to do this...
    if(kids.length() == 0)
      return objectlist();
    return objectlist (kids, this);
  }
  catch (...)
    {
      debug(dlError, "Exception caught in pdf::getPages()");
    }

  return objectlist ();
}

string
pdf::getFilename()
{
  return m_filename;
}

void
pdf::appendLine(wxPoint start, wxPoint end)
{
  if(m_previousEnd != start)
    {
      // This line does not start from the previous line
      appendMove(start);
    }
  appendLine(end);
  m_previousEnd = end;
}

void
pdf::appendLine(wxPoint pt)
{
  

}

void
pdf::appendMove(wxPoint pt)
{
  //  move cmd(pt.x, pt.y);
  //  m_commands.push_back(cmd);
}
