#include "fileutil.h"
#include "winlang.h"

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

// Display a C++ serialized string
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
}

// Display an 8 byte long
long fileutil_displaylong(FILE *input, char *format){
  mint64 mylong;

  mylong.i = 0;
  printf(format);
  mylong.c[0] = fgetc(input);
  mylong.c[1] = fgetc(input);
  mylong.c[2] = fgetc(input);
  mylong.c[3] = fgetc(input);
  mylong.c[4] = fgetc(input);
  mylong.c[5] = fgetc(input);
  mylong.c[6] = fgetc(input);
  mylong.c[7] = fgetc(input);

  printf("(long long) %d [8 bytes]", mylong.i);
  return mylong.i;
}

// Display a 4 byte int
int fileutil_displayinteger(FILE *input, char *format){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = fgetc(input);
  myint.c[1] = fgetc(input);
  myint.c[2] = fgetc(input);
  myint.c[3] = fgetc(input);

  printf("(int) %d [4 bytes]", myint.i);
  return myint.i;
}

// Display a short (aka DWORD), 2 bytes
int fileutil_displayshort(FILE *input, char *format){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = fgetc(input);
  myint.c[1] = fgetc(input);

  printf("(short) %d [2 bytes]", myint.i);
  return myint.i;
}

char *fileutil_displaybyteblock(FILE *input, char *format, int len){
  int count;
  unsigned char *result;

  printf("%s (%d bytes) ", format, len);

  if((result = (unsigned char *) malloc(sizeof(char) * len)) == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }

  for(count = 0; count < len; count++){
    result[count] = fileutil_displaybyte_actual(input, "");
  }

  return result;
}

int fileutil_displaybyte(FILE *input, char *format){
  printf(format);
  return fileutil_displaybyte_actual(input, "(byte) ");
}

// Display a byte (aka WORD)
int fileutil_displaybyte_actual(FILE *input, char *format){
  unsigned char byte;

  printf(format);
  byte = fgetc(input);
  printf("%d", byte);
  return byte;
}

// Display a language ID having made a half hearted attempt to decode it
int fileutil_displaywindowslanguage(FILE *input, char *format){
  mint32 myint;
  int index;

  myint.i = 0;
  printf(format);
  myint.c[0] = fgetc(input);
  myint.c[1] = fgetc(input);
  myint.c[2] = fgetc(input);
  myint.c[3] = fgetc(input);
  
  index = 0;
  while(languages[index].name != NULL){
    if(languages[index].id == myint.i){
      printf("0x%04x %s (%s, %s)", 
	     languages[index].id,
	     languages[index].name,
	     languages[index].lang,
	     languages[index].sublang);
      return myint.i;
    }

    index++;
  }
     
  printf("0x%04x (unknown language)", myint.i);
  return myint.i;
}

// Display a MS GUID
char *fileutil_displayguid(FILE *input, char *format){
  printf(format);
  fileutil_displayinteger(input, "");
  fileutil_displayshort(input, " ");
  fileutil_displayshort(input, " ");
  fileutil_displaybyteblock(input, " ", 8);
}

// "Encoded integer", used by MS CHM
int fileutil_displayencinteger(FILE *input, char *format, int *read){
  int continued = 1, val, accval = 0;
  unsigned char b;
  printf(format);

  printf("(encoded int) ");
  while(continued == 1){
    b = fgetc(input);
    continued = b >> 7;
    val = b << 1;
    val = val >> 1;

    printf("[%d %d] ", continued, val);
    accval += val;

    *read++;
  }

  printf("= %d", accval);
}
