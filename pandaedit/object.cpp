// This is the implementation of the PDF object model

#include "objectmodel.h"
#include <stdio.h>

object::object()
{}

object::object(int number, int generation):
  m_number(number),
  m_generation(generation)
{
  printf("DEBUG: Created a new object %d %d\n", number, generation);
}

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
  printf("DEBUG: Added a dictionary with %d items\n", dict.size());
}

bool object::hasDictItem(dictitem::diType type, string dname, string dvalue)
{
  // todo_mikal: this is inefficient
  if(hasDictItem(type, dname)){
    // todo_mikal: this is a hack
    dictitem foo;
    dictitem& item = foo;
    if(m_dictionary.findItem(dname, item))
      return item.getStringValue() == dvalue;
    
  }
}

bool object::hasDictItem(dictitem::diType type, string dname)
{
  // todo_mikal: this is a hack
  dictitem foo;
  dictitem& item = foo;
  if(m_dictionary.findItem(dname, item)){
    if(item.getType() == type){
      return true;
    }
    else{
      printf("DEBUG: Dictionary item type is wrong\n");
    }
  }
  else{
    printf("DEBUG: No dictionary item with name %s\n", dname.c_str());
  }

  return false;
}

dictionary& object::getDict()
{
  return m_dictionary;
}

int object::getNumber()
{
  return m_number;
}

int object::getGeneration()
{
  return m_generation;
}
