// Disconnected UDP socket example: this example simply waits for 
// traffic, and the starts a process to deal with the results. One
// process per packet, one packet per process. This version wont 
// work, because the socket is not connected. In fact, cat is 
// smart enough to warn us about this:
//
//           cat: write error: Transport endpoint is not connected

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
  int lfd;
  struct sockaddr_in servaddr;
  struct pollfd pfd;
  
  // We will listen with this file descriptor
  if((lfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    fprintf(stderr, "Error whilst starting to listen\n");
    exit(42);
  }

  // Define what we are listening for
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(1234);

  // Bind to the address
  if(bind(lfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) 
     < 0){
    perror("Couldn't bind");
    exit(42);
  }

  // Setup the list of file descriptors we want to wait for 
  // events on
  pfd.fd = lfd;
  pfd.events = POLLIN | POLLPRI;
  
  // Do stuff
  while(1){
    if(poll(&pfd, 1, -1) < 0){
      perror("Waiting for new data failed");
      exit(42);
    }

    printf("Data arrived\n");

    // Spawn a child to handle this packet
    switch(fork()){
    case -1:
      perror("Couldn't spawn child to handle connection");
      exit(42);
      
    case 0:
      // Child process -- setup the file descriptors, and the run 
      // the helper application
      dup2(lfd, 0);
      dup2(lfd, 1);

      execl("/bin/cat", "cat", NULL);
      perror("Exec failed");
      exit(42);
      break;

    default:
      // Parent process
      break;
    }
  }
}
