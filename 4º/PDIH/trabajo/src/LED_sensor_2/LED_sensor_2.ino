const int sonido = 340;
int distancia;
long tiempo;

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void calculate_distance(int trigger, int echo)
{
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  tiempo = pulseIn(echo, HIGH);
  distancia = (tiempo * sonido) / (2 * 10000);
}

void loop()
{
  calculate_distance(2, 3);

  if (distancia < 50) digitalWrite(12, HIGH);
  else digitalWrite(12, LOW);

  calculate_distance(4, 5);

  if (distancia < 50)
  {
    digitalWrite(13, HIGH);
    char text[] = " Hello From Arduino Uno";
    Serial.println(text);
    delay(1000);
  }
  else digitalWrite(13, LOW);
}