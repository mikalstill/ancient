#include <stdio.h>
#include <ctype.h>
#include <string>
#include <stdio.h>

string
toString (int number)
{
  char buffer[10];

  snprintf (buffer, 10, "%d", number);
  return string (buffer);
}

string
toString (long number)
{
  char buffer[20];

  snprintf (buffer, 20, "%ld", number);
  return string (buffer);
}

string
toString (double number)
{
  char buffer[20];

  snprintf (buffer, 20, "%f", number);
  return string (buffer);
}

string
toString (float number, bool suppressTrailingZeros)
{
  char buffer[10];

  snprintf (buffer, 10, "%f", number);

  // Remove trailing zeros (sometimes)
  if (suppressTrailingZeros)
    {
      int i = strlen (buffer) - 1;
      while (buffer[i] == '0')
	{
	  i--;
	}
      buffer[i + 1] = '\0';
    }

  // If there is just a point sign, then put a zero back...
  if (buffer[strlen (buffer) - 1] == '.')
    {
      return string (string (buffer) + "0");
    }

  return string (buffer);
}

string
toString (bool val)
{
  if (val)
    return "true";
  return "false";
}

string
toString (char val)
{
  char buffer[2];

  snprintf (buffer, 2, "%c", val);
  return string (buffer);
}

string
toString (size_t val)
{
  char buffer[10];

  snprintf (buffer, 10, "%d", val);
  return string (buffer);
}

bool
isBlankCharacter (char c)
{
  if (c == ' ')
    return true;
  if (c == '\t')
    return true;
  if (c == '\r')
    return true;
  if (c == '\n')
    return true;

  return false;
}

bool
isNumericCharacter (char c, bool negAllowed)
{
  // This line of code:
  //
  // if(((c < 48) || (c > 57)) && (isBlank(c) == false) && (c != '.') 
  //    && (c != '-'))
  //
  // Became:

  if (isdigit (c))
    {
      return true;
    }
  if (c == '.')
    {
      return true;
    }
  if ((c == '-') && negAllowed)
    {
      return true;
    }

  return false;
}

bool
isNameCharacter (char c, bool slashAllowed)
{
  if (c == '\0')
    return false;
  if (c == '\t')
    return false;
  if (c == '\n')
    return false;
  if (c == '\f')
    return false;
  if (c == '\r')
    return false;
  if (c == ' ')
    return false;
  if ((c == '/') && !slashAllowed)
    return false;
  return true;
}

bool
isPositiveInteger (string str)
{
  for (int i = 0; i < str.length (); i++)
    {
      if (!isdigit (str[i]))
	return false;
    }

  return true;
}

bool
isNumber (string str)
{
  for (int i = 0; i < str.length (); i++)
    {
      if (!isNumericCharacter (str[i], i == 0))
	return false;
    }

  return true;
}

bool
isName (string str)
{
  if (str[0] != '/')
    return false;

  for (int i = 1; i < str.length (); i++)
    {
      if (!isNameCharacter (str[i], false))
	return false;
    }

  return true;
}

int
max (int a, int b)
{
  if (a > b)
    return a;
  return b;
}

int
min (int a, int b)
{
  if (a < b)
    return a;
  return b;
}

int
abs (int a)
{
  if (a < 0)
    return -a;
  return a;
}

bool
isPowerOfTwo (int test)
{
  unsigned int val = 1;

  while (val < 32768)
    {
      if (test == val)
	return true;
      val = val << 1;
    }

  return false;
}
