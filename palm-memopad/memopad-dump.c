#include <stdio.h>
#include "fileutil.h"

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
  fileutil_displaystring(input, "File path [%d chars]: ");
  fileutil_displaystring(input, "Custom show header [%d chars]: ");
  fileutil_displayinteger(input, "Next free category id: ");
  i = fileutil_displayinteger(input, "Number of categories minus one: ");
  
  for(; i != 0; i--){
    printf("Category\n");
    fileutil_displayinteger(input, "  Index: ");
    fileutil_displayinteger(input, "  Id: ");
    fileutil_displayinteger(input, "  Dirty: ");
    fileutil_displaystring(input, "  Long name [%d chars]: ");
    fileutil_displaystring(input, "  Short name [%d chars]: ");
  }

  fileutil_displayinteger(input, "Schema resource id: ");
  fileutil_displayinteger(input, "Schema fields per row: ");
  fileutil_displayinteger(input, "Schema record id position: ");
  fileutil_displayinteger(input, "Schema record status position: ");
  fileutil_displayinteger(input, "Schema record placement position: ");
  fcnt = i = fileutil_displayshort(input, "Schema field count: ");

  printf("Schema fields\n");  
  for(; i != 0; i--){
    fileutil_displayshort(input, "  Item: ");
  }

  i = fileutil_displayinteger(input, "Record count * field count: ");
  i /= fcnt;

  for(; i != 0; i--){
    printf("Memopad entry\n");
    fileutil_displayinteger(input, "  Recordid field type: ");
    fileutil_displayinteger(input, "  Recordid: ");
    fileutil_displayinteger(input, "  Status field type: ");
    fileutil_displayinteger(input, "  Status: ");
    fileutil_displayinteger(input, "  Position field type: ");
    fileutil_displayinteger(input, "  Position: ");
    fileutil_displayinteger(input, "  Memo text field type: ");
    fileutil_displayinteger(input, "    [Pad]: ");
    fileutil_displaystring(input, "  Memo text [%d chars]:\n");
    fileutil_displayinteger(input, "  Private field type: ");
    fileutil_displayinteger(input, "  Private (1 = True): ");
    fileutil_displayinteger(input, "  Category field type: ");
    fileutil_displayinteger(input, "  Category: ");
  }
}
