/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher 
 * Copyright: GPL V2
 *
 * IP/ARP/UDP/TCP functions
 *
 * Chip type           : ATMEGA88 with ENC28J60
 *********************************************/
 /*********************************************
 * Modified: nuelectronics.com -- Ethershield for Arduino
 *********************************************/
//@{
#ifndef IP_ARP_UDP_TCP_H
#define IP_ARP_UDP_TCP_H
#include "ip_config.h"
#include <avr/pgmspace.h>

// -- web server functions --
// you must call this function once before you use any of the other server functions:
extern void init_ip_arp_udp_tcp(uint8_t *mymac,uint8_t *myip);
// for a UDP server:
extern uint8_t eth_type_is_arp_and_my_ip(uint8_t *buf,uint16_t len);
extern uint8_t eth_type_is_ip_and_my_ip(uint8_t *buf,uint16_t len);
extern uint8_t eth_type_is_arp_reply(uint8_t *buf);
extern uint8_t eth_type_is_arp_req(uint8_t *buf);

extern void make_udp_reply_from_request(uint8_t *buf,char *data,uint8_t datalen,uint16_t port);
extern void make_echo_reply_from_request(uint8_t *buf,uint16_t len);

extern void make_arp_answer_from_request(uint8_t *buf);
extern void make_tcp_synack_from_syn(uint8_t *buf);
extern void init_len_info(uint8_t *buf);
extern uint16_t get_tcp_data_pointer(void);

extern void make_tcp_ack_from_any(uint8_t *buf, uint16_t dlen, uint8_t addflags);
extern void make_tcp_ack_with_data(uint8_t *buf,uint16_t dlen);
extern void make_tcp_ack_with_data_noflags(uint8_t *buf,uint16_t dlen);


// return 0 to just continue in the packet loop and return the position 
// of the tcp data if there is tcp data part
extern int16_t packetloop_icmp_tcp(uint8_t *buf,uint16_t plen);
// functions to fill the web pages with data:
extern uint16_t fill_tcp_data_p(uint8_t *buf,uint16_t pos, const prog_char *progmem_s);
extern uint16_t fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s);
// send data from the web server to the client:

#define HTTP_HEADER_START ((uint16_t)TCP_SRC_PORT_H_P+(buf[TCP_HEADER_LEN_P]>>4)*4)

// you can find out who ping-ed you if you want:
extern void register_ping_rec_callback(void (*callback)(uint8_t *srcip));

#endif /* IP_ARP_UDP_TCP_H */
//@}
