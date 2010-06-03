/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 *
 * Author: Guido Socher 
 * Author: Michael Still (mikal@stillhq.com)
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * IP, Arp, UDP and TCP functions.
 *
 * The TCP implementation uses some size optimisations which are valid
 * only if all data can be sent in one single packet. This is however
 * not a big limitation for a microcontroller as you will anyhow use
 * small web-pages. The web server must send the entire web page in one
 * packet.
 *
 * Note that this is a significantly cut down and refactored version
 * of the original nuelectronics code.
 *
 * Chip type           : ATMEGA88 with ENC28J60
 *********************************************/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include "net.h"
#include "enc28j60.h"
#include "ip_config.h"

static uint8_t macaddr[6];
static uint8_t ipaddr[4];
static int16_t info_hdr_len=0;
static uint16_t info_data_len=0;
static uint8_t seqnum = 0xa; // my initial tcp sequence number

#define HTTP_HEADER_START ((uint16_t)TCP_SRC_PORT_H_P+(buf[TCP_HEADER_LEN_P]>>4)*4)
const char arpreqhdr[] PROGMEM ={0,1,8,0,6,4,0,1};

// The IP checksum is calculated over the ip header only starting
// with the header length field and a total length of 20 bytes
// unitl ip.dst
// You must set the IP checksum field to zero before you start
// the calculation.
// len for ip is 20.
//
// For UDP/TCP we do not make up the required pseudo header. Instead we 
// use the ip.src and ip.dst fields of the real packet:
// The udp checksum calculation starts with the ip.src field
// Ip.src=4bytes,Ip.dst=4 bytes,Udp header=8bytes + data length=16+len
// In other words the len here is 8 + length over which you actually
// want to calculate the checksum.
// You must set the checksum field to zero before you start
// the calculation.
// The same algorithm is also used for udp and tcp checksums.
// len for udp is: 8 + 8 + data length
// len for tcp is: 4+4 + 20 + option len + data length
//
// For more information on how this algorithm works see:
// http://www.netfor2.com/checksum.html
// http://www.msc.uky.edu/ken/cs471/notes/chap3.htm
// The RFC has also a C code example: http://www.faqs.org/rfcs/rfc1071.html
uint16_t checksum(uint8_t *buf, uint16_t len,uint8_t type){
  // type 0=ip , icmp
  //      1=udp
  //      2=tcp
  uint32_t sum = 0;

  if(type==1){
    sum+=IP_PROTO_UDP_V; // protocol udp
    // the length here is the length of udp (data+header len)
    // =length given to this function - (IP.scr+IP.dst length)
    sum+=len-8; // = real udp len
  }
  if(type==2){
    sum+=IP_PROTO_TCP_V; 
    // the length here is the length of tcp (data+header len)
    // =length given to this function - (IP.scr+IP.dst length)
    sum+=len-8; // = real tcp len
  }

  // build the sum of 16bit words
  while(len >1){
    sum += 0xFFFF & (((uint32_t)*buf<<8)|*(buf+1));
    buf+=2;
    len-=2;
  }

  // if there is a byte left then add it (padded with zero)
  if (len){
    sum += ((uint32_t)(0xFF & *buf))<<8;
  }

  // now calculate the sum over the bytes in the sum
  // until the result is only 16bit long
  while (sum>>16){
    sum = (sum & 0xFFFF)+(sum >> 16);
  }

  // build 1's complement:
  return( (uint16_t) sum ^ 0xFFFF);
}

// This initializes the TCP functionality.
void init_ip_arp_udp_tcp(uint8_t *mymac, uint8_t *myip){
  memcpy(ipaddr, myip, 4);
  memcpy(macaddr, mymac, 6);  
}

// Is this IP at this location?
uint8_t check_ip(uint8_t *buf, int location, uint8_t *ip)
{
  if(memcmp(buf + location, ip, 4) == 0) return 1;
  return 0;
}

// Is this an ARP packet intended for me?
uint8_t eth_type_is_arp_and_my_ip(uint8_t *buf,uint16_t len){
  if(len < 41) return 0;
  if(buf[ETH_TYPE_H_P] != ETHTYPE_ARP_H_V || 
     buf[ETH_TYPE_L_P] != ETHTYPE_ARP_L_V) return 0;
  return check_ip(buf, ETH_ARP_DST_IP_P, ipaddr);
}

// Is this an IP packet intended for me? Must be IPv4 with a 20 byte header
uint8_t eth_type_is_ip_and_my_ip(uint8_t *buf,uint16_t len){
  if(len < 42) return 0;
  if(buf[ETH_TYPE_H_P]!=ETHTYPE_IP_H_V ||
     buf[ETH_TYPE_L_P]!=ETHTYPE_IP_L_V) return 0;
  if (buf[IP_HEADER_LEN_VER_P] != 0x45) return 0;
  return check_ip(buf, IP_DST_P, ipaddr);
}

// Is it an arp reply (no len check here, you must first call eth_type_is_arp_and_my_ip)
uint8_t eth_type_is_arp_reply(uint8_t *buf){
  return (buf[ETH_ARP_OPCODE_L_P] == ETH_ARP_OPCODE_REPLY_L_V);
}

// Is it an arp request (no len check here, you must first call eth_type_is_arp_and_my_ip)
uint8_t eth_type_is_arp_req(uint8_t *buf){
  return (buf[ETH_ARP_OPCODE_L_P] == ETH_ARP_OPCODE_REQ_L_V);
}

// Make a return eth header from a received eth packet
void make_eth(uint8_t *buf)
{
  memcpy(buf + ETH_DST_MAC, buf + ETH_SRC_MAC, 6);
  memcpy(buf + ETH_SRC_MAC, macaddr, 6);
}

// Fill the checksum field of the IP header
void fill_ip_hdr_checksum(uint8_t *buf)
{
  uint16_t ck;

  // clear the 2 byte checksum
  buf[IP_CHECKSUM_P] = 0;
  buf[IP_CHECKSUM_P + 1] = 0;
  buf[IP_FLAGS_P] = 0x40; // don't fragment
  buf[IP_FLAGS_P + 1] = 0;  // fragement offset
  buf[IP_TTL_P] = 64; // ttl

  // calculate the checksum:
  ck = checksum(&buf[IP_P], IP_HEADER_LEN, 0);
  buf[IP_CHECKSUM_P] = ck >> 8;
  buf[IP_CHECKSUM_P + 1] = ck & 0xff;
}

// Make a return ip header from a received ip packet
void make_ip(uint8_t *buf)
{
  memcpy(buf + IP_DST_P, buf + IP_SRC_P, 4);
  memcpy(buf + IP_SRC_P, ipaddr, 4);
  fill_ip_hdr_checksum(buf);
}

// Swap seq and ack number and count ack number up
void step_seq(uint8_t *buf,uint16_t rel_ack_num,uint8_t cp_seq)
{
        uint8_t i;
        uint8_t tseq;
        i=4;
        // sequence numbers:
        // add the rel ack num to SEQACK
        while(i>0){
                rel_ack_num=buf[TCP_SEQ_H_P+i-1]+rel_ack_num;
                tseq=buf[TCP_SEQACK_H_P+i-1];
                buf[TCP_SEQACK_H_P+i-1]=0xff&rel_ack_num;
                if (cp_seq){
                        // copy the acknum sent to us into the sequence number
                        buf[TCP_SEQ_H_P+i-1]=tseq;
                }else{
                        buf[TCP_SEQ_H_P+i-1]= 0; // some preset vallue
                }
                rel_ack_num=rel_ack_num>>8;
                i--;
        }
}

// make a return tcp header from a received tcp packet
// rel_ack_num is how much we must step the seq number received from the
// other side. We do not send more than 765 bytes of text (=data) in the tcp packet.
// No mss is included here.
//
// After calling this function you can fill in the first data byte at TCP_OPTIONS_P+4
// If cp_seq=0 then an initial sequence number is used (should be use in synack)
// otherwise it is copied from the packet we received
void make_tcphead(uint8_t *buf,uint16_t rel_ack_num,uint8_t cp_seq)
{
        uint8_t i;
        // copy ports:
        i=buf[TCP_DST_PORT_H_P];
        buf[TCP_DST_PORT_H_P]=buf[TCP_SRC_PORT_H_P];
        buf[TCP_SRC_PORT_H_P]=i;
        //
        i=buf[TCP_DST_PORT_L_P];
        buf[TCP_DST_PORT_L_P]=buf[TCP_SRC_PORT_L_P];
        buf[TCP_SRC_PORT_L_P]=i;
        step_seq(buf,rel_ack_num,cp_seq);
        // zero the checksum
        buf[TCP_CHECKSUM_H_P]=0;
        buf[TCP_CHECKSUM_L_P]=0;
        // no options:
        // 20 bytes:
        // The tcp header length is only a 4 bit field (the upper 4 bits).
        // It is calculated in units of 4 bytes.
        // E.g 20 bytes: 20/4=6 => 0x50=header len field
        buf[TCP_HEADER_LEN_P]=0x50;
}

void make_arp_answer_from_request(uint8_t *buf)
{
  make_eth(buf);
  buf[ETH_ARP_OPCODE_H_P]=ETH_ARP_OPCODE_REPLY_H_V;
  buf[ETH_ARP_OPCODE_L_P]=ETH_ARP_OPCODE_REPLY_L_V;
  memcpy(buf + ETH_ARP_DST_MAC_P, buf + ETH_ARP_SRC_MAC_P, 6);
  memcpy(buf + ETH_ARP_SRC_MAC_P, macaddr, 6);
  memcpy(buf + ETH_ARP_DST_IP_P, buf + ETH_ARP_SRC_IP_P, 4);
  memcpy(buf + ETH_ARP_SRC_IP_P, ipaddr, 4);
  enc28j60PacketSend(42, buf); 
}

void make_echo_reply_from_request(uint8_t *buf,uint16_t len)
{
  make_eth(buf);
  make_ip(buf);
  buf[ICMP_TYPE_P] = ICMP_TYPE_ECHOREPLY_V;
  
  // we changed only the icmp.type field from request(=8) to reply(=0).
  // we can therefore easily correct the checksum:
  if (buf[ICMP_CHECKSUM_P] > (0xff - 0x08)){
    buf[ICMP_CHECKSUM_P+1]++;
  }
  buf[ICMP_CHECKSUM_P] += 0x08;
  enc28j60PacketSend(len,buf);
}

// you can send a max of 220 bytes of data
void make_udp_reply_from_request(uint8_t *buf,char *data,uint8_t datalen,uint16_t port)
{
        uint8_t i=0;
        uint16_t ck;
        make_eth(buf);
        if (datalen>220){
                datalen=220;
        }
        // total length field in the IP header must be set:
        buf[IP_TOTLEN_H_P]=0;
        buf[IP_TOTLEN_L_P]=IP_HEADER_LEN+UDP_HEADER_LEN+datalen;
        make_ip(buf);
        // send to port:
        //buf[UDP_DST_PORT_H_P]=port>>8;
        //buf[UDP_DST_PORT_L_P]=port & 0xff;
        // sent to port of sender and use "port" as own source:
        buf[UDP_DST_PORT_H_P]=buf[UDP_SRC_PORT_H_P];
        buf[UDP_DST_PORT_L_P]= buf[UDP_SRC_PORT_L_P];
        buf[UDP_SRC_PORT_H_P]=port>>8;
        buf[UDP_SRC_PORT_L_P]=port & 0xff;
        // calculte the udp length:
        buf[UDP_LEN_H_P]=0;
        buf[UDP_LEN_L_P]=UDP_HEADER_LEN+datalen;
        // zero the checksum
        buf[UDP_CHECKSUM_H_P]=0;
        buf[UDP_CHECKSUM_L_P]=0;
        // copy the data:
        while(i<datalen){
                buf[UDP_DATA_P+i]=data[i];
                i++;
        }
        ck=checksum(&buf[IP_SRC_P], 16 + datalen,1);
        buf[UDP_CHECKSUM_H_P]=ck>>8;
        buf[UDP_CHECKSUM_L_P]=ck& 0xff;
        enc28j60PacketSend(UDP_HEADER_LEN+IP_HEADER_LEN+ETH_HEADER_LEN+datalen,buf);
}

void make_tcp_synack_from_syn(uint8_t *buf)
{
        uint16_t ck;
        make_eth(buf);
        // total length field in the IP header must be set:
        // 20 bytes IP + 24 bytes (20tcp+4tcp options)
        buf[IP_TOTLEN_H_P]=0;
        buf[IP_TOTLEN_L_P]=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4;
        make_ip(buf);
        buf[TCP_FLAGS_P]=TCP_FLAGS_SYNACK_V;
        make_tcphead(buf,1,0);
        // put an inital seq number
        buf[TCP_SEQ_H_P+0]= 0;
        buf[TCP_SEQ_H_P+1]= 0;
        // we step only the second byte, this allows us to send packts 
        // with 255 bytes, 512  or 765 (step by 3) without generating
        // overlapping numbers.
        buf[TCP_SEQ_H_P+2]= seqnum; 
        buf[TCP_SEQ_H_P+3]= 0;
        // step the inititial seq num by something we will not use
        // during this tcp session:
        seqnum+=3;
        // add an mss options field with MSS to 1280:
        // 1280 in hex is 0x500
        buf[TCP_OPTIONS_P]=2;
        buf[TCP_OPTIONS_P+1]=4;
        buf[TCP_OPTIONS_P+2]=0x05;
        buf[TCP_OPTIONS_P+3]=0x0;
        // The tcp header length is only a 4 bit field (the upper 4 bits).
        // It is calculated in units of 4 bytes.
        // E.g 24 bytes: 24/4=6 => 0x60=header len field
        buf[TCP_HEADER_LEN_P]=0x60;
        // here we must just be sure that the web browser contacting us
        // will send only one get packet
        buf[TCP_WIN_SIZE]=0x5; // 1400=0x578
        buf[TCP_WIN_SIZE+1]=0x78;
        // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + 4 (one option: mss)
        ck=checksum(&buf[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN+4,2);
        buf[TCP_CHECKSUM_H_P]=ck>>8;
        buf[TCP_CHECKSUM_L_P]=ck& 0xff;
        // add 4 for option mss:
        enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4+ETH_HEADER_LEN,buf);
}

// do some basic length calculations and store the result in static varibales
uint16_t get_tcp_data_len(uint8_t *buf)
{
        int16_t i;
        i=(((int16_t)buf[IP_TOTLEN_H_P])<<8)|(buf[IP_TOTLEN_L_P]&0xff);
        i-=IP_HEADER_LEN;
        i-=(buf[TCP_HEADER_LEN_P]>>4)*4; // generate len in bytes;
        if (i<=0){
                i=0;
        }
        return((uint16_t)i);
}

// get a pointer to the start of tcp data in buf
// Returns 0 if there is no data
// You must call init_len_info once before calling this function
uint16_t get_tcp_data_pointer(void)
{
        if (info_data_len){
                return((uint16_t)TCP_SRC_PORT_H_P+info_hdr_len);
        }else{
                return(0);
        }
}


// do some basic length calculations and store the result in static varibales
void init_len_info(uint8_t *buf)
{
        info_data_len=(((int16_t)buf[IP_TOTLEN_H_P])<<8)|(buf[IP_TOTLEN_L_P]&0xff);
        info_data_len-=IP_HEADER_LEN;
        info_hdr_len=(buf[TCP_HEADER_LEN_P]>>4)*4; // generate len in bytes;
        info_data_len-=info_hdr_len;
        if (info_data_len<=0){
                info_data_len=0;
        }
}


// fill in tcp data at position pos. pos=0 means start of
// tcp data. Returns the position at which the string after
// this string could be filled.
uint16_t fill_tcp_data_p(uint8_t *buf,uint16_t pos, const prog_char *progmem_s)
{
        char c;
        // fill in tcp data at position pos
        //
        // with no options the data starts after the checksum + 2 more bytes (urgent ptr)
        while ((c = pgm_read_byte(progmem_s++))) {
                buf[TCP_CHECKSUM_L_P+3+pos]=c;
                pos++;
        }
        return(pos);
}

// fill in tcp data at position pos. pos=0 means start of
// tcp data. Returns the position at which the string after
// this string could be filled.
uint16_t fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s)
{
        // fill in tcp data at position pos
        //
        // with no options the data starts after the checksum + 2 more bytes (urgent ptr)
        while (s && *s) {
                buf[TCP_CHECKSUM_L_P+3+pos]=*s;
                pos++;
                s++;
        }
        return(pos);
}

// Make just an ack packet with no tcp data inside
// This will modify the eth/ip/tcp header 
void make_tcp_ack_from_any(uint8_t *buf,int16_t datlentoack,uint8_t addflags)
{
        uint16_t j;
        make_eth(buf);
        // fill the header:
        buf[TCP_FLAGS_P]=TCP_FLAGS_ACK_V|addflags;
        if (datlentoack==0){
                // if there is no data then we must still acknoledge one packet
                make_tcphead(buf,1,1); // no options
        }else{
                make_tcphead(buf,datlentoack,1); // no options
        }
        // total length field in the IP header must be set:
        // 20 bytes IP + 20 bytes tcp (when no options) 
        j=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN;
        buf[IP_TOTLEN_H_P]=j>>8;
        buf[IP_TOTLEN_L_P]=j& 0xff;
        make_ip(buf);
        // use a low window size otherwise we have to have
        // timers and can not just react on every packet.
        buf[TCP_WIN_SIZE]=0x4; // 1024=0x400
        buf[TCP_WIN_SIZE+1]=0x0;
        // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
        j=checksum(&buf[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN,2);
        buf[TCP_CHECKSUM_H_P]=j>>8;
        buf[TCP_CHECKSUM_L_P]=j& 0xff;
        enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+ETH_HEADER_LEN,buf);
}


// dlen is the amount of tcp data (http data) we send in this packet
// You can use this function only immediately after make_tcp_ack_from_any
// This is because this function will NOT modify the eth/ip/tcp header except for
// length and checksum
// You must set TCP_FLAGS before calling this
void make_tcp_ack_with_data_noflags(uint8_t *buf,uint16_t dlen)
{
        uint16_t j;
        // total length field in the IP header must be set:
        // 20 bytes IP + 20 bytes tcp (when no options) + len of data
        j=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+dlen;
        buf[IP_TOTLEN_H_P]=j>>8;
        buf[IP_TOTLEN_L_P]=j& 0xff;
        fill_ip_hdr_checksum(buf);
        // zero the checksum
        buf[TCP_CHECKSUM_H_P]=0;
        buf[TCP_CHECKSUM_L_P]=0;
        // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
        j=checksum(&buf[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN+dlen,2);
        buf[TCP_CHECKSUM_H_P]=j>>8;
        buf[TCP_CHECKSUM_L_P]=j& 0xff;
        enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+dlen+ETH_HEADER_LEN,buf);
}


// you must have called init_len_info at some time before calling this function
// dlen is the amount of tcp data (http data) we send in this packet
// You can use this function only immediately after make_tcp_ack_from_any
// This is because this function will NOT modify the eth/ip/tcp header except for
// length and checksum
void make_tcp_ack_with_data(uint8_t *buf,uint16_t dlen)
{
        uint16_t j;
        // fill the header:
        // This code requires that we send only one data packet
        // because we keep no state information. We must therefore set
        // the fin here:
        buf[TCP_FLAGS_P]=TCP_FLAGS_ACK_V|TCP_FLAGS_PUSH_V|TCP_FLAGS_FIN_V;

        // total length field in the IP header must be set:
        // 20 bytes IP + 20 bytes tcp (when no options) + len of data
        j=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+dlen;
        buf[IP_TOTLEN_H_P]=j>>8;
        buf[IP_TOTLEN_L_P]=j& 0xff;
        fill_ip_hdr_checksum(buf);
        // zero the checksum
        buf[TCP_CHECKSUM_H_P]=0;
        buf[TCP_CHECKSUM_L_P]=0;
        // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
        j=checksum(&buf[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN+dlen,2);
        buf[TCP_CHECKSUM_H_P]=j>>8;
        buf[TCP_CHECKSUM_L_P]=j& 0xff;
        enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+dlen+ETH_HEADER_LEN,buf);
}

// return 0 to just continue in the packet loop and return the position 
// of the tcp/udp data if there is tcp/udp data part
int16_t packetloop_icmp_tcp(uint8_t *buf,uint16_t plen)
{
  uint16_t len;

  // arp is broadcast if unknown but a host may also
  // verify the mac address by sending it to 
  // a unicast address.
  if(eth_type_is_arp_and_my_ip(buf, plen)){
    if (buf[ETH_ARP_OPCODE_L_P] == ETH_ARP_OPCODE_REQ_L_V){
      make_arp_answer_from_request(buf);
    }
    return(-1);
  }

  // check if ip packets are for us:
  if(eth_type_is_ip_and_my_ip(buf, plen) == 0) return(-7);

  if(buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V){
    make_echo_reply_from_request(buf,plen);
    return(-2);
  }

  if (plen<54 && buf[IP_PROTO_P]!=IP_PROTO_TCP_V ){
    // smaller than the smallest TCP packet and not tcp port
    return(-3);
  }

  // This is a packet that the calling code should handle.
  if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V){
    make_tcp_synack_from_syn(buf);
    // make_tcp_synack_from_syn does already send the syn,ack
    return(-4);
  }

  if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V){
    info_data_len = get_tcp_data_len(buf);

    // we can possibly have no data, just ack:
    // Here we misuse plen for something else to save a variable.
    // plen is now the position of start of the tcp user data.
    len=HTTP_HEADER_START;
    if (info_data_len==0){
      if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V){
        // finack, answer with ack
        make_tcp_ack_from_any(buf,0,0);
      }

      // just an ack with no data, wait for next packet
      return(-5);
    }

    // Here we misuse plen for something else to save a variable
    return(len);
  }

  return(-6);
}
/* end of ip_arp_udp.c */
