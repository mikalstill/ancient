// A list of objects constructed from a string

#include "objectmodel.h"
#include "stringArray.h"
#include "utility.h"
#include "verbosity.h"

objectlist::objectlist ():
  m_pdf(NULL)
{
}

objectlist::objectlist (string input, pdf* thePDF):
  m_pdf (thePDF)
{
  // There might be non numeric stuff in the first few characters which
  // we don't want. For instance Oracle Reports puts \r's after the [ and
  // before the ]...
  unsigned int startchar = 0;
  while(!isNumericCharacter(input[startchar], false))
    startchar++;

  stringArray tokens (input.substr (startchar, 
				    input.length () - startchar - 1), " ");
  unsigned int inset = 0;
  objectreference ref;

  debug(dlTrace, string("Started constructing object list from ") +
	  input.substr (startchar, input.length () - startchar - 1));
  while (1)
    {
      if (!isPositiveInteger (tokens[inset])){
	debug(dlError, 
	      string("Object number for page is not a positive integer:") +
	      string(tokens[inset]));
	return;
      }
      if (!isPositiveInteger (tokens[inset + 1])){
	debug(dlError, 
	      string("Generation number for page is not a positive integer:") +
	      string(tokens[inset]));
	return;
      }
      if (tokens[inset + 2] != "R"){
	debug(dlError, 
	      string("Object reference for page lacks a reference identifier: ") +
	      tokens[inset] + string(" ") + tokens[inset + 1] + string(" ") +
	      tokens[inset + 2]);
	return;
      }

      // This can be a recursive list, so we check that here
      ref.number = atoi (tokens[inset++].c_str ());
      ref.generation = atoi (tokens[inset++].c_str ());
      object subpages(-1, -1);
      if(!thePDF->findObject(ref.number, ref.generation, subpages)){
	debug(dlError, "Referenced page does not exist");
	return;
      }

      string subkids;
      if(subpages.getDict().getValue("Kids", subkids)){
	debug(dlTrace, "This pages entry is recursive");

	objectlist pages(subkids, thePDF);
	for(unsigned int p = 0; p < pages.size(); p++){
	  ref.number = pages[p].getNumber();
	  ref.generation = pages[p].getGeneration();
	  m_objects.push_back(ref);
	}

	debug(dlTrace, string("Recursion found ") + toString(pages.size()) +
	      string(" elements"));
      }
      else{
	debug(dlTrace, string("List referenced object ") + toString(ref.number) +
	      string(" ") + toString(ref.generation));
	m_objects.push_back (ref);
      }

      inset++;
    }

  debug(dlTrace, string("Built a ") + toString(m_objects.size()) + 
	string(" item list"));
}

object
objectlist::operator[] (unsigned int i)
{
  // todo_mikal: this is a hack
  object
  foo (-1, -1);
  object & obj = foo;
  m_pdf->findObject (m_objects[i].number, m_objects[i].generation, obj);
  return obj;
}

unsigned int
objectlist::size ()
{
  return m_objects.size ();
}
