#include <stdio.h>

int main(int argc, char *argv[]){
  FILE *output;

  if((output = fopen("memopad.dat", "w")) == NULL){
    fprintf(stderr, "Could not create memopad.dat\n");
    exit(42);
  }

  /////////////////////////////////////////////////////////////////////////////
  // File header
  /////////////////////////////////////////////////////////////////////////////

  // Version
  fprintf(output, "%c%c%c%c%c", 0x00, 0x01, 0x50, 0x4d, 0x21);
  
  // Filename
  fprintf(output, "%s%c", "c:\\palm\\mikal\\memopad\\memopad.dat", 0x0);

  // This is a custom show header... Whatever that is
  fprintf(output, "0 1 0 0 1 0X%c", 0x0);

  // 
}
