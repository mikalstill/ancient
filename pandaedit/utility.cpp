#include <stdio.h>
#include <ctype.h>
#include <string>

bool
isBlank (char c)
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
isNumeric (char c)
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
  if (c == '-')
    {
      return true;
    }

  return false;
}

bool isPositiveInteger(string str)
{
  for(int i = 0; i < str.length(); i++){
    if(!isdigit(str[i]))
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
