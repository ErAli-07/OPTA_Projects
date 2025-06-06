#include <Arduino_BuiltIn.h>

void setup() {
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  pinMode(LED_D3, OUTPUT);
}

void loop() {

  //Spegnere tutti i led
  digitalWrite(LED_D0, LOW);
  digitalWrite(LED_D1, LOW);
  digitalWrite(LED_D2, LOW);
  digitalWrite(LED_D3, LOW);

  // Ogni due secondi accendere un led alla volta
  delay(2000);
  digitalWrite(LED_D0, HIGH);
  delay(2000);
  digitalWrite(LED_D1, HIGH);
  delay(2000);
  digitalWrite(LED_D2, HIGH);
  delay(2000);
  digitalWrite(LED_D3, HIGH);
  delay(2000);

  digitalWrite(LED_D1, LOW);
  digitalWrite(LED_D2, LOW);

  delay(2000);

  digitalWrite(LED_D0, LOW);
  digitalWrite(LED_D3, LOW);
  digitalWrite(LED_D1, HIGH);
  digitalWrite(LED_D2, HIGH);

  delay(2000);

  digitalWrite(LED_D2, LOW);
  digitalWrite(LED_D0, HIGH);

  delay(2000);

  digitalWrite(LED_D0, LOW);
  digitalWrite(LED_D1, LOW);
  digitalWrite(LED_D2, HIGH);
  digitalWrite(LED_D3, HIGH);

  delay(2000);

  digitalWrite(LED_D2, LOW);
  digitalWrite(LED_D1, HIGH);

  delay(2000);

  digitalWrite(LED_D3, LOW);
  digitalWrite(LED_D1, LOW);
  digitalWrite(LED_D2, HIGH);
  digitalWrite(LED_D0, HIGH);

  delay(2000);

  digitalWrite(LED_D0, HIGH);
  digitalWrite(LED_D1, HIGH);
  digitalWrite(LED_D2, HIGH);
  digitalWrite(LED_D3, HIGH);

  delay(2000);
}
