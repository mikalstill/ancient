// Implements a simple (and very broken) cryptographic algorithm based on
// the congruence that y = ax + b (mod 26)      using = to mean "congruent"
// A thru Z are mapped to 0 thru 25 -- there is no punctuation nor lowercase

#include <stdio.h>

int rebound(int u){
  while(u < 0) u += 26;
  while(u >= 26) u -= 26;
  return u;
}

int main(int argc, char *argv[]){
  int *cipher, *plain;
  int cipherlen = 2, ciphercount = 0, freq[26], count, fmost, fnext, u, v;

  if((cipher = (int *) malloc(cipherlen * sizeof(int))) == NULL){
    printf("Could not allocate memory\n");
    return 1;
  }

  // Initialise the frequencies
  for(count = 0; count < 26; count++)
    freq[count] = 0;

  /////////////////////////////////////////
  // Read in the encrypted message
  printf("Enter the encrypted message as a series of numbers, one per line.\n");
  printf("A line with just a . will end this program\n\n");
  while(1){
    char line[100];
    int now;

    fgets(line, 100, stdin);    
    if(line[0] == '.') break;
    now = atoi(line);

    if(ciphercount > cipherlen){
      cipherlen *= 2;
      if((cipher = (int *) realloc(cipher, cipherlen * sizeof(int))) == NULL){
	printf("Could not reallocate memory\n");
	return 1;
      }
    }

    freq[now]++;
    cipher[ciphercount++] = now;
  }

  /////////////////////////////////////////
  // Determine the most frequent
  for(count = 0, fmost = 0; count < 26; count++){
    if(freq[count] > freq[fmost]) fmost = count; 
  }
  printf("Most frequent is %d with %d appearances\n", fmost, freq[fmost]);

  // Determine the next most frequent
  for(count = 0, fnext = 0; count < 26; count++){
    if((freq[count] > freq[fnext]) && (count != fmost)) fnext = count; 
  }
  printf("Next most frequent is %d with %d appearances\n\n", fnext, freq[fnext]);

  /////////////////////////////////////////
  // Determine the congruence coefficients -- u
  printf("Solve: %d = %du + v (mod 26)\n", 'E' - 'A', fmost);
  printf("       %d = %du + v (mod 26)\t\t\t= meaning congruent\n\n", 
	 'T' - 'A', fnext);

  // This doesn't handle fmost - fnext being non integer
  printf("       u = (%d - %d) / (%d - %d)\n", 'T' - 'A', 'E' - 'A',
	 fnext, fmost);
  printf("         = 15 / %d\n", (fnext - fmost));
  u = 15 / (fnext - fmost);
  printf("         = %d\n", u);
  u = rebound(u);
  printf("         = %d\n\n", u);
  
  /////////////////////////////////////////
  // Determine the congruence coefficients -- v
  printf("       v = %d - %d * %d\n", 'E' - 'A', fmost, u);
  v = ('E' - 'A') - (fmost * u);
  printf("         = %d\n", v);
  u = rebound(u);
  printf("         = %d\n\n", v);
    
  /////////////////////////////////////////
  // Repeat back the input
  for(count = 0; count < ciphercount; count++){
    printf("%02d ", cipher[count]);
  }
  printf("\n");

  for(count = 0; count < ciphercount; count++){
    if(cipher[count] == fmost) printf("## ");
    else if(cipher[count] == fnext) printf(".. ");
    else printf("   ");
  }
  printf("\n");

  // Decrypt
  if((plain = (int *) malloc(ciphercount * sizeof(int))) == NULL){
    printf("Could not allocate memory\n");
    return 1;
  }

  for(count = 0; count < ciphercount; count++){
    int now = u * cipher[count] + v;
    u = rebound(u);
    plain[count] = now;
    printf("%02d ", now);
  }
  printf("\n");

  for(count = 0; count < ciphercount; count++){
    printf("%c  ", plain[count] + 'A');
  }
  printf("\n");

  return 0;
}
