// FInd the missing digit from an ISBN number. If a modulus other than the normal
// 11 is desired, then specify it on the command line...

#include <stdio.h>

int rebound(int u, int modulus){
  while(u < 0) u += modulus;
  while(u >= modulus) u -= modulus;
  return u;
}

// Find the multiplicative inverse of u, based on the given modulus
int multinverse(int u, int modulus){
  int count;

  for(count = 1; count < modulus; count++){
    if(rebound(count * u, modulus) == 1)
      return count;
  }

  return -1;
}

int main(int argc, char *argv[]){
  int isbn[10], inset, total, unknownpos, mi, mod;

  /////////////////////////////////////////
  // Determine what modulus to use
  if(argc > 1)
    mod = atoi(argv[1]);
  else
    mod = 11;
  printf("Working modulus %d\n\n", mod);

  /////////////////////////////////////////
  // Read in the ISBN code
  printf("Enter the digits of the ISBN code, one per line.\n");
  printf("Use a ? to mark the number which is missing...\n");
  printf("A line with just a . will end this program\n\n");

  inset = 0;
  while(1){
    char line[100];

    fgets(line, 100, stdin);    
    if(line[0] == '.') break;
    if(line[0] == 'X')
      isbn[inset++] = 10;
    else if(line[0] == '?')
      isbn[inset++] = -1;
    else
      isbn[inset++] = atoi(line);
    }

  if(inset != 10){
    printf("The ISBN number you entered was the wrong length (%d)\n", inset);
    return 1;
  }

  /////////////////////////////////////////
  // Calculate the ISBN checksum
  printf("The ISBN code entered is: ");
  for(inset = 0, total = 0, unknownpos = -1; inset < 10; inset++){
    if(isbn[inset] != -1){
      printf("%d ", isbn[inset]);
      total += (inset + 1 ) * isbn[inset];
    }
    else{
      if(unknownpos != -1){
	printf("\nToo many unknowns\n");
	return 1;
      }

      printf("? ");
      unknownpos = inset;
    }
  }
  printf("\n\n");

  /////////////////////////////////////////
  // Determine the value which statisfies the congruence
  printf("Sum before modulus = %d\n", total);
  total = rebound(total, mod);
  printf("Sum after modulus = %d\n", total);
  printf("Value of missing element, including coefficient = %d\n",
	 rebound(0 - total, mod));
  printf("Coefficient of missing element = %d\n", unknownpos + 1);

  mi = multinverse(unknownpos + 1, mod);
  if(mi == -1){
    printf("No multiplicative inverse for %d found with modulus %d\n",
	   unknownpos + 1, mod);
    return 1;
  }
  printf("Multiplicative inverse of this coefficient = %d\n", mi);
  printf("Unknown element = %d\n", rebound(total * mi, mod));
}
