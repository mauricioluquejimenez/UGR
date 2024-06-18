void mov_LED_der(int LED)
{  
  for(LED - 2; LED > 3; LED -= 2)
  {
    digitalWrite(LED, HIGH);
  	delay(50);
  	digitalWrite(LED + 2, LOW);
  	delay(50);
  }
}

void mov_LED_izq(int LED)
{  
 
  for(LED + 2; LED < 13; LED += 2)
  {
    digitalWrite(LED, HIGH);
  	delay(50);
  	digitalWrite(LED - 2, LOW);
  	delay(50);
  }

}

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop()
{
  
  mov_LED_der(13);
  mov_LED_izq(3);

}