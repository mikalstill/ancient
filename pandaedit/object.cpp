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
m_stream (NULL),
m_changed(true)
{
}

object::object (const object & other)
{
  m_number = other.m_number;
  m_generation = other.m_generation;
  m_dictionary = other.m_dictionary;

  if (other.m_stream != NULL)
    {
      // todo_mikal: fix this!
      m_stream = new char[other.m_streamLength * 2];

      if (m_stream != NULL)
	{
	  memcpy (m_stream, other.m_stream, other.m_streamLength);
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
}

object::~object ()
{
  if (m_stream != NULL)
    {
      delete[]m_stream;
      m_stream = NULL;
    }
}

object
object::operator= (const object & other)
{
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
    }
  else
    {
      m_stream = NULL;
      m_streamLength = 0;
    }
  return *this;
}

void
object::addStream (char *streamData, unsigned int streamLength)
{
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
}

void
object::addDictionary (dictionary dict)
{
  m_dictionary = dict;
}

bool
object::hasDictItem (dictitem::diType type, string dname, string dvalue)
{
  // todo_mikal: this is inefficient
  if (hasDictItem (type, dname))
    {
      // todo_mikal: this is a hack
      dictitem foo;
      dictitem & item = foo;
      if (m_dictionary.findItem (dname, item))
	{
	  bool result = item.getStringValue () == dvalue;
	  return result;
	}
    }

  return false;
}

bool
object::hasDictItem (dictitem::diType type, string dname)
{
  // Check we exist
  if(m_number == objNumNoSuch)
    return false;

  // todo_mikal: this is a hack
  dictitem foo;
  dictitem & item = foo;
  if (m_dictionary.findItem (dname, item))
    {
      if (item.getType () == type)
	{
	  return true;
	}
      else
	{
	  debug(dlWarning, "hasDictItem hit had wrong type");
	}
    }
  return false;
}

dictionary & object::getDict ()
{
  return m_dictionary;
}

int
object::getNumber ()
{
  return m_number;
}

void
object::setNumber(int no)
{
  m_number = no;
}

int
object::getGeneration ()
{
  return m_generation;
}

void
object::setGeneration(int no)
{
  m_generation = no;
}

// Allocates memory which the caller then owns
char *
object::getStream (unsigned long &length)
{
  // Sometimes the stream is compressed, if it is, then we
  // decompress it here
  string filter;

  // todo_mikal: filters can appear as arrays of filters in the order they
  // were applied
  if ((getDict ().getValue ("Filter", filter)) && (filter != ""))
    {
      // Is this an array of filters?
      if(filter.substr(0, 1) == "["){
	stringArray filters(filter.substr(1, filter.length() - 2), " ");

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

	debug(dlTrace, "Returning global stream");
	length = outlength;
	return out;
      }
      else{
	return applyFilter(filter, m_stream, m_streamLength, length);
      }
    }
  else
    {
      char *out;
      out = new char[m_streamLength];
      memcpy(out, m_stream, m_streamLength);
      length = m_streamLength;
      return out;
    }
}

char *
object::getStream (raster & rast, unsigned long &length)
{
  // Sometimes the stream is compressed, if it is, then we
  // decompress it here
  string filter;

  // todo_mikal: filters can appear as arrays of filters in the order they
  // were applied
  if (getDict ().getValue ("Filter", filter))
    {
      return applyFilter(rast, filter, m_stream, m_streamLength, length);
    }
  else
    {
      char *out;
      out = new char[m_streamLength];
      memcpy(out, m_stream, m_streamLength);
      length = m_streamLength;
      return out;
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

void
object::appendCommand(string visible, string control, string select)
{
  command cmd;
  cmd.visible = visible;
  cmd.control = control;
  cmd.select = select;

  m_commands.push_back(cmd);
  m_changed = true;
}

unsigned int
object::getCommandCount()
{
  return m_commands.size();
}

// TODO mikal: this might not be the most efficient way of doing this. What
// about pushing each into Panda individually?
string
object::getCommandStream()
{
  string rval;

  debug(dlTrace, string("Dumping a ") + toString(m_commands.size()) +
	string(" command chain"));
  for(unsigned int i = 0; i < m_commands.size(); i++)
    {
      rval += m_commands[i].control;
      rval += m_commands[i].visible;
    }

  debug(dlTrace, "Dumping finished");
  return rval;
}
