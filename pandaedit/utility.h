#ifndef UTILITY_HEADER
#define UTILITY_HEADER

#include <string>

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

#endif
