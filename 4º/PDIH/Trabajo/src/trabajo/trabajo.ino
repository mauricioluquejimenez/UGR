const int sonido = 340;
int distancia;
long tiempo;

void setup()
{
  Serial.begin(9600);
  
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
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
  calculate_distance(4, 5);

  if (distancia < 50) 
  {
    Serial.println("pausa");
    digitalWrite(13, HIGH);
  }
  else
  {
    Serial.println("reproducir");
    digitalWrite(13, LOW);
  }

  calculate_distance(2, 3);
  
  if (distancia < 10)
  {
    Serial.println("up");
    digitalWrite(12, HIGH);
  }
  else
  {
    Serial.println("down");
    digitalWrite(12, LOW);
  }

  delay(200);
}