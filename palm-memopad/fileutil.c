#include "fileutil.h"

void memopad_insertstring(FILE *output, char *string){
  int len = strlen(string);

  if(len < 0xFF){
    fprintf(output, "%c", len);
  }
  else if(len < 0xFFFE){
    fprintf(output, "%c", 0xFF);
    memopad_insertshort(output, len);
  }
  else{
    fprintf(output, "%c", 0xFF);
    fprintf(output, "%c", 0xFF);
    fprintf(output, "%c", 0xFF);
    memopad_insertinteger(output, len);
  }

  fprintf(output, "%s", string);
}

void memopad_insertinteger(FILE *output, int value){
  mint32 myInt;
  myInt.i = value;

  fprintf(output, "%c%c%c%c", myInt.c[0], myInt.c[1], myInt.c[2], myInt.c[3]);
}

void memopad_insertshort(FILE *output, int value){
  mint32 myInt;
  myInt.i = value;

  fprintf(output, "%c%c", myInt.c[0], myInt.c[1]);
}

void memopad_displaystring(FILE *input, char *format){
  int i;

  // Determine the length of the file -- this matches C++ CString serialization
  i = fgetc(input);
  if(i == 0xFF){
    i = memopad_displayshort(input, "  Length might be a short: ");
    if(i == 0xFFFF){
      i = memopad_displayinteger(input, "  Length might be an integer: ");
    }
  }

  printf(format, i);
  for(; i != 0; i--){
    printf("%c", fgetc(input));
  }
  printf("\n");
}

int memopad_displayinteger(FILE *input, char *format){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = fgetc(input);
  myint.c[1] = fgetc(input);
  myint.c[2] = fgetc(input);
  myint.c[3] = fgetc(input);

  printf("(int) %d\n", myint.i);
  return myint.i;
}

int memopad_displayshort(FILE *input, char *format){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = fgetc(input);
  myint.c[1] = fgetc(input);

  printf("(short) %d\n", myint.i);
  return myint.i;
}
