#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define TABLESIZE 100

void netmon ();
void cleanup (void);

pid_t pid;

typedef struct netflow_internal_conn
{
  char source[6];
  char destination[6];
  unsigned long kbps;
  unsigned long packets;
}
netflow_conn;

int
main (int argc, char **argv)
{
  switch (pid = fork ())
    {
    case -1:
      fprintf (stderr, "Fork error\n");
      exit (-1);

    case 0:
      netmon ();
      break;

    default:
      atexit (cleanup);
      sleep (1000);
      // ...
      break;
    }
}

void
cleanup ()
{
  printf ("Killing network monitor thread\n");
  kill (pid, SIGKILL);
  printf ("Done\n");
}

void
netmon ()
{
  int i;
  char *dev = "eth1";
  char errbuf[PCAP_ERRBUF_SIZE];
  char *time;
  pcap_t *descr;
  const u_char *packet;
  struct pcap_pkthdr hdr;	/* pcap.h */
  struct ether_header *eptr;	/* net/ethernet.h */
  long seconds;
  unsigned long transSize;
  netflow_conn connections[TABLESIZE];
  u_char *ptr;			/* printing out hardware header info */

  descr = pcap_open_live (dev, BUFSIZ, 0, -1, errbuf);
  if (descr == NULL)
    {
      printf ("pcap_open_live(): %s\n", errbuf);
      exit (1);
    }

  // Initialise the connections
  seconds = 0;
  for (i = 0; i < TABLESIZE; i++)
    {
      connections[i].kbps = 0;
      connections[i].packets = 0;
    }

  printf ("Listening on %s\n", dev);
  while (1)
    {
      printf("W");
      packet = pcap_next (descr, &hdr);
      if (packet == NULL)
	{
	  printf ("Didn't grab packet\n");
	  exit (1);
	}
      printf(".");

      if (seconds + 1 < hdr.ts.tv_sec)
	{
	  printf("--------------------------------------------------------\n");
	  for (i = 0; i < TABLESIZE; i++)
	    if (connections[i].kbps != 0)
	      {
		int divisor = 1;

		printf
		  ("%02x:%02x:%02x:%02x:%02x:%02x --> "
		   "%02x:%02x:%02x:%02x:%02x:%02x ", 
		   (unsigned char) connections[i].source[0],
		   (unsigned char) connections[i].source[1],
		   (unsigned char) connections[i].source[2],
		   (unsigned char) connections[i].source[3],
		   (unsigned char) connections[i].source[4],
		   (unsigned char) connections[i].source[5],
		   (unsigned char) connections[i].destination[0],
		   (unsigned char) connections[i].destination[1],
		   (unsigned char) connections[i].destination[2],
		   (unsigned char) connections[i].destination[3],
		   (unsigned char) connections[i].destination[4],
		   (unsigned char) connections[i].destination[5]);

		while(connections[i].kbps > divisor * 1024){
		  divisor *= 1024;
		  printf("D");
		}
		divisor /= 1024;
		printf("(%d ", connections[i].kbps / divisor);
		switch(divisor)
		  {
		  case 1: printf("b"); break;
		  case 1024: printf("k"); break;
		  case 1048576: printf("m"); break;
		  case 1073741824: printf("g"); break;
		  default: printf("?"); break;
		  }

		printf(", %d packets)\n",
		       connections[i].packets);

		connections[i].kbps = 0;
		connections[i].packets = 0;
	      }

	  seconds = hdr.ts.tv_sec;
	}

      /* lets start with the ether header... */
      eptr = (struct ether_header *) packet;
      transSize = hdr.len + 26;	
      for (i = 0; i < TABLESIZE; i++)
	{
	  if (memcmp (connections[i].source, eptr->ether_shost, 6) != 0)
	    continue;
	  if (memcmp (connections[i].destination, eptr->ether_dhost, 6) != 0)
	    continue;

	  // We can store the value here
	  connections[i].kbps += transSize;
	  ++connections[i].packets;
	  goto done;
	}

      // We didn't find a existing connection
      for (i = 0; i < TABLESIZE; i++)
	{
	  if (connections[i].kbps != 0)
	    continue;

	  // We can store the value here
	  memcpy (connections[i].source, eptr->ether_shost, 6);
	  memcpy (connections[i].destination, eptr->ether_dhost, 6);
	  connections[i].kbps += transSize;
	  ++connections[i].packets;
	  goto done;
	}

    done:
      printf("@");
    }
}
