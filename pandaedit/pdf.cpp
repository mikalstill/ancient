// Implementation of the PDF object
#include "objectmodel.h"
#include "verbosity.h"
#include "utility.h"

pdf::pdf ():
  m_filename(""),
  m_previousEnd(objNumNoSuch, objNumNoSuch),
  m_highestObject(0)
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
pdf::addObject (object& theObject)
{
  if(theObject.getNumber() == objNumAppended)
    {
      debug(dlTrace, "Issue an object number to the object");
      theObject.setNumber(m_highestObject + 1);
      theObject.setGeneration(0);
    }

  debug(dlTrace, string("Adding object ") + toString(theObject.getNumber()) +
	string(" ") + toString(theObject.getGeneration ()));
  m_objects.push_back (theObject);

  // Keep track of what the highest object number we have seen is
  if(theObject.getNumber() > m_highestObject)
    m_highestObject = theObject.getNumber();
}

bool
pdf::findObject (int number, int generation, int& inset)
{
  for (unsigned int i = 0; i < m_objects.size (); i++)
    {
      if ((m_objects[i].getNumber () == number) &&
	  (m_objects[i].getGeneration () == generation))
	{
	  inset = i;
	  return true;
	}
    }

  debug(dlTrace, "Not found in pdf::findObject(int)");
  inset = -1;
  return false;
}

bool
pdf::findObject (int number, int generation, object & obj)
{
  int i;
  if(!findObject(number, generation, i))
    return false;
  
  obj = m_objects[i];
  obj.setReadOnly();
  
  debug(dlTrace, "Found in pdf::findObject(objectreference)");
  return true;
}

bool
pdf::findObject (dictitem::diType type, string dname, string dvalue,
		 object & obj)
{
  for (unsigned int i = 0; i < m_objects.size (); i++)
    {
      if (m_objects[i].hasDictItem (type, dname, dvalue))
	{
	    obj = m_objects[i];
	    debug(dlTrace, "Found in pdf::findObject(object)");
	    return true;
	}
    }

  debug(dlTrace, "Not found in pdf::findObject(object)");
  return false;
}

vector < object > &pdf::getObjects ()
{
  return m_objects;
}

bool
pdf::getCatalogObject(object& catalog)
{
  return findObject (dictitem::diTypeName, "Type", "Catalog", catalog);
}

bool
pdf::getPagesObject(object& pages)
{
  object tempobject(objNumNoSuch, objNumNoSuch);
  object& catalog = tempobject;
  if(!getCatalogObject(catalog))
    {
      debug(dlError, "Couldn't find catalog object");
      return false;
    }
  if (!catalog.hasDictItem (dictitem::diTypeObjectReference, "Pages"))
    {
      return false;
    }
  if (!catalog.getDict ().getValue ("Pages", *this, pages))
    {
      return false;
    }
  return true;
}

objectlist
pdf::getPages ()
{
  try
  {
    object tempobject(objNumNoSuch, objNumNoSuch);
    object& pages = tempobject;
    if(!getPagesObject(pages))
      return objectlist();
    
    // Now find all the page objects referenced in the pages object
    string kids;
    if (!pages.getDict ().getValue ("Kids", kids))
      {
	debug(dlTrace, "Bad PDF: No pages found in PDF");
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
pdf::appendPage(object& thePage)
{
  debug(dlTrace, "PDF appending page");
  object tempobject(objNumNoSuch, objNumNoSuch);
  object & pages = tempobject;
  if(!getPagesObject(pages)){
    debug(dlError, "Cannot append to non-existant pages object");
    return;
  }
  
  debug(dlTrace, "Finding the kids list within the pages object");
  objectlist footoo;
  objectlist & kids = footoo;
  if(!pages.getDict().getValue("Kids", ((pdf &) *this), kids))
    {
      debug(dlError, "Bad PDF for page append: No kids");
      return;
    }

  // This is just a representation of the kids list from the objectmodel,
  // not the actual storage. We thus need to push this back into the
  // dictionary...
  kids.push_back(thePage, this);
  if(!pages.getDict().setValue("Kids", kids))
    {
      debug(dlError, "Could not update kids list");
      return;
    }

  debug(dlTrace, string("PDF append finished. There are now ") +
	toString(kids.size()) + string(" pages"));
}

void
pdf::appendCommand(objectreference obj, command cmd)
{
  int inset;
  if(!findObject(obj.number, obj.generation, inset))
    return;

  m_objects[inset].appendCommand(cmd);
}
