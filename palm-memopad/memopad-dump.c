#include <stdio.h>
#include "memopad.h"

void memopad_displaystring(FILE *, char *);
int memopad_displayinteger(FILE *, char *);
int memopad_displayshort(FILE *, char *);

int main(int argc, char *argv[]){
  FILE *input;
  int c, i, fcnt;

  if(argc < 2){
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    exit(43);
  }

  if((input = fopen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open %s\n", argv[1]);
    exit(42);
  }

  /////////////////////////////////////////////////////////////////////////////
  // File header
  /////////////////////////////////////////////////////////////////////////////

  printf("Magic number: 0x%x 0x%x 0x%x 0x%x\n", 
	 fgetc(input), fgetc(input), fgetc(input), fgetc(input));
  memopad_displaystring(input, "File path [%d chars]: ");
  memopad_displaystring(input, "Custom show header [%d chars]: ");
  memopad_displayinteger(input, "Next free category id: ");
  i = memopad_displayinteger(input, "Number of categories minus one: ");
  
  for(; i != 0; i--){
    printf("Category\n");
    memopad_displayinteger(input, "  Index: ");
    memopad_displayinteger(input, "  Id: ");
    memopad_displayinteger(input, "  Dirty: ");
    memopad_displaystring(input, "  Long name [%d chars]: ");
    memopad_displaystring(input, "  Short name [%d chars]: ");
  }

  memopad_displayinteger(input, "Schema resource id: ");
  memopad_displayinteger(input, "Schema fields per row: ");
  memopad_displayinteger(input, "Schema record id position: ");
  memopad_displayinteger(input, "Schema record status position: ");
  memopad_displayinteger(input, "Schema record placement position: ");
  fcnt = i = memopad_displayshort(input, "Schema field count: ");

  printf("Schema fields\n");  
  for(; i != 0; i--){
    memopad_displayshort(input, "  Item: ");
  }

  i = memopad_displayinteger(input, "Record count * field count: ");
  i /= fcnt;

  for(; i != 0; i--){
    printf("Memopad entry\n");
    memopad_displayinteger(input, "  Recordid field type: ");
    memopad_displayinteger(input, "  Recordid: ");
    memopad_displayinteger(input, "  Status field type: ");
    memopad_displayinteger(input, "  Status: ");
    memopad_displayinteger(input, "  Position field type: ");
    memopad_displayinteger(input, "  Position: ");
    memopad_displayinteger(input, "  Memo text field type: ");
    memopad_displayinteger(input, "    [Pad]: ");
    memopad_displaystring(input, "  Memo text [%d chars]:\n");
    memopad_displayinteger(input, "  Private field type: ");
    memopad_displayinteger(input, "  Private (1 = True): ");
    memopad_displayinteger(input, "  Category field type: ");
    memopad_displayinteger(input, "  Category: ");
  }
}

void memopad_displaystring(FILE *input, char *format){
  int i;

  i = fgetc(input);
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
