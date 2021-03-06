#include "objectmodel.h"
#include "utility.h"
#include "decompressor.h"
#include "lzw.h"
#include "verbosity.h"

bool
operator== (vector < unsigned char >&a, vector < unsigned char >&b)
{
  if (a.size () != b.size ())
    return false;

  for (unsigned int i = 0; i < a.size (); i++)
    if (a[i] != b[i])
      return false;

  return true;
}

vector < unsigned char >
lzwTable::getValue (int code)
{
  if (code - 257 > m_table.size ())
    {
      vector < unsigned char >foo;
      return foo;
    }

  return m_table[code - 258];
}

bool
lzwTable::exists (vector < unsigned char >seq)
{
  for (unsigned int i = 0; i < m_table.size (); i++)
    if (seq == m_table[i])
      return true;

  return false;
}

bool
lzwTable::pushValue (vector < unsigned char >seq)
{
  m_table.push_back (seq);

  // todo_mikal: this is probably wrong
  // todo_mikal: I never check this, which is bad and will break large file decompression
  return isPowerOfTwo (m_table.size ());
}

unsigned int
lzwTable::size ()
{
  return m_table.size ();
}

void
lzwTable::clear ()
{
  m_table.clear ();
}

void
lzw::reinit ()
{
  m_table.clear ();
}

char *
lzw::decompress (char *input, unsigned long length, unsigned long &newlength)
{
  unsigned long count;
  int bitinset, valinset;
  unsigned char victim;
  int codesize = 9;
  unsigned int value;

  // Decompression variables
  vector < unsigned char >oldCode, newCode, output;
  bool setup (false);

  debug(dlTrace, string("LZW decompression of ") + toString((long) length) +
	string(" byte stream"));

  // The codebook size varies between 9 and 12, but starts at 9
  value = 0;
  valinset = 0;
  for (count = 0; count < length; count++)
    {
      for (bitinset = 0; bitinset < 8; bitinset++)
	{
	  victim = input[count];
	  victim = victim << bitinset;
	  victim = victim >> 7;
	  value += victim << (codesize - 1 - valinset);
	  valinset++;

	  if (valinset == codesize)
	    {
	      // Print out some debugging information
	      debug(dlTrace, string("LZW decompression: ") + toString(value));
	      if (value == 256)
		{
		  debug(dlTrace, "Clear table");
		  oldCode.clear ();
		  m_table.clear ();
		}
	      else if (value == 257)
		{
		  debug(dlTrace, "End of data");
		  goto done;
		}
	      else if (value < 256)
		{
		  debug(dlTrace, "Value");

		  // By definitition the first entry in the stream will be a value
		  if (!setup)
		    {
		      setup = true;
		      oldCode.push_back (value);
		      output.push_back (value);
		    }
		  else
		    {
		      newCode.clear ();
		      newCode.push_back (value);
		    }
		}
	      else
		{
		  debug(dlTrace, "Code");
		  newCode = m_table.getValue (value);
		}

	      // Deal with missed codes
	      if ((newCode.size () == 0) && (oldCode.size () > 0)
		  && (m_table.size () != 0))
		{
		  debug(dlTrace, string("LZW miss detection (") + 
			toString(m_table.size ()) + string(")"));
		  newCode = oldCode;
		  newCode.push_back (oldCode[0]);
		}

	      // Update our codebook
	      if (setup && (newCode.size () > 0))
		{
		  for (unsigned int i = 0; i < newCode.size (); i++)
		    output.push_back (newCode[i]);

		  oldCode.push_back (newCode[0]);
		  m_table.pushValue (oldCode);
		  oldCode = newCode;
		}

	      // Get ready for next time
	      value = 0;
	      valinset = 0;
	    }
	}
    }

done:
  newlength = output.size ();

  char *outputPtr = new char[output.size ()];
  for (unsigned int i = 0; i < output.size (); i++)
    {
      outputPtr[i] = output[i];
    }
  return outputPtr;
}
