// A list of objects constructed from a string

#include "objectmodel.h"
#include "stringArray.h"
#include "utility.h"
#include "verbosity.h"

objectlist::objectlist ():
  m_pdf(NULL)
{
}

objectlist::objectlist (string input, pdf* thePDF)
{
  push_back(input, thePDF);
}

object
objectlist::operator[] (unsigned int i)
{
  // todo_mikal: this is a hack
  object foo (-1, -1);
  object & obj = foo;
  
  if(m_pdf){
    m_pdf->findObject (m_objects[i].number, m_objects[i].generation, obj);
  }
  else{
    debug(dlError, "Object list references to a non existant PDF");
  }

  return obj;
}

unsigned int
objectlist::size ()
{
  return m_objects.size ();
}

void
objectlist::push_back(const objectreference& ref, pdf *thePDF)
{
  m_pdf = thePDF;
  m_objects.push_back(ref);
}

void
objectlist::push_back(const object &obj, pdf* thePDF)
{
  m_pdf = thePDF;

  objectreference temp;
  temp.number = ((object) obj).getNumber();
  temp.generation = ((object) obj).getGeneration();
  m_objects.push_back(temp);
}

void
objectlist::push_back(const string input, pdf *thePDF)
{
  // There might be non numeric stuff in the first few characters which
  // we don't want. For instance Oracle Reports puts \r's after the [ and
  // before the ]...
  m_pdf = thePDF;

  unsigned int startchar = 0;
  while(!isNumericCharacter(input[startchar], false))
    startchar++;

  stringArray tokens (input.substr (startchar, 
				    input.length () - startchar - 1), " ");
  objectreference ref;

  debug(dlTrace, string("Started constructing object list from ") +
	  input.substr (startchar, input.length () - startchar - 1));
  for(unsigned int inset = 0; inset < tokens.size();)
    {
      debug(dlTrace, string("Processing object candidate: ") +
	    tokens[inset] + string(" ") + tokens[inset + 1] + string(" ") +
	    tokens[inset + 2]);

      if (!isPositiveInteger (tokens[inset])){
	debug(dlTrace, 
	      string("Object number for page is not a positive integer:") +
	      string(tokens[inset]));
	return;
      }
      if (!isPositiveInteger (tokens[inset + 1])){
	debug(dlTrace, 
	      string("Generation number for page is not a positive integer:") +
	      string(tokens[inset]));
	return;
      }
      if (tokens[inset + 2] != "R"){
	debug(dlTrace, 
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
	debug(dlTrace, string("List referenced object ") + 
	      toString(ref.number) + string(" ") + toString(ref.generation));
	m_objects.push_back (ref);
      }

      inset++;
    }

  debug(dlTrace, string("Built a ") + toString(m_objects.size()) + 
	string(" item list"));
}
