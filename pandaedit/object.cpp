// This is the implementation of the PDF object model

#include "objectmodel.h"

object::object()
{}

object::object(int number, int generation):
  m_number(number),
  m_generation(generation)
{}

void object::addStream(char *streamData, unsigned int streamLength)
{
  //  memcpy(m_stream, streamData, streamLength);
  //  m_streamLength = streamLength;
  m_stream = strdup("Foobar");
  m_streamLength = strlen("Foobar");
}

void object::addDictionary(dictionary dict)
{
  m_dictionary = dict;
}

bool object::hasDictItem(string dname, string dvalue)
{
  // todo_mikal: this is a hack
  dictitem foo;
  dictitem& item = foo;
  if(m_dictionary.findItem(dname, item)){
    if(item.getType() == dictitem::diTypeString){
      if(item.getStringValue() == dvalue){
	return true;
      }
    }
  }

  return false;
}
