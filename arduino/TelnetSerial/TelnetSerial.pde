// Provide a serial port over telnet

extern "C" {
  #include <enc28j60.h>
  #include <ip_arp_udp_tcp.h>
  #include <ip_config.h>
  #include <net.h>
}
#include <etherShield.h>

#define DEBUG 1
#define BUFFER_SIZE 550

static uint8_t mymac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x27}; 
static uint8_t myip[4] = {192, 168, 1, 250};
static uint8_t buf[BUFFER_SIZE + 1];

// The ethernet shield
EtherShield es = EtherShield();

void setup()
{
  Serial.begin(115200);
  Serial.println("TelnetSerial");

  es.ES_enc28j60Init(mymac);
  es.ES_init_ip_arp_udp_tcp(mymac, myip);
}

void dumppacketinfo(uint16_t l)
{
  int i;
  uint16_t port, t;

  Serial.print("Packet of length ");
  Serial.print(l);

  Serial.print(" type ");
  t = (byte) buf[ETH_TYPE_H_P];
  t += (byte) buf[ETH_TYPE_L_P];
  Serial.print((int) t);
    
  if(buf[ETH_TYPE_H_P] == ETHTYPE_ARP_H_V && buf[ETH_TYPE_L_P] == ETHTYPE_ARP_L_V)
    Serial.println(" ARP");
  else if(buf[ETH_TYPE_H_P] == ETHTYPE_IP_H_V && buf[ETH_TYPE_L_P] == ETHTYPE_IP_L_V)
  {
    Serial.print(" IPv");
    Serial.print((int) (byte) (buf[IP_HEADER_LEN_VER_P] >> 4));
    
    Serial.print(" from ");
    for(i = 0; i < 4; i++)
    {
      Serial.print((int) (byte) buf[IP_SRC_P + i]);
      if(i != 3) Serial.print(".");
    }

    Serial.print(" to ");
    for(i = 0; i < 4; i++)
    {
      Serial.print((int) (byte) buf[IP_DST_P + i]);
      if(i != 3) Serial.print(".");
    }
    
    if(buf[IP_PROTO_P] == IP_PROTO_ICMP_V)
      Serial.println(" ICMP");
    else if(buf[IP_PROTO_P] == IP_PROTO_TCP_V)
    {
      Serial.print(" TCP [");
      port = (byte) buf[TCP_SRC_PORT_H_P];
      port += (byte) buf[TCP_SRC_PORT_L_P];
      Serial.print((int) port);
      Serial.print("->");
      port = (byte) buf[TCP_DST_PORT_H_P];
      port += (byte) buf[TCP_DST_PORT_L_P];
      Serial.print((int) port);
      Serial.println("]");
    }
    else if(buf[IP_PROTO_P] == IP_PROTO_UDP_V)
      Serial.println(" UDP");
    else
      Serial.println("");
  }
  else
    Serial.println(" unknown");
}

void loop()
{
  int i;
  uint16_t l, datastart;
  
  // Read: ES_enc28j60PacketReceive is a thin wrapper which returns a packet of the queue.
  // A length of zero means that there were no packets waiting.
  // ES_packetloop_icmp_tcp then processes things like ICMP "for free" and returns non-zero
  // only if we need to further process the packet.
  l = es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf);
  if(l > 0)
  {   
    #ifdef DEBUG
    dumppacketinfo(l);
    #endif

    datastart = es.ES_packetloop_icmp_tcp(buf, l);
    if(datastart > 0){
      #ifdef DEBUG
      Serial.print("Data starts at byte: ");
      Serial.print(datastart);
      Serial.print(" String of length ");
      Serial.println(strlen((const char *) buf + datastart));
      #endif
    
      // I have a problem with small packets (those less than 5 bytes of data?) here. It seems
      // that the buffer returned from the enc28j60 is padded with nulls in that case, which
      // pushes the ACKs outside the range of valid data, and therefore causes retransmits.
      // Note this fix wont work for binary data...
      // TODO(mikal): this might be because the ethernet frame length is being used instead of
      // the TCP packet length?
      if(l - datastart < 7)
        l = datastart + strlen((const char *) buf + datastart);

      make_tcp_ack_from_any(buf, l - datastart, 0);
      Serial.print("Acked ");
      Serial.print(l - datastart);
      Serial.println(" bytes");
    
      Serial.print(">>");
      for(i = datastart; i < l; i++){
        if(buf[i] == '\n') Serial.print("\\n");
        else if(buf[i] == '\r') Serial.print("\\r");
        else if(buf[i] == 0) Serial.print("NULL");
        else Serial.print((char) buf[i]);
        
        Serial.print("(");
        Serial.print((int) buf[i]);
        Serial.print(") ");
      }
      Serial.println("<<");
    }
  }
  
  // Write
  //l = es.ES_fill_tcp_data_p(buf, 0, PSTR("Hello\n"));
  //buf[TCP_FLAGS_P] = TCP_FLAGS_ACK_V | TCP_FLAGS_PUSH_V;
  //make_tcp_ack_with_data_noflags(buf, l);
}
