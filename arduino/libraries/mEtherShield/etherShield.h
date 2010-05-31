/*
  EHTERSHIELD_H library for Arduino etherShield
  Copyright (c) 2008 Xing Yu.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ETHERSHIELD_H
#define ETHERSHIELD_H

#include <inttypes.h>
#include "enc28j60.h"
#include "ip_config.h"
#include "ip_arp_udp_tcp.h"
#include "net.h"


class EtherShield
{
  public:
  
    EtherShield();

	void ES_enc28j60Init(uint8_t* macaddr);
	void ES_enc28j60clkout(uint8_t clk);
	uint8_t ES_enc28j60linkup(void);
	void ES_enc28j60PhyWrite(uint8_t address, uint16_t data);
	uint16_t ES_enc28j60PacketReceive(uint16_t len, uint8_t* packet);

	void ES_init_ip_arp_udp_tcp(uint8_t *mymac,uint8_t *myip);
	// for a UDP server:
	uint8_t ES_eth_type_is_arp_and_my_ip(uint8_t *buf,uint16_t len);

	void ES_make_echo_reply_from_request(uint8_t *buf,uint16_t len);
	void ES_make_tcp_synack_from_syn(uint8_t *buf);
	void ES_init_len_info(uint8_t *buf);
	uint16_t ES_get_tcp_data_pointer(void);

	// return 0 to just continue in the packet loop and return the position 
	// of the tcp data if there is tcp data part
	uint16_t ES_packetloop_icmp_tcp(uint8_t *buf,uint16_t plen);
	// functions to fill the web pages with data:
	uint16_t ES_fill_tcp_data_p(uint8_t *buf,uint16_t pos, const prog_char *progmem_s);
	uint16_t ES_fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s);
	// send data from the web server to the client:
	void ES_www_server_reply(uint8_t *buf,uint16_t dlen);
	
#define HTTP_HEADER_START ((uint16_t)TCP_SRC_PORT_H_P+(buf[TCP_HEADER_LEN_P]>>4)*4)

	// you can find out who ping-ed you if you want:
	void ES_register_ping_rec_callback(void (*callback)(uint8_t *srcip));

#ifdef FROMDECODE_websrv_help
	uint8_t ES_find_key_val(char *str,char *strbuf, uint8_t maxlen,char *key);
	void ES_urldecode(char *urlbuf);
#endif	// FROMDECODE_websrv_help

#ifdef URLENCODE_websrv_help
	void ES_urlencode(char *str,char *urlbuf);
#endif	// URLENCODE_websrv_help

	uint8_t ES_parse_ip(uint8_t *bytestr,char *str);
	void ES_mk_net_str(char *resultstr,uint8_t *bytestr,uint8_t len,char separator,uint8_t base);

};

#endif

