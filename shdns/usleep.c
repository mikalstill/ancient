#include <unistd.h>

int main(int argc, char *argv[]){
  if(argc > 1)
    return usleep(atol(argv[1]));
}
