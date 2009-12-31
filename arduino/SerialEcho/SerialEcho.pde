void setup() 
{ 
  Serial.begin(9600);
}

void loop()
{
  int i;
  char temp[2];
  
  for(i = 0; i < Serial.available(); i++)
  {
    sprintf(temp, "%c", Serial.read());
    Serial.print(temp);
  }
}
