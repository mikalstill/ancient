#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#define TABLESIZE 100

typedef struct netflow_internal_conn
{
  char source[6];
  char destination[6];
  unsigned long kbps;
  unsigned long packets;
} netflow_conn;  

int
main (int argc, char **argv)
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

  /* grab a device to peak into... */
  //    dev = pcap_lookupdev(errbuf);

  if (dev == NULL)
    {
      printf ("%s\n", errbuf);
      exit (1);
    }

  descr = pcap_open_live (dev, BUFSIZ, 0, -1, errbuf);

  if (descr == NULL)
    {
      printf ("pcap_open_live(): %s\n", errbuf);
      exit (1);
    }

  // Initialise the connections
  seconds = 0;
  for(i = 0; i < TABLESIZE; i++){
    connections[i].kbps = 0;
    connections[i].packets = 0;
    }
  
  printf ("Listening on %s\n", dev);
  while(1){
    packet = pcap_next (descr, &hdr);
    
    if (packet == NULL)
      {				/* dinna work *sob* */
	printf ("Didn't grab packet\n");
	exit (1);
      }
    
    if(seconds + 10 < hdr.ts.tv_sec){
      printf("------------------------------------------------------------\n");
      for(i = 0; i < TABLESIZE; i++)
	if(connections[i].kbps != 0){
	  printf("%02x:%02x:%02x:%02x:%02x:%02x --> %02x:%02x:%02x:%02x:%02x:%02x (%d bytes, %d packets)\n",
		 (unsigned char) connections[i].source[0], (unsigned char) connections[i].source[1],
		 (unsigned char) connections[i].source[2], (unsigned char) connections[i].source[3],
		 (unsigned char) connections[i].source[4], (unsigned char) connections[i].source[5],
		 (unsigned char) connections[i].destination[0], (unsigned char) connections[i].destination[1],
		 (unsigned char) connections[i].destination[2], (unsigned char) connections[i].destination[3],
		 (unsigned char) connections[i].destination[4], (unsigned char) connections[i].destination[5],
		 connections[i].kbps, connections[i].packets);
	  connections[i].kbps = 0;
	  connections[i].packets = 0;
	}
      
      seconds =  hdr.ts.tv_sec;
    }

    /* lets start with the ether header... */
    eptr = (struct ether_header *) packet;
    ptr = eptr->ether_shost;
    i = ETHER_ADDR_LEN;
    do
      {
	//	printf ("%s%02x", (i == ETHER_ADDR_LEN) ? "" : ":", *ptr++);
      }
    while (--i > 0);
    //    printf(" to ");
    
    ptr = eptr->ether_dhost;
    i = ETHER_ADDR_LEN;
    do
      {
	//	printf ("%s%02x", (i == ETHER_ADDR_LEN) ? "" : ":", *ptr++);
      }
    while (--i > 0);
    
    //    printf(" %d bytes\n", hdr.len);
    transSize = hdr.len + 26; // ignores ethernet frame padding up to min len of 64

    for(i = 0; i < TABLESIZE; i++){
      if(memcmp(connections[i].source, eptr->ether_shost, 6) != 0)
	goto next;
      if(memcmp(connections[i].destination, eptr->ether_dhost, 6) != 0)
	goto next;
      
      // We can store the value here
      connections[i].kbps += transSize;
      ++connections[i].packets;
      goto done;
      
      // Ugly but efficient
    next:
    }
    
    // We didn't find a existing connection
    for(i = 0; i < TABLESIZE; i++){
      if(connections[i].kbps != 0)
	goto newnext;
      
      // We can store the value here
      memcpy(connections[i].source, eptr->ether_shost, 6);
      memcpy(connections[i].destination, eptr->ether_dhost, 6);
      connections[i].kbps += transSize;
      ++connections[i].packets;
      goto done;
      
      // Ugly but efficient
    newnext:
    }
    
    // Ugly but efficient
  done:
  }
}
