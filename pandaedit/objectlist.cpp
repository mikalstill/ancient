// A list of objects constructed from a string

#include "objectmodel.h"
#include "stringArray.h"
#include "utility.h"
#include <stdio.h>

objectlist::objectlist ():
  m_pdf(NULL)
{
}

objectlist::objectlist (string input, pdf* thePDF):
  m_pdf (thePDF)
{
  stringArray tokens (input.substr (1, input.length () - 2), " ");
  unsigned int inset = 0;
  objectreference ref;

  printf ("DEBUG: Started constructing object list from %s\n",
	  input.substr (1, input.length () - 2).c_str ());
  while (1)
    {
      if (!isPositiveInteger (tokens[inset]))
	return;
      if (!isPositiveInteger (tokens[inset + 1]))
	return;
      if (tokens[inset + 2] != "R")
	return;

      ref.number = atoi (tokens[inset++].c_str ());
      ref.generation = atoi (tokens[inset++].c_str ());
      inset++;

      printf ("DEBUG: List referenced object %d %d\n", ref.number,
	      ref.generation);
      m_objects.push_back (ref);
    }

  printf ("DEBUG: Built a %d item list\n", m_objects.size ());
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
