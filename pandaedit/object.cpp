// This is the implementation of the PDF object model

#include "objectmodel.h"
#include <stdio.h>

object::object(int number, int generation):
  m_number(number),
  m_generation(generation),
  m_stream(NULL)
{
  printf("DEBUG: Created a new object %d %d\n", number, generation);
}

object::object(const object& other)
{
  printf("DEBUG: Copy constructing object %d %d\n", other.m_number, other.m_generation);
  m_number = other.m_number;
  m_generation = other.m_generation;
  m_dictionary = other.m_dictionary;

  if(other.m_stream != NULL){
    printf("DEBUG: Before new (asking for %d)\n", other.m_streamLength);
    m_stream = new char [other.m_streamLength * 2];
    printf("DEBUG: After new m_stream is 0x%08x\n", m_stream);
    
    if(m_stream != NULL){
      memcpy(m_stream, other.m_stream, other.m_streamLength);
      m_stream[other.m_streamLength] = '\0';
      m_streamLength = other.m_streamLength;
    }
    else{
      printf("DEBUG: Memory allocation failed\n");
    }
  }
  else{
    m_stream = NULL;
    m_streamLength = 0;
  }
  printf("DEBUG: Copied\n");
}

object::~object()
{
  printf("DEBUG: Deleting object %d %d\n", m_number, m_generation);
  if(m_stream != NULL){
    printf("DEBUG: Cleaning up stream 0x%08x\n", m_stream);
    delete[] m_stream;
    m_stream = NULL;
  }
  printf("DEBUG: Finished cleaning up\n");
}

object object::operator=(const object& other)
{
  printf("DEBUG: Copying object %d %d to %d %d\n", other.m_number, other.m_generation,
	 m_number, m_generation);
  m_number = other.m_number;
  m_generation = other.m_generation;
  m_dictionary = other.m_dictionary;
  
  if(m_stream != NULL)
    delete[] m_stream;

  if(other.m_stream != NULL){
    m_stream = new char [other.m_streamLength * 2];
    memcpy(m_stream, other.m_stream, other.m_streamLength);
    m_stream[other.m_streamLength] = '\0';
    m_streamLength = other.m_streamLength;
  }
  else{
    m_stream = NULL;
    m_streamLength = 0;
  }
  printf("DEBUG: Copied\n");
  return *this;
}

void object::addStream(char *streamData, unsigned int streamLength)
{
  printf("DEBUG: Added a stream to object %d %d\n", m_number, m_generation);
  m_stream = new char [streamLength * 2];
  if(m_stream != NULL){
    memcpy(m_stream, streamData, streamLength);
    m_stream[streamLength] = '\0';
    m_streamLength = streamLength;
  }
  else{
    printf("DEBUG: Failed to allocate enough memory for the stream\n");
  }
  printf("DEBUG: Added\n");
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
