// Implementation of the PDF object

#include <stdio.h>

#include "objectmodel.h"

pdf::pdf ():
  m_filename("")
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
  printf ("DEBUG: Adding object %d %d\n", theObject.getNumber (),
	  theObject.getGeneration ());
  m_objects.push_back (theObject);
}

bool
pdf::findObject (int number, int generation, object & obj)
{
  printf ("DEBUG: Finding object %d %d\n", number, generation);
  for (unsigned int i = 0; i < m_objects.size (); i++)
    {
      if ((m_objects[i].getNumber () == number) &&
	  (m_objects[i].getGeneration () == generation))
	{
	  obj = m_objects[i];
	  printf ("DEBUG: Found\n");
	  return true;
	}
    }

  printf ("DEBUG: Not found\n");
  return false;
}

bool
pdf::findObject (dictitem::diType type, string dname, string dvalue,
		 object & obj)
{
  printf ("DEBUG: Finding object %s\n", dname.c_str ());
  for (unsigned int i = 0; i < m_objects.size (); i++)
    {
      printf ("DEBUG: Checking object %d\n", i);
      if (m_objects[i].hasDictItem (type, dname, dvalue))
	{
	    obj = m_objects[i];
	    printf ("DEBUG: Object found\n");
	    return true;
	}
    }

  printf ("DEBUG: Object not found\n");
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
    object foo (-1, -1);

    // Find the catalog -- I could probably miss this step, but it seems like
    // a good idea for now...
    object & catalog = foo;
    if (!findObject (dictitem::diTypeName, "Type", "Catalog", catalog))
      {
 	fprintf (stderr, "Bad PDF: No catalog\n");
 	exit (1);
      }
    
    // Now find the pages object as refered to by the catalog
    if (!catalog.hasDictItem (dictitem::diTypeObjectReference, "Pages"))
      {
	fprintf (stderr, "Bad PDF: No pages object refered to in catalog\n");
	exit (1);
      }
    object & pages = foo;
    if (!catalog.getDict ().getValue ("Pages", *this, pages))
      {
	fprintf (stderr,
		 "Bad PDF: Could not get pages object, but the catalog references it!\n");
	exit (1);
      }
    
    // Now find all the page objects referenced in the pages object
    string kids;
    if (!pages.getDict ().getValue ("Kids", kids))
      {
	fprintf (stderr, "Bad PDF: No pages found in PDF\n");
	exit (1);
      }
    
    // Find the pages, and then display just the first page
    return objectlist (kids, this);
  }
  catch (...)
    {
      printf("DEBUG: Exception caught in pdf::getPages()\n");
    }

  return objectlist ();
}
