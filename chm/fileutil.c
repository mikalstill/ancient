#include "fileutil.h"

void fileutil_insertstring(FILE *output, char *string){
  int len = strlen(string);

  if(len < 0xFF){
    fprintf(output, "%c", len);
  }
  else if(len < 0xFFFE){
    fprintf(output, "%c", 0xFF);
    fileutil_insertshort(output, len);
  }
  else{
    fprintf(output, "%c", 0xFF);
    fprintf(output, "%c", 0xFF);
    fprintf(output, "%c", 0xFF);
    fileutil_insertinteger(output, len);
  }

  fprintf(output, "%s", string);
}

void fileutil_insertinteger(FILE *output, int value){
  mint32 myInt;
  myInt.i = value;

  fprintf(output, "%c%c%c%c", myInt.c[0], myInt.c[1], myInt.c[2], myInt.c[3]);
}

void fileutil_insertshort(FILE *output, int value){
  mint32 myInt;
  myInt.i = value;

  fprintf(output, "%c%c", myInt.c[0], myInt.c[1]);
}

void fileutil_displaystring(FILE *input, char *format){
  int i;

  // Determine the length of the file -- this matches C++ CString serialization
  i = fgetc(input);
  if(i == 0xFF){
    i = fileutil_displayshort(input, "  Length might be a short: ");
    if(i == 0xFFFF){
      i = fileutil_displayinteger(input, "  Length might be an integer: ");
    }
  }

  printf(format, i);
  for(; i != 0; i--){
    printf("%c", fgetc(input));
  }
  printf("\n");
}

int fileutil_displayinteger(FILE *input, char *format){
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

int fileutil_displayshort(FILE *input, char *format){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = fgetc(input);
  myint.c[1] = fgetc(input);

  printf("(short) %d\n", myint.i);
  return myint.i;
}

int fileutil_displaywindowslanguage(FILE *input, char *format){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = fgetc(input);
  myint.c[1] = fgetc(input);
  myint.c[2] = fgetc(input);
  myint.c[3] = fgetc(input);
  
  switch(myint.i){
  case 0x0409:
    printf("(windows language) LANG_ENGLISH/SUBLANG_ENGLISH_US");
    break;

  case 0x0407:
    printf("(windows language) LANG_GERMAN/SUBLANG_GERMAN");
    break;

  default:
    printf("(windows language) %#x\n", myint.i);
    break;
  }

  return myint.i;
}

char *fileutil_displayguid(FILE *input, char *format){
  mint32 myint1, myint2, count;
  char bytes[8], result[;

  printf(format);

  myint1.i = 0;
  myint1.c[0] = fgetc(input);
  myint1.c[1] = fgetc(input);
  myint1.c[2] = fgetc(input);
  myint1.c[3] = fgetc(input);

  myint2.i = 0;
  myint2.c[0] = fgetc(input);
  myint2.c[1] = fgetc(input);
  myint2.c[2] = fgetc(input);
  myint2.c[3] = fgetc(input);
  
  for(count = 0; count < 8; count++){
    bytes[count] = fgetc(input);
  }

  
}
