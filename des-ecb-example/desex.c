/******************************************************************************
  desex.c, an example of how to use the openssl DES implementation... Data
  is input on stdin, and the enrypted information is output to stdout. Prompts
  appear on stderr...

  This is an example of the Electronic Code Book mode, which is predictable
  for known input, and is not recommended.

  Copyright (c) Michael Still 2001
  Released under the terms of the GNU GPL
******************************************************************************/

#include <stdio.h>
#include <openssl/des.h>

int main(int argc, char *argv[]){
  des_cblock key, input, output;
  des_key_schedule sched;
  int c, i;

  fprintf(stderr, "Setting up the DES library...\n");
  des_string_to_key("Mary had a little lamb, it's fleece as white as snow. Everywhere that Mary went, the lamb would surely go...", &key);

  fprintf(stderr, "Schedualling the key...\n");
  switch(des_set_key_checked(&key, sched)){
  case -1:
    fprintf(stderr, "Bad parity\n");
    _exit(42);
    break;

  case -2:
    fprintf(stderr, "Key is weak\n");
    _exit(42);
    break;
  }

  fprintf(stderr, "Start typing and hit ctrl-d to finish...\n");
  
  i = 0;
  while((c = fgetc(stdin)) != EOF){
    input[i] = c;

    i++;
    if(i == 8){
      des_ecb_encrypt(&input, &output, sched, DES_ENCRYPT);
      fprintf(stdout, "%c%c%c%c%c%c%c%c", output[0], output[1], output[2],
	      output[3], output[4], output[5], output[6], output[7]);
      i = 0;
    }
  }
      
  //  fprintf(stderr, "Check we can decrypt...\n");
  //  des_ecb_encrypt(&output, &input, sched, DES_DECRYPT);
  //  printf("Clear: ");
  //  for(c = 0; c < 8; c++)
  //    printf("%c", input[c]);
  //  printf("\n");
}
  



