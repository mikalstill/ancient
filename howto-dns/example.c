// Demonstrates how to lookup a name via the system DNS setup
//
// Copyright (c) Michael Still 2003, released under the terms of the GNU GPL

#include <stdio.h>
#include <netdb.h>
#include <errno.h>

int main(int argc, char *argv[]){
  struct hostent *result;
  int i, j;

  if(argc < 2){
    fprintf(stderr, "Usage: %s <dns name to resolve>\n", argv[0]);
    exit(43);
  }

  result = gethostbyname(argv[1]);
  if(result == NULL)
    perror("Name lookup failed: ");
  else{
    printf("Name resolved to: %s (%d name elements)\n", result->h_name,
	   result->h_length);
    j = 0;
    while(result->h_addr_list[j] != 0){
      printf("  ");
      for(i = 0; i < result->h_length; i++){
	printf("%d ", (unsigned char) result->h_addr_list[j][i]);
      }
      printf("\n");
      j++;
    }
  }
}
