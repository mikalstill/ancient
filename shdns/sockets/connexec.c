// Connected UDP socket example: this example this 
// example simply waits for traffic, and the starts 
// a process to deal with the results. One process 
// per packet, one packet per process. You'll note 
// that we can now use read and write to get to the 
// traffic, and that this all works...

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main (int argc, char *argv[])
{
  int lfd;
  struct sockaddr_in servaddr;
  struct sockaddr clientaddr;
  char buf[1024];
  size_t len;
  socklen_t clen;

  // We will listen with this file descriptor
  if ((lfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      fprintf (stderr, 
	       "Error whilst starting to listen\n");
      exit (42);
    }

  // Define what we are listening for
  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
  servaddr.sin_port = htons (1234);

  // Bind to the address
  if (bind (lfd, (struct sockaddr *) &servaddr, 
	    sizeof (servaddr)) < 0)
    {
      perror ("Couldn't bind");
      exit (42);
    }

  // Do stuff
  while (1)
    {
      // We need to peek at the first part of the 
      // packet to determine who to connect to
      len = 1;
      printf ("Reading...\n");
      clen = sizeof (clientaddr);
      if ((len = recvfrom (lfd, buf, len, MSG_PEEK,
			   (struct sockaddr *) 
			   &clientaddr, &clen)) < 0)
	{
	  perror ("Socket peek error");
	  exit (42);
	}
      if (len == 0)
	break;

      // Connect
      if (connect (lfd, &clientaddr, clen) < 0)
	{
	  perror ("Could not connect");
	  exit (42);
	}

      printf ("Data arrived\n");

      // Spawn a child to handle this packet
      switch (fork ())
	{
	case -1:
	  perror ("Couldn't spawn child to handle connection");
	  exit (42);

	case 0:
	  // Child process -- setup the file 
	  // descriptors, and the run the helper 
	  // application
	  dup2 (lfd, 0);
	  dup2 (lfd, 1);

	  execl ("/bin/cat", "cat", NULL);
	  perror ("Exec failed");
	  exit (42);
	  break;

	default:
	  // Parent process
	  break;
	}
    }
}
