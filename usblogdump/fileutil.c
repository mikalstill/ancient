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
void fileutil_displaystring(char *input, char *format, long long *filep){
  int i, read, readlocal;
  long long count = *filep;

  // Determine the length of the file -- this matches C++ CString serialization
  // The byte length broke MFC serialization
  //i = fileutil_displaybyte(input, "  Length might be a byte: ", &count); printf("\n");
  //if(i == 0xFF){
  i = fileutil_displayshort(input, "  Length might be a short: ", &count); printf("\n");
  if(i == 0xFFFF){
    i = fileutil_displayinteger(input, "  Length might be an integer: ", &count); 
    printf("\n");
  }
  //}

  printf(format, i);
  for(; i != 0; i--){
    printf("%c", input[count++]);
  }

  *filep = count;
}

// Display a C++ serialized number
int fileutil_displaynumber(char *input, char *format, long long *filep){
  int i, read, readlocal;
  long long count = *filep;

  // Determine the length -- this matches C++ CArchive serialization
  i = fileutil_displayshort(input, "  Value might be a short: ", &count);
  printf("\n");
  if(i == 0xFFFF){
    i = fileutil_displayinteger(input, "  Value might be an integer: ", 
				&count);
    printf("\n");
    printf(format);
    printf("%d", i);
  }
  else{
    printf(format);
    printf("%d", i);
  }

  *filep = count;
  return i;
}

// Display a C++ serialized number
unsigned int fileutil_displayunumber(char *input, char *format, 
				     long long *filep){
  int i, read, readlocal;
  long long count = *filep;

  // Determine the length -- this matches C++ CArchive serialization
  i = fileutil_displayshort(input, "  Value might be a short: ", &count);
  printf("\n");
  if(i == 0xFFFF){
    i = fileutil_displayinteger(input, "  Value might be an integer: ", 
				&count);
    printf("\n");
    printf(format);
    printf("%u", (unsigned int) i);
  }
  else{
    printf(format);
    printf("%u", (unsigned int) i);
  }

  *filep = count;
  return i;
}

// Display a unicode string (slightly broken, it assumes double byte)
void fileutil_displayunicodestring(char *input, char *format, long long *filep){
  int i, read, readlocal;
  long long count = *filep;

  printf(format);
  while(input[count] != '\0'){
    printf("%c", input[count++], input[count]);
    count++;
  }

  // Skip over the NULL short at the end
  count += 2;
  printf(" [%d bytes]", count - *filep);

  *filep = count;
}

// Display an 8 byte long
long fileutil_displaylong(char *input, char *format, long long *filep){
  mint64 mylong;

  mylong.i = 0;
  printf(format);
  mylong.c[0] = input[*filep]; 
  mylong.c[1] = input[*filep + 1]; 
  mylong.c[2] = input[*filep + 2]; 
  mylong.c[3] = input[*filep + 3]; 
  mylong.c[4] = input[*filep + 4]; 
  mylong.c[5] = input[*filep + 5]; 
  mylong.c[6] = input[*filep + 6]; 
  mylong.c[7] = input[*filep + 7]; 

  printf("(long long) %d [8 bytes]", mylong.i);
  *filep += 8;
  return mylong.i;
}

// Display a 4 byte int
int fileutil_displayinteger(char *input, char *format, long long *filep){
  mint32 myint;

  myint.i = 0;
  printf(format);

  myint.c[0] = input[*filep]; 
  myint.c[1] = input[*filep + 1];
  myint.c[2] = input[*filep + 2]; 
  myint.c[3] = input[*filep + 3]; 
  *filep += 4;

  printf("(int) %d [4 bytes]", myint.i);
  return myint.i;
}

// Display a 4 byte unsigned int (DWORD)
unsigned int fileutil_displayuinteger(char *input, char *format, 
				      long long *filep){
  mint32 myint;

  myint.i = 0;
  printf(format);

  myint.c[0] = input[*filep]; 
  myint.c[1] = input[*filep + 1];
  myint.c[2] = input[*filep + 2]; 
  myint.c[3] = input[*filep + 3]; 
  *filep += 4;

  printf("(unsigned int) %u [4 bytes]", (unsigned int) myint.i);
  return myint.i;
}

// Display a short, 2 bytes
int fileutil_displayshort(char *input, char *format, long long *filep){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = input[*filep]; 
  myint.c[1] = input[*filep + 1]; 
  *filep += 2;

  printf("(short) %d [2 bytes]", myint.i);
  return myint.i;
}

// Display an unsigned short, 2 bytes
unsigned int fileutil_displayushort(char *input, char *format, long long *filep){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = input[*filep]; 
  myint.c[1] = input[*filep + 1]; 
  *filep += 2;

  printf("(unsigned short) %u [2 bytes]", (unsigned int) myint.i);
  return myint.i;
}

char *fileutil_displaybyteblock(char *input, char *format, int len, long long *filep){
  int count;
  unsigned char *result;
  long long consumed = *filep;

  printf("%s (%d bytes) ", format, len);
  if((result = (unsigned char *) malloc(sizeof(char) * len)) == NULL){
    fprintf(stderr, "Memory allocation error\n");
    return NULL;
  }

  for(count = 0; count < len; count++){
    result[count] = fileutil_displaybyte_actual(input, "", &consumed);
  }

  *filep = consumed;
  return result;
}

int fileutil_displaybyte(char *input, char *format, long long *filep){
  printf(format);
  return fileutil_displaybyte_actual(input, "(byte) ", filep);
}

// Display a byte (aka WORD)
int fileutil_displaybyte_actual(char *input, char *format, long long *filep){
  unsigned char byte;

  printf(format);
  byte = input[*filep]; 
  printf("%d", byte);
  *filep += 1;
  return byte;
}

// Display a language ID having made a half hearted attempt to decode it
int fileutil_displaywindowslanguage(char *input, char *format, long long *filep){
  mint32 myint;
  int index;

  myint.i = 0;
  printf(format);
  myint.c[0] = input[*filep]; 
  myint.c[1] = input[*filep + 1]; 
  myint.c[2] = input[*filep + 2]; 
  myint.c[3] = input[*filep + 3]; 
  *filep += 4;
  
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
char *fileutil_displayguid(char *input, char *format, long long *filep){
  long long count = *filep;

  printf(format);
  fileutil_displayinteger(input, "", &count);
  fileutil_displayshort(input, " ", &count);
  fileutil_displayshort(input, " ", &count);
  fileutil_displaybyteblock(input, " ", 8, &count);
  *filep = count;
}

// "Encoded integer", used by MS CHM
int fileutil_displayencinteger(char *input, char *format, int *read, long long *filep){
  int continued = 1, val, accval = 0, readcount = 0;
  unsigned char b;
  long long count = *filep;

  printf(format);
  printf("(encoded int) ");
  while(continued == 1){
    readcount++;

    b = input[count++];
    continued = b >> 7;
    val = b << 1;
    val = val >> 1;

    printf("[%d %d] ", continued, val);
    accval += val;
  }

  printf("= %d", accval);
  *read = readcount;
  *filep = count;
  return accval;
}

// Math all the directories we need to get to this path (assumes the last entry is the filename)
void mkpath(char *input){
  char path[1024], path2[1024];
  char *tok;
  int count;

  // Remove the last item as it is the name of a file
  strncpy(path, input, 1024); 
  count = strlen(path);
  while((count > 0) && (path[count] != '/'))
    count--;
  if(count != 0)
    path[count] = '\0';

  tok = (char *) strtok(path, "/");
  strcpy(path2, tok);

  while(1){
    mkdir(path2, 0);

    tok = (char *) strtok(NULL, "/");    
    if(tok == NULL) break;
    while(strlen(tok) == 0){
      if(tok == NULL) break;
      tok = (char *) strtok(NULL, "/");
    }
    strncat(path2, "/", 1024);
    strncat(path2, tok, 1024);
  }
}
