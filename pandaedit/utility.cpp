#include <stdio.h>
#include <ctype.h>
#include <string>

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

bool isNameCharacter (char c, bool slashAllowed)
{
  if(c == '\0') return false;
  if(c == '\t') return false;
  if(c == '\n') return false;
  if(c == '\f') return false;
  if(c == '\r') return false;
  if(c == ' ') return false;
  if((c == '/') && !slashAllowed) return false;
  return true;
}

bool isPositiveInteger(string str)
{
  for(int i = 0; i < str.length(); i++){
    if(!isdigit(str[i]))
      return false;
  }

  return true;
}

bool isNumber(string str)
{
  for(int i = 0; i < str.length(); i++){
    if(!isNumericCharacter(str[i], i == 0))
      return false;
  }

  return true;
}

bool isName(string str)
{
  if(str[0] != '/') return false;

  for(int i = 1; i < str.length(); i++){
    if(!isNameCharacter(str[i], false))
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
