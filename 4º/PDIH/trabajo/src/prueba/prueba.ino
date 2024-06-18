int data;

void setup()
 { 
  Serial.begin(9600); //initialize serial COM at 9600 baudrate
  pinMode(13, OUTPUT); //make the LED pin (13) as output
}

void loop()
{
  while (Serial.available()) { data = Serial.read(); }

  if (data == '1') digitalWrite (13, HIGH);
  if (data == '0') digitalWrite (13, LOW);
}
