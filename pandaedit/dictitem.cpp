// This is the implementation of the PDF object model

#include "objectmodel.h"
#include "verbosity.h"
#include "utility.h"

dictitem::dictitem (diType type, string name):
m_type (type), m_name (name)
{
  debug(dlTrace, string("Created an item with name ") + name + 
	string(", type ") + toString(type));
}

dictitem::dictitem ()
{
}

void
dictitem::setValue (string value)
{
  m_string = value;
}

void
dictitem::setValue (int num, int gen)
{
  m_int = num;
  m_generation = gen;
}

void
dictitem::setValue (int integer)
{
  m_int = integer;
}

void
dictitem::setValue (dictionary dict)
{
  m_dictionary = dict.getItems ();
}

dictitem::diType dictitem::getType ()
{
  return m_type;
}

string
dictitem::getName ()
{
  return m_name;
}

int
dictitem::getIntValue ()
{
  return m_int;
}

int
dictitem::getGeneration ()
{
  return m_generation;
}

string
dictitem::getStringValue ()
{
  return m_string;
}

dictionary
dictitem::getDictionaryValue ()
{
  return dictionary (m_dictionary);
}
