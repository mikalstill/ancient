#ifndef UTILITY_HEADER
#define UTILITY_HEADER

#include <string>

// String methods
string toString (int number);
string toString (long number);
string toString (double number);
string toString (float number, bool suppressTailingZeros = false);
string toString (bool val);
string toString (char val);
string toString (size_t val);

string binaryToString (void *buf, unsigned int len);

// Evaluation methods
bool isBlankCharacter (char chr);
bool isNumericCharacter (char chr, bool negAllowed = true);
bool isNameCharacter (char chr, bool slashAllowed = true);

bool isPositiveInteger (string str);
bool isNumber (string str);

// String methods
bool isName (string str);

// Mathematical methods
int max (int a, int b);
int min (int a, int b);
int abs (int a);
bool isPowerOfTwo (int a);

#endif
