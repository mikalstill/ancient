// This is the implementation of the PDF object model

#include "objectmodel.h"

dictitem::dictitem(diType type, string name):
  m_type(type),
  m_name(name)
{}

dictitem::dictitem()
{}

void dictitem::setValue(string value)
{
  m_string = value;
}

void dictitem::setValue(int num, int gen)
{
  m_int = num;
  m_generation = gen;
}

void dictitem::setValue(int integer)
{
  m_int = integer;
}

bool dictitem::isNamed(string dname)
{
  return m_name == dname;
}

dictitem::diType dictitem::getType()
{
  return m_type;
}

string dictitem::getName()
{
  return m_name;
}

string dictitem::getStringValue()
{
  return m_string;
}
