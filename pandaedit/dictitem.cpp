// This is the implementation of the PDF object model

#include "objectmodel.h"
#include "verbosity.h"
#include "utility.h"

dictitem::dictitem (diType type, string name, pdf *thePDF):
  m_type (type), m_name (name), m_pdf(thePDF)
{
  debug(dlTrace, string("Created an item with name ") + name + 
	string(", type ") + toString(type));
}

dictitem::dictitem ():
  m_pdf(NULL)
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
  switch(m_type){
  case diTypeName:
  case diTypeString:
    return m_string;
  
  case diTypeObjectReference:
    debug(dlTrace, "Dictionary item refers to remote object invocation");
    if(m_pdf){
      object value(-1, -1);
      if(m_pdf->findObject(m_int, m_generation, value))
	{
	  string strval;
	  if(value.getDict().getValue("STREAM", strval))
	    return strval;
	}
      }
    break;

  default:
    // todo_mikal: a toString for these types
    debug(dlError, string("Cannot convert dictionary type ") +
	  toString(m_type) + string(" to a string"));
    break;
  }

  // If we get here, m_string is empty anyway...
  debug(dlTrace, "Value lookup failed");
  return m_string;
}

dictionary
dictitem::getDictionaryValue ()
{
  switch(m_type){
  case diTypeDictionary:
    return dictionary (m_dictionary);
  
  case diTypeObjectReference:
    debug(dlTrace, "Dictionary item refers to remote object invocation");
    if(m_pdf){
      object value(-1, -1);
      if(m_pdf->findObject(m_int, m_generation, value))
	{
	  return value;
	}
      }
    break;

  default:
    // todo_mikal: a toString for these types
    debug(dlError, string("Cannot convert dictionary type ") +
	  toString(m_type) + string(" to a dictionary"));
    break;
  }

  // If we get here, m_dictionary is empty anyway...
  debug(dlTrace, "Value lookup failed");
  return m_dictionary;
}

objectreference
dictitem::getObjectReferenceValue()
{
  objectreference ref;
  ref.number = m_int;
  ref.generation = m_generation;
  return ref;
}
