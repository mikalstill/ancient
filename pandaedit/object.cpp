// This is the implementation of the PDF object model

#include "objectmodel.h"
#include "verbosity.h"
#include "utility.h"

#include "decompressor.h"
#include "lzw.h"
#include "fax.h"
#include "flate.h"

#include "stringArray.h"

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
  debug(dlTrace, string("Copy constructing object ") + 
	toString(other.m_number) +
	string(" ") + toString(other.m_generation));
  m_number = other.m_number;
  m_generation = other.m_generation;
  m_dictionary = other.m_dictionary;

  if (other.m_stream != NULL)
    {
      debug(dlTrace, string("Before new (asking for ") + 
	    toString((long) other.m_streamLength) +
	    string(")"));
      // todo_mikal: fix this!
      m_stream = new char[other.m_streamLength * 2];

      if (m_stream != NULL)
	{
	  memcpy (m_stream, other.m_stream, other.m_streamLength);
	  m_streamLength = other.m_streamLength;
	  debug(dlTrace, string("Stream is: ") + 
		binaryToString(m_stream, m_streamLength));
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
  debug(dlTrace, string("Copying object ") + toString(other.m_number) + 
	string(" ") + toString(other.m_generation) + string(" to ") + 
	toString(m_number) + string(" ") + toString(m_generation));
  m_number = other.m_number;
  m_generation = other.m_generation;
  m_dictionary = other.m_dictionary;

  if (m_stream != NULL)
    delete[]m_stream;

  if (other.m_stream != NULL)
    {
      // todo_mikal: fix this!
      m_stream = new char[other.m_streamLength * 2];
      memcpy (m_stream, other.m_stream, other.m_streamLength);
      m_streamLength = other.m_streamLength;
      debug(dlTrace, string("Stream is: ") + 
	    binaryToString(m_stream, m_streamLength));
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
  debug(dlTrace, "Added a stream to object " + toString(m_number) + 
	string(" ") + toString(m_generation));
  debug(dlTrace, string("Stream is: ") + 
	binaryToString(streamData, streamLength));

  // todo_mikal: fix this!
  m_stream = new char[streamLength * 2];
  if (m_stream != NULL)
    {
      memcpy (m_stream, streamData, streamLength);
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
	string(" in object ") + toString(m_number) + string(" ") + 
	toString(m_generation));
  
  // todo_mikal: this is inefficient
  if (hasDictItem (type, dname))
    {
      // todo_mikal: this is a hack
      dictitem foo;
      dictitem & item = foo;
      if (m_dictionary.findItem (dname, item))
	return item.getStringValue () == dvalue;
    }

  return false;
}

bool
object::hasDictItem (dictitem::diType type, string dname)
{
  debug(dlTrace, string("Checking for a dictionary item named ") + dname +
	string(" in object ") + toString(m_number) + string(" ") + 
	toString(m_generation));

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
	  debug(dlTrace, 
		string("Dictionary item type is wrong. The item is of type: ") +
		toString(item.getType()));
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

  debug(dlTrace, string("Object stream starts with: ") +
	binaryToString(m_stream, min(10, m_streamLength)) + 
	string(" (first 10 bytes)"));

  // todo_mikal: filters can appear as arrays of filters in the order they
  // were applied
  if ((getDict ().getValue ("Filter", filter)) && (filter != ""))
    {
      needsStreamClean = true;
	
      // Is this an array of filters?
      if(filter.substr(0, 1) == "["){
	stringArray filters(filter.substr(1, filter.length() - 2), " ");
	debug(dlTrace, string("Filter array is: ") + 
	      filter.substr(1, filter.length() - 2));
	debug(dlTrace, string("Array contains ") + toString((int) filters.size()) +
	      string(" elements"));

	// Setup the decompression loop
	char *out;
	unsigned long outlength = m_streamLength;
        out = new char[outlength];
	memcpy(out, m_stream, outlength);

	for(unsigned int i = 0; i < filters.size(); i++){
	  char *in;
	  unsigned long inlength = outlength;
	  in = new char[inlength];
	  memcpy(in, out, inlength);
	  
	  // Apply the filter
	  out = applyFilter(filters[i], in, inlength, outlength);

	  // Cleanup
	  delete[] in;
	}

	length = outlength;
	return out;
      }
      else{
	return applyFilter(filter, m_stream, m_streamLength, length);
      }
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
  debug(dlTrace, "Raster decompression started");

  // todo_mikal: filters can appear as arrays of filters in the order they
  // were applied
  if (getDict ().getValue ("Filter", filter))
    {
      needsStreamClean = true;
      return applyFilter(rast, filter, m_stream, m_streamLength, length);
    }
  else
    {
      needsStreamClean = false;
      length = m_streamLength;
      return m_stream;
    }
}

char *
object::applyFilter(string filter, char *instream, unsigned long inlength,
		    unsigned long& length)
{
  string realfilter;
  debug(dlTrace, string("The stream is filtered with filter ") +
	filter);
  if(filter.substr(0, 1) == "/")
    realfilter = filter.substr(1, filter.length() - 1);
  else
    realfilter = filter;
  debug(dlTrace, string("The stream is really filtered with filter ") +
	 realfilter);

  decompressor *mydec = NULL;
  char *stream = NULL;
  length = 0;

  if (realfilter == "LZWDecode")
    {
      mydec = new lzw ();
    }
  else if (realfilter == "CCITTFaxDecode")
    {
      debug(dlError, "Fax decode not supported for non raster streams");
    }
  else if (realfilter == "FlateDecode")
    {
      mydec = new flate();
    }
  else
    {
      debug(dlTrace, "Unknown stream filter");
    }
  
  if (mydec != NULL)
    {
      stream = mydec->decompress (instream, inlength, length);
      delete mydec;
    }
  else
    {
      debug(dlTrace, string("Unknown compression scheme ") + realfilter);
      stream = NULL;
    }
  
  return stream;
}

char *
object::applyFilter(raster& rast, string filter, 
		    char *instream, unsigned long inlength, 
		    unsigned long& length){
  string realfilter;
  debug(dlTrace, string("The stream is filtered with filter ") +
	filter);
  if(filter.substr(0, 1) == "/")
    realfilter = filter.substr(1, filter.length() - 1);
  else
    realfilter = filter;
  debug(dlTrace, string("The stream is really filtered with filter ") +
	realfilter);
  
  decompressor *mydec = NULL;
  char *stream = NULL;
  length = 0;
  
  if (realfilter == "LZWDecode")
    {
      mydec = new lzw ();
    }
  else if (realfilter == "CCITTFaxDecode")
    {
      mydec = new fax ();
      ((fax *) mydec)->setWidth(rast.getWidth());
      ((fax *) mydec)->setLength(rast.getHeight());
      ((fax *) mydec)->setK(rast.getK());
    }
  else if (realfilter == "FlateDecode")
    {
      mydec = new flate();
    }
  
  if (mydec != NULL)
    {
      stream = mydec->decompress (instream, inlength, length);
      delete mydec;
    }
  else
    {
      debug(dlTrace, string("Unknown compression scheme ") + realfilter);
      stream = NULL;
    }
  
  return stream;
}

unsigned long
object::getStreamLength ()
{
  return m_streamLength;
}
