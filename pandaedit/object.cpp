// This is the implementation of the PDF object model

#include "objectmodel.h"
#include "verbosity.h"
#include "utility.h"

#include "decompressor.h"
#include "lzw.h"
#include "fax.h"

object::object (int number, int generation):
m_number (number),
m_generation (generation),
m_stream (NULL)
{
  debug(dlTrace, string("Created a new object ") + toString(number) +
	string(" ") + toString(generation));
}

object::object (const object & other)
{
  debug(dlTrace, string("Copy constructing object ") + toString(other.m_number) +
	string(" ") + toString(other.m_generation));
  m_number = other.m_number;
  m_generation = other.m_generation;
  m_dictionary = other.m_dictionary;

  if (other.m_stream != NULL)
    {
      debug(dlTrace, string("Before new (asking for ") + toString((long) other.m_streamLength) +
	    string(")"));
      m_stream = new char[other.m_streamLength * 2];

      if (m_stream != NULL)
	{
	  memcpy (m_stream, other.m_stream, other.m_streamLength);
	  m_stream[other.m_streamLength] = '\0';
	  m_streamLength = other.m_streamLength;
	}
      else
	{
	  debug(dlError, "Memory allocation failed");
	}
    }
  else
    {
      m_stream = NULL;
      m_streamLength = 0;
    }
  debug(dlTrace, "Copied");
}

object::~object ()
{
  debug(dlTrace, string("Deleting object ") + toString(m_number) +
	string(" ") + toString(m_generation));
  if (m_stream != NULL)
    {
      delete[]m_stream;
      m_stream = NULL;
    }
  debug(dlTrace, "Finished cleaning up");
}

object
object::operator= (const object & other)
{
  debug(dlTrace, string("Copying object ") + toString(other.m_number) + string(" ") +
	toString(other.m_generation) + string(" to ") + toString(m_number) +
	string(" ") + toString(m_generation));
  m_number = other.m_number;
  m_generation = other.m_generation;
  m_dictionary = other.m_dictionary;

  if (m_stream != NULL)
    delete[]m_stream;

  if (other.m_stream != NULL)
    {
      m_stream = new char[other.m_streamLength * 2];
      memcpy (m_stream, other.m_stream, other.m_streamLength);
      m_stream[other.m_streamLength] = '\0';
      m_streamLength = other.m_streamLength;
    }
  else
    {
      m_stream = NULL;
      m_streamLength = 0;
    }
  debug(dlTrace, "DEBUG: Copied");
  return *this;
}

void
object::addStream (char *streamData, unsigned int streamLength)
{
  debug(dlTrace, "Added a stream to object " + toString(m_number) + string(" ") +
	toString(m_generation));
  m_stream = new char[streamLength * 2];
  if (m_stream != NULL)
    {
      memcpy (m_stream, streamData, streamLength);
      m_stream[streamLength] = '\0';
      m_streamLength = streamLength;
    }
  else
    {
      debug(dlError, "Failed to allocate enough memory for the stream");
    }
  debug(dlTrace, "Added");
}

void
object::addDictionary (dictionary dict)
{
  m_dictionary = dict;
  debug(dlTrace, string("Added a dictionary with ") + toString(dict.size()) +
	string(" items to object ") + toString(m_number) + string(" ") +
	toString(m_generation));

  for (unsigned int i = 0; i < m_dictionary.size (); i++)
    {
      debug(dlTrace, string("   ") + m_dictionary[i].getName ());
    }
}

bool
object::hasDictItem (dictitem::diType type, string dname, string dvalue)
{
  debug(dlTrace, string("Checking for a dictionary item named ") + dname +
    string(" in object ") + toString(m_number) + string(" ") + toString(m_generation));

  // todo_mikal: this is inefficient
  if (hasDictItem (type, dname))
    {
      // todo_mikal: this is a hack
      dictitem foo;
      dictitem & item = foo;
      if (m_dictionary.findItem (dname, item))
	return item.getStringValue () == dvalue;
    }
}

bool
object::hasDictItem (dictitem::diType type, string dname)
{
  debug(dlTrace, string("Checking for a dictionary item named ") + dname +
    string(" in object ") + toString(m_number) + string(" ") + toString(m_generation));

  // todo_mikal: this is a hack
  dictitem foo;
  dictitem & item = foo;
  if (m_dictionary.findItem (dname, item))
    {
      if (item.getType () == type)
	{
	  debug(dlTrace, "Found");
	  return true;
	}
      else
	{
	  debug(dlTrace, "Dictionary item type is wrong");
	}
    }
  else
    {
      debug(dlTrace, string("No dictionary item with name ") + dname);
    }

  return false;
}

dictionary & object::getDict ()
{
  debug(dlTrace, string("Grabbed a ") + toString(m_dictionary.size()) +
	string(" item dictionary"));
  return m_dictionary;
}

int
object::getNumber ()
{
  return m_number;
}

int
object::getGeneration ()
{
  return m_generation;
}

char *
object::getStream (bool & needsStreamClean, unsigned long &length)
{
  // Sometimes the stream is compressed, if it is, then we
  // decompress it here
  string filter;
  char *stream;

  if (getDict ().getValue ("Filter", filter))
    {
      debug(dlTrace, string("The stream is filtered with filter ") +
	    filter);
      decompressor *mydec = NULL;

      if (filter == "LZWDecode")
	{
	  mydec = new lzw ();
	}
      else if (filter == "FlateDecode")
	{
	  mydec = new fax ();
	}
      else
	{
	  debug(dlTrace, "Unknown stream filter");
	}

      if (mydec != NULL)
	{
	  stream = mydec->decompress (m_stream, m_streamLength, length);
	  delete mydec;
	  needsStreamClean = true;
	}
      else
	{
	  debug(dlTrace, string("Unknown compression scheme ") + filter);
	  stream = NULL;
	}

      return stream;
    }
  else
    {
      needsStreamClean = false;
      length = m_streamLength;
      return m_stream;
    }
}

char *
object::getStream (raster & rast, bool & needsStreamClean,
		   unsigned long &length)
{
  // Sometimes the stream is compressed, if it is, then we
  // decompress it here
  string filter;
  char *stream;

  if (getDict ().getValue ("Filter", filter))
    {
      debug(dlTrace, string("The stream is filtered with filter ") +
	filter);
      decompressor *mydec = NULL;

      if (filter == "LZWDecode")
	{
	  mydec = new lzw ();
	}
      else if (filter == "CCITTFaxDecode")
	{
	  mydec = new fax ();
	}

      if (mydec != NULL)
	{
	  stream = mydec->decompress (m_stream, m_streamLength, length);
	  delete mydec;
	  needsStreamClean = true;
	}
      else
	{
	  debug(dlTrace, string("Unknown compression scheme ") + filter);
	  stream = NULL;
	}

      return stream;
    }
  else
    {
      needsStreamClean = false;
      length = m_streamLength;
      return m_stream;
    }
}

unsigned long
object::getStreamLength ()
{
  return m_streamLength;
}
