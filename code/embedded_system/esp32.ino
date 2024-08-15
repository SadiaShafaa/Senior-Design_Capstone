#include <ESP32Servo.h>
#include <DHT.h>
#define DHTTYPE DHT11

Servo myservo1;
Servo myservo2;

int x;

// humidity sensor
int humidity = 25;
DHT dht(humidity, DHTTYPE);
// int Buzzer135 = 27;
int Gas_analog135 = 4;

// for mq2
// int Buzzer2 = 23;
int Gas_analog2 = 2;

// for mq4
// int Buzzer4 = 32;
int Gas_analog4 = 5;

// for mq5
// int Buzzer5 = 26;
int Gas_analog5 = 16;

// flame sensor
int buzzerPin = 32;
int flamePin = 15;

int Flame = HIGH;

int pump1 = 27;
int pump2 = 26;
int pos1 = 0;
int pos2 = 0;
boolean fire1 = false;
boolean fire2 = false;

void setup()
{
  Serial.begin(115200);
  pinMode(Gas_analog2, INPUT);
  pinMode(Gas_analog4, INPUT);
  pinMode(Gas_analog5, INPUT);
  pinMode(Gas_analog135, INPUT);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  digitalWrite(pump1, HIGH);
  digitalWrite(pump2, HIGH);
  myservo1.attach(13);
  myservo1.write(90);
  myservo2.attach(14);
  myservo2.write(90);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  pinMode(buzzerPin, OUTPUT);
  pinMode(flamePin, INPUT);
  Serial.setTimeout(1);
}

void loop()
{
  myservo1.write(90);
  myservo2.write(90);

  int gassensorAnalog2 = analogRead(Gas_analog2);
  int gassensorAnalog4 = analogRead(Gas_analog4);
  int gassensorAnalog5 = analogRead(Gas_analog5);
  int gassensorAnalog135 = analogRead(Gas_analog135);

  while (!Serial.available())
    ;
  x = Serial.readString().toInt();

  if (x > 0)
  {
    digitalWrite(buzzerPin, HIGH);
  }

  Flame = digitalRead(flamePin);
  if (Flame == LOW)
  {
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Fire");

    // digitalWrite(redled, HIGH);
    // digitalWrite(greenled, LOW);
  }
  else
  {
    digitalWrite(buzzerPin, LOW);
    Serial.println("No Fire");

    //   digitalWrite(greenled, HIGH);
    //   digitalWrite(redled, LOW);
  }

  if (Flame == LOW && (gassensorAnalog2 > 50 || gassensorAnalog4 > 500 || gassensorAnalog5 > 500 || gassensorAnalog135 > 150))
  {
    // digitalWrite(buzzerPin, HIGH);
    // Serial.println("Fire");
    fire1 = true;
  }

  // else{
  //   digitalWrite(buzzerPin, LOW);

  // }

  if (Flame == LOW && (gassensorAnalog2 < 500 || gassensorAnalog4 < 500 || gassensorAnalog5 < 500 || gassensorAnalog135 < 150))
  {
    // digitalWrite(buzzerPin, HIGH);
    // Serial.println("Fire");
    fire2 = true;
  }

  if (gassensorAnalog2 > 100)
  {
    Serial.print("LPG Gas is Detected in MQ2 = ");
    Serial.print(gassensorAnalog2);
    Serial.println("");
    //  fire = true;
  }
  else
  {
    //  Serial.println("MQ2 CLear");
  }

  if (gassensorAnalog4 > 500)
  {
    Serial.print("C.Methane Gas is Detected in MQ4 = ");
    Serial.print(gassensorAnalog4);
    Serial.println("");
    //  fire = true;
  }
  else
  {

    //  Serial.println("MQ4 CLear");
  }

  if (gassensorAnalog5 > 500)
  {
    Serial.print("NG Gas is Detected in MQ5 = ");
    Serial.print(gassensorAnalog5);
    Serial.println("");
    //  fire = true;
  }
  else
  {
    //  Serial.println("MQ5 CLear");
  }

  if (gassensorAnalog135 > 150)
  {
    Serial.print("Bad Air Quaility Detected in MQ135 = ");
    Serial.print(gassensorAnalog135);
    Serial.println("");
  }
  else
  {
    //  Serial.println("Air Quality Stable");
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.print(F("°C "));
  // Serial.print(f);
  // Serial.print(F("°F  Heat index: "));
  // Serial.print(hic);
  // Serial.print(F("°C "));
  // Serial.print(hif);
  // Serial.println(F("°F"));

  while (fire1 == true)
  {
    put_off_fire();
    Serial.println("Fire Fire!");
  }

  while (fire2 == true)
  {
    put_off_fire_water();
    Serial.println("Fire Fire!");
  }
  delay(100);
}

void put_off_fire()
{
  digitalWrite(pump1, LOW);
  delay(1000);
  for (pos1 = 50; pos1 <= 180; pos1 += 1)
  {
    myservo1.write(pos1);
    Serial.println(pos1);
    delay(10);
  }
  for (pos1 = 180; pos1 >= 50; pos1 -= 1)
  {
    myservo1.write(pos1);
    delay(10);
  }

  digitalWrite(pump1, HIGH);
  myservo1.write(90);
  fire1 = false;
}

void put_off_fire_water()
{
  digitalWrite(pump2, LOW);
  delay(1000);

  for (pos2 = 50; pos2 <= 180; pos2 += 1)
  {
    myservo2.write(pos2);
    Serial.println(pos2);
    delay(10);
  }

  for (pos2 = 180; pos2 >= 50; pos2 -= 1)
  {
    myservo2.write(pos2);
    delay(10);
  }
  digitalWrite(pump2, HIGH);
  myservo2.write(90);
  fire2 = false;
}
