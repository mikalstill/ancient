// a wrapper class for EtherShield

extern "C" {
	#include "enc28j60.h"
	#include "ip_arp_udp_tcp.h"
	#include "websrv_help_functions.h"
	#include "wiring.h"
}
#include "EtherShield.h"

//constructor
EtherShield::EtherShield(){
}



void EtherShield::ES_enc28j60Init(uint8_t* macaddr){
  /*initialize enc28j60*/
  enc28j60Init(macaddr);
  enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
  delay(10);

  /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
  // LEDA=greed LEDB=yellow
  //
  // 0x880 is PHLCON LEDB=on, LEDA=on
  // enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
  enc28j60PhyWrite(PHLCON,0x880);
  delay(500);
  //
  // 0x990 is PHLCON LEDB=off, LEDA=off
  // enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
  enc28j60PhyWrite(PHLCON,0x990);
  delay(500);
  //
  // 0x880 is PHLCON LEDB=on, LEDA=on
  // enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
  enc28j60PhyWrite(PHLCON,0x880);
  delay(500);
  //
  // 0x990 is PHLCON LEDB=off, LEDA=off
  // enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
  enc28j60PhyWrite(PHLCON,0x990);
  delay(500);
  //
  // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
  // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
  enc28j60PhyWrite(PHLCON,0x476);
  delay(100);
}

void EtherShield::ES_enc28j60clkout(uint8_t clk){
	enc28j60clkout(clk);
}

uint8_t EtherShield::ES_enc28j60linkup(void) {
	return enc28j60linkup();
}

void EtherShield::ES_enc28j60PhyWrite(uint8_t address, uint16_t data){
	enc28j60PhyWrite(address,  data);
}

uint16_t EtherShield::ES_enc28j60PacketReceive(uint16_t len, uint8_t* packet){
	return enc28j60PacketReceive(len, packet);
}

void EtherShield::ES_init_ip_arp_udp_tcp(uint8_t *mymac,uint8_t *myip){
	init_ip_arp_udp_tcp(mymac,myip);
}

uint8_t EtherShield::ES_eth_type_is_arp_and_my_ip(uint8_t *buf,uint16_t len) {
	return eth_type_is_arp_and_my_ip(buf,len);
}

void EtherShield::ES_make_echo_reply_from_request(uint8_t *buf,uint16_t len) {
	make_echo_reply_from_request(buf,len);
}

void EtherShield::ES_make_tcp_synack_from_syn(uint8_t *buf) {
	make_tcp_synack_from_syn(buf);
}

void EtherShield::ES_init_len_info(uint8_t *buf) {
	init_len_info(buf);
}

uint16_t EtherShield::ES_get_tcp_data_pointer(void) {
	return get_tcp_data_pointer();
}

uint16_t EtherShield::ES_packetloop_icmp_tcp(uint8_t *buf,uint16_t plen) {
	return packetloop_icmp_tcp(buf,plen);
}

uint16_t EtherShield::ES_fill_tcp_data_p(uint8_t *buf,uint16_t pos, const prog_char *progmem_s){
	return fill_tcp_data_p(buf, pos, progmem_s);
}

uint16_t EtherShield::ES_fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s){
	return fill_tcp_data(buf,pos, s);
}

void EtherShield::ES_www_server_reply(uint8_t *buf,uint16_t dlen) {
	www_server_reply(buf,dlen);
}
	
void EtherShield::ES_register_ping_rec_callback(void (*callback)(uint8_t *srcip)) {
	register_ping_rec_callback(callback);
}

#ifdef FROMDECODE_websrv_help
uint8_t EtherShield::ES_find_key_val(char *str,char *strbuf, uint8_t maxlen,char *key) {
	return find_key_val(str,strbuf, maxlen,key);
}

void EtherShield::ES_urldecode(char *urlbuf) {
	urldecode(urlbuf);
}
#endif

#ifdef URLENCODE_websrv_help
void EtherShield::ES_urlencode(char *str,char *urlbuf) {
	urlencode(str,urlbuf);
}
#endif

uint8_t EtherShield::ES_parse_ip(uint8_t *bytestr,char *str) {
	return parse_ip(bytestr,str);
}

void EtherShield::ES_mk_net_str(char *resultstr,uint8_t *bytestr,uint8_t len,char separator,uint8_t base) {
	mk_net_str(resultstr,bytestr,len,separator,base);
}
