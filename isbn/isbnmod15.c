// Find numbers which can be swapped in a mod 15 ISBN code
#include <stdio.h>

int rebound(int u, int modulus){
  while(u < 0) u += modulus;
  while(u >= modulus) u -= modulus;
  return u;
}

int checksum(int isbn[]){
  int total, inset;

  for(inset = 0, total = 0; inset < 10; inset++){
    total += (inset + 1) * isbn[inset];
  }

  return (rebound(total, 15) == 0) ? 0 : -1; 
}

int main(int argc, char *argv[]){
  int isbn[10], newisbn[10], inset, total, count1, count2;

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
  }
  printf("\n");
  printf("Checksum: %s\n", checksum(isbn) == 0 ? "OK" : "Invalid");

  for(count1 = 0; count1 < 10; count1++){
    for(count2 = 0; count2 < 10; count2++){
      int temp;
      
      if(count1 != count2){
	memcpy(newisbn, isbn, sizeof(int) * 10);
	temp = newisbn[count1];
	newisbn[count1] = newisbn[count2];
	newisbn[count2] = temp;
	
	if(checksum(newisbn) == 0){
	  printf("Swapping element %d and element %d worked\n",
		 count1 + 1, count2 + 1);
	}
      }
    }
  }
}
