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

object& object::operator=(object& other)
{
  printf("DEBUG: Copying object\n");
  m_number = other.m_number;
  m_generation = other.m_generation;
  m_dictionary = other.m_dictionary;
  printf("DEBUG: Stream to be copied is: %s %d\n", other.m_stream,
	 other.m_streamLength);
  //memcpy(m_stream, other.m_stream, strlen(other.m_stream));
  m_streamLength = other.m_streamLength;
  printf("DEBUG: Copied\n");
  return *this;
}

void object::addStream(char *streamData, unsigned int streamLength)
{
  //  memcpy(m_stream, streamData, streamLength);
  //  m_streamLength = streamLength;
  m_stream = strdup("Foobar");
  m_streamLength = strlen("Foobar") + 1;
}

void object::addDictionary(dictionary dict)
{
  m_dictionary = dict;
  printf("DEBUG: Added a dictionary with %d items to object %d %d:\n", 
	 dict.size(), m_number, m_generation);

  for(unsigned int i = 0; i < m_dictionary.size(); i++){
    printf("DEBUG:   %s\n", m_dictionary[i].getName().c_str());
  }
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
  printf("DEBUG: Checking for a dictionary item named %s in object %d %d\n", 
	 dname.c_str(), m_number, m_generation);
  
  // todo_mikal: this is a hack
  dictitem foo;
  dictitem& item = foo;
  if(m_dictionary.findItem(dname, item)){
    if(item.getType() == type){
      printf("DEBUG: Found\n");
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
  printf("DEBUG: Grabbed a %d item dictionary\n",
	 m_dictionary.size());
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

char *object::getStream()
{
  return m_stream;
}

unsigned int object::getStreamLength()
{
  return m_streamLength;
}
