#include <stdio.h>

void displayString(FILE *, char *);
int displayInteger(FILE *, char *);
int displayShort(FILE *, char *);

typedef union memopad_internal_mint32{
  int i;
  char c[3];
} mint32;

int main(int argc, char *argv[]){
  FILE *output;
  int c, i, fcnt;

  if((output = fopen("memopad.sample", "r")) == NULL){
    fprintf(stderr, "Could not open memopad.sample\n");
    exit(42);
  }

  /////////////////////////////////////////////////////////////////////////////
  // File header
  /////////////////////////////////////////////////////////////////////////////

  printf("Magic number: 0x%x 0x%x 0x%x 0x%x\n", 
	 fgetc(output), fgetc(output), fgetc(output), fgetc(output));
  displayString(output, "File path [%d chars]: ");
  displayString(output, "Custom show header [%d chars]: ");
  displayInteger(output, "Next free category id: ");
  i = displayInteger(output, "Number of categories minus one: ");
  
  for(; i != 0; i--){
    printf("Category\n");
    displayInteger(output, "  Index: ");
    displayInteger(output, "  Id: ");
    displayInteger(output, "  Dirty: ");
    displayString(output, "  Long name [%d chars]: ");
    displayString(output, "  Short name [%d chars]: ");
  }

  displayInteger(output, "Schema resource id: ");
  displayInteger(output, "Schema fields per row: ");
  displayInteger(output, "Schema record id position: ");
  displayInteger(output, "Schema record status position: ");
  displayInteger(output, "Schema record placement position: ");
  fcnt = i = displayShort(output, "Schema field count: ");

  printf("Schema fields\n");  
  for(; i != 0; i--){
    displayShort(output, "  Item: ");
  }

  i = displayInteger(output, "Record count * field count: ");
  i /= fcnt;

  for(; i != 0; i--){
    printf("Memopad entry\n");
    displayInteger(output, "  Recordid field type: ");
    displayInteger(output, "  Recordid: ");
    displayInteger(output, "  Status field type: ");
    displayInteger(output, "  Status: ");
    displayInteger(output, "  Position field type: ");
    displayInteger(output, "  Position: ");
    displayInteger(output, "  Memo text field type: ");
    displayInteger(output, "    [Pad]: ");
    displayString(output, "  Memo text [%d chars]:\n");
    displayInteger(output, "  Private field type: ");
    displayInteger(output, "  Private (1 = True): ");
    displayInteger(output, "  Category field type: ");
    displayInteger(output, "  Category: ");
  }
}

void displayString(FILE *output, char *format){
  int i;

  i = fgetc(output);
  printf(format, i);
  for(; i != 0; i--){
    printf("%c", fgetc(output));
  }
  printf("\n");
}

int displayInteger(FILE *output, char *format){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = fgetc(output);
  myint.c[1] = fgetc(output);
  myint.c[2] = fgetc(output);
  myint.c[3] = fgetc(output);

  printf("%d\n", myint.i);
  return myint.i;
}

int displayShort(FILE *output, char *format){
  mint32 myint;

  myint.i = 0;
  printf(format);
  myint.c[0] = fgetc(output);
  myint.c[1] = fgetc(output);

  printf("%d\n", myint.i);
  return myint.i;
}
