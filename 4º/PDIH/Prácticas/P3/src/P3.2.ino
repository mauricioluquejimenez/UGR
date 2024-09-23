int buttonState = 0;

void setup()
{
  pinMode(7, INPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  // read the state of the pushbutton
  buttonState = digitalRead(7);
  
  // check if pushbutton is pressed.
  // if it is, the button state is HIGH
  if (buttonState == HIGH)
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
  }
  else
  {
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(11, HIGH);
  }
  
  delay(10);
}