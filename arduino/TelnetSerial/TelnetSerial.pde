// Provide a serial port over telnet

extern "C" {
  #include <enc28j60.h>
  #include <ip_arp_udp_tcp.h>
  #include <ip_config.h>
  #include <net.h>
  
  #include <dumppacketinfo.h>
}

#define BUFFER_SIZE 550
#define ERROR_500 "HTTP/1.0 500 Error\r\nContent-Type: text/html\r\n\r\n<h1>500 Error</h1>"

static uint8_t mymac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x27}; 
static uint8_t myip[4] = {192, 168, 1, 250};
static uint8_t buf[BUFFER_SIZE + 1];
char data[BUFFER_SIZE + 1];

void setup()
{
  Serial.begin(9600);
  enc28j60Init(mymac);
  init_ip_arp_udp_tcp(mymac, myip);
}

uint16_t http200ok(void)
{
  return(fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n"
                                      "Pragma: no-cache\r\n\r\n")));
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf)
{
  uint16_t plen;
  plen = http200ok();
  plen = fill_tcp_data_p(buf, plen, PSTR("<html><head><title>Temperature sensor</title>"
                                         "</head><body><pre>"));
  plen = fill_tcp_data(buf, plen, data);
  plen = fill_tcp_data_p(buf, plen, PSTR("</pre></body></html>"));

  return(plen);
}

void loop()
{
  int i;
  char c[1];
  uint16_t l, j, datastart, port, packetlength;
  
  // Read: ES_enc28j60PacketReceive is a thin wrapper which returns a packet of the queue.
  // A length of zero means that there were no packets waiting.
  // ES_packetloop_icmp_tcp then processes things like ICMP "for free" and returns non-zero
  // only if we need to further process the packet.
  packetlength = enc28j60PacketReceive(BUFFER_SIZE, buf);
  if(packetlength > 0)
  {
    dumppacketinfo(buf, packetlength);
  
    // Handle low level packets
    datastart = packetloop_icmp_tcp(buf, packetlength);
    if(datastart < 1) Serial.println(datastart);
    else if(datastart > 0){
      // This is a packet which requires further processing
    
      // I have a problem with small packets (those less than 5 bytes of data?) here. It seems
      // that the buffer returned from the enc28j60 is padded with nulls in that case, which
      // pushes the ACKs outside the range of valid data, and therefore causes retransmits.
      // Note this fix wont work for binary data...
      // TODO(mikal): this might be because the ethernet frame length is being used instead of
      // the TCP packet length?
      if(packetlength - datastart < 7)
        packetlength = datastart + strlen((const char *) buf + datastart);

      port = (byte) buf[TCP_DST_PORT_H_P];
      port += (byte) buf[TCP_DST_PORT_L_P];
      switch(port)
      {
        case 23:
          // Simple ethernet serial adapter. We just ack and send the data to serial.
          make_tcp_ack_from_any(buf, packetlength - datastart, 0);
          for(i = datastart; i < packetlength; i++){
            Serial.print((char) buf[i]);
          }
          break;
          
        case 80:
          // Simple HTTP server
          if(strncmp("GET ", (char *) &(buf[datastart]), 4) != 0)
          {
            // head, post and other methods:
            l = http200ok();
            l = fill_tcp_data_p(buf, l, PSTR("<h1>200 OK</h1>"));
          }
          else if(strncmp("/ ", (char *)&(buf[datastart + 4]), 2) == 0)
          {
            // Temperature web page
            sprintf(data, "Time is %d", millis());
            l = print_webpage(buf);
          }
          else{
            l = fill_tcp_data_p(buf, 0, PSTR(ERROR_500));
          }
          
          make_tcp_ack_from_any(buf, packetlength - datastart, 0);
          buf[TCP_FLAGS_P] = TCP_FLAGS_ACK_V | TCP_FLAGS_PUSH_V | TCP_FLAGS_FIN_V;
          make_tcp_ack_with_data_noflags(buf, l);
          break;
      }
    }
  }
  
  // Write
//  if(Serial.available())
//  {
//    c[0] = Serial.read();
//    c[1] = 0;
//
//    Serial.print(c);
//
//    l = fill_tcp_data(buf, 0, c);
//    for(i = 0; i < 6; i++)
//        {
//                buf[ETH_DST_MAC +i ] = buf[ETH_SRC_MAC + i];
//                buf[ETH_SRC_MAC + i] = macaddr[i];
//                i++;
//        }
//
//    buf[TCP_FLAGS_P] = TCP_FLAGS_PUSH_V;
//    
//    buf[IP_TOTLEN_H_P] = (IP_HEADER_LEN + TCP_HEADER_LEN_PLAIN + 1) >> 8;
//    buf[IP_TOTLEN_L_P] = (IP_HEADER_LEN + TCP_HEADER_LEN_PLAIN + 1) & 0xff;
//        
//    fill_ip_hdr_checksum(buf);
//    buf[TCP_CHECKSUM_H_P] = 0;
//    buf[TCP_CHECKSUM_L_P] = 0;
//        
//    // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
//    j = checksum(&buf[IP_SRC_P], 8 + TCP_HEADER_LEN_PLAIN + 1, 2);
//    buf[TCP_CHECKSUM_H_P] = j >> 8;
//    buf[TCP_CHECKSUM_L_P] = j & 0xff;
//    enc28j60PacketSend(IP_HEADER_LEN + TCP_HEADER_LEN_PLAIN + 1 + ETH_HEADER_LEN, buf);
//  }
}
