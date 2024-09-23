void setup()
{
	pinMode(12, OUTPUT);
}

void loop()
{
	digitalWrite(12, HIGH);
	delay(1500);

	digitalWrite(12, LOW);
	delay(1500);

	digitalWrite(13, HIGH);
	delay(1500);

	digitalWrite(13, LOW);
	delay(1500);
}
