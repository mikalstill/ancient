// Verify that an ISBN number is correct (or at least that the checksum works)

#include <stdio.h>

int rebound(int u, int modulus){
  while(u < 0) u += modulus;
  while(u >= modulus) u -= modulus;
  return u;
}

int main(int argc, char *argv[]){
  int isbn[10], inset, total;

  /////////////////////////////////////////
  // Read in the ISBN code
  printf("Enter the digits of the ISBN code, one per line.\n");
  printf("A line with just a . will end this program\n\n");

  inset = 0;
  while(1){
    char line[100];

    fgets(line, 100, stdin);    
    if(line[0] == '.') break;
    if(line[0] == 'X')
      isbn[inset++] = 10;
    else
      isbn[inset++] = atoi(line);
    }

  if(inset != 10){
    printf("The ISBN number you entered was the wrong length\n");
    return 1;
  }

  /////////////////////////////////////////
  // Calculate the ISBN checksum
  printf("The ISBN code entered is: ");
  for(inset = 0, total = 0; inset < 10; inset++){
    printf("%d ", isbn[inset]);
    total += (inset + 1) * isbn[inset];
  }
  printf("\n");

  printf("Checksum: ");
  if(rebound(total, 11) == 0)
    printf("OK");
  else
    printf("Corruption detected");
  printf("\n");
}
