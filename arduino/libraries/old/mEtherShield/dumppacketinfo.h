void dumppacketinfo(uint8_t* buf, uint16_t l)
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

