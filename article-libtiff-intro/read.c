#include <stdio.h>
#include <tiffio.h>

int main(int argc, char *argv[]){
  // Define an image -- this is 32 pixels by 9 pixels

  // 12345678 12345678 12345678 12345678
  // 00000000 00000000 00000000 00000000
  // 01100000 00011111 11111000 00000110
  // 01100000 11100000 00000111 00000110
  // 01100111 00000000 00000000 11100110
  // 01111000 00000000 00000000 00011110
  // 01100000 00000000 00000000 00000110
  // 01111111 11111111 11111111 11111110
  // 01100000 00000000 00000000 00000110
  // 00000000 00000000 00000000 00000000
  char buffer[32 * 9] = {0x00, 0x00, 0x00, 0x00,
			 0x60, 0x1F, 0xF8, 0x06,
			 0x60, 0xD0, 0x07, 0x06,
			 0x67, 0x00, 0x00, 0xD6,
			 0x7};



}
