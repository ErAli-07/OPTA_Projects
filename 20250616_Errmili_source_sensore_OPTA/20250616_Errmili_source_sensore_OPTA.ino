#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

int pezzi = 0;
int sensorvalue;
unsigned long momento;

void setup() {
  //Lampadina LED
  pinMode(LED_D0, OUTPUT);
  // Ingresso - Sensore
  pinMode(A0, INPUT);

  Serial.begin(9600);
  Serial.println("Simulazione avviata...");
}

void loop() {

  sensorvalue = digitalRead(A0);

  if (sensorvalue == HIGH) {
    digitalWrite(LED_D0, HIGH);
    momento = millis();

    while (digitalRead(A0) == LOW) {
      if ((millis() - momento) > 1250) {
        Serial.println("Errore: Ritardo dell'avanzamento pezzo");
        digitalWrite(LED_D0, LOW);

        // Aspetta rilascio
        while (digitalRead(A0) == LOW)
          ;
        delay(50);  // debounce
        return;
      }

      digitalWrite(LED_D0, LOW);
      pezzi++;
      Serial.print("Pezzi: ");
      Serial.println(pezzi);

      delay(150);  // debounce
    }
  }
}