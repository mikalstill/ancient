uint8_t ports[8] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

void usage()
{
  Serial.println("");
  Serial.println("Arduino remote power manager");
  Serial.println("?: list port states");
  Serial.println("0: turn off a port");
  Serial.println("1: turn on a port");
  Serial.println("");
  Serial.print(">> ");
}

void portstate()
{
  int i;
  
  for(i = 0; i < 8; i++)
  {
    Serial.print("Port ");
    Serial.print(i);
    Serial.print(" is ");
    if(ports[i] == HIGH) Serial.println("on");
    else Serial.println("off");
  }
  usage();
}

uint8_t read()
{
  int c = -1;
  char temp[100];
  
  while(Serial.available() == 0) delay(1);
  while(c == -1) c = Serial.read();
  sprintf(temp, "%c", c);
  Serial.println(temp);
  return c;
}

void setup() 
{ 
  Serial.begin(9600);
  usage();
}

void loop()
{
  int c, port = -1;

  if(Serial.available() > 0)
  {
    c = read();
    switch(c)
    {
      case '?':
        portstate();
        break;
        
      case '0':
        Serial.print("Which port? >>");
        port = read() - '0';
        ports[port] = LOW;
        digitalWrite(port + 2, LOW);
        portstate();
        break;
        
      case '1':
        Serial.print("Which port? >>");
        port = read() - '0';
        ports[port] = HIGH;
        digitalWrite(port + 2, HIGH);
        portstate();
        break;
        
      default:
        usage();
    }
  }
}
