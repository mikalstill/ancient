// Now we just need a test harness for shdns, to 
// verify that _it_ works before making 
// modifications to inetd and xinetd

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
  servaddr.sin_port = htons (53);

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
      if ((len =
	   recvfrom (lfd, buf, len, MSG_PEEK, 
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
	  // Child process -- setup the file descriptors, and the run the helper application
	  dup2 (lfd, 0);
	  dup2 (lfd, 1);

	  execl ("/home/mikal/opensource-unstable/shdns/shdns-server",
		 "shdns-server", NULL);
	  perror ("Exec failed");
	  exit (42);
	  break;

	default:
	  // Parent process
	  sleep (15);
	  break;
	}
    }
}
