// Connected UDP socket example: this example simply reads from clients (there can be more 
// than one), and returns what they said straight back to them. You'll note that we can now use 
// read and write to get to the traffic...

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
  int lfd;
  struct sockaddr_in servaddr;
  struct sockaddr clientaddr;
  char buf[1024];
  size_t len;
  socklen_t clen;
  
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
  if(bind(lfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
    perror("Couldn't bind");
    exit(42);
  }
  
  // Do stuff
  while(1){
    // We need to peek at the first part of the packet to determine who to connect to
    len = 1;
    printf("Reading...\n");
    clen = sizeof(clientaddr);
    if((len = recvfrom(lfd, buf, len, MSG_PEEK, (struct sockaddr *) &clientaddr, 
		       &clen)) < 0){
      perror("Socket peek error");
      exit(42);
    }
    if(len == 0) break;

    // Connect
    if(connect(lfd, &clientaddr, clen) < 0){
      perror("Could not connect");
      exit(42);
    }

    // And now we can just use the normal read and write
    len = 1024;
    if((len = read(lfd, buf, len)) < 0){
      perror("Socket read error");
      exit(42);
    }
    if(write(lfd, buf, len) < 0){
      perror("Socket write error");
      exit(42);
    }
  }
}
