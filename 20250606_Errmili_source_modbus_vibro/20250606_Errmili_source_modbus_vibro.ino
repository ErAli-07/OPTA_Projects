#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

int sw1 = 0;
unsigned long tempo1 = 0;

int sensorvalue0;
int sensorvalue1;

IPAddress ip(192, 168, 1, 23);  // Indirizzo IP

EthernetServer ethServer(502);  // Numero di Porta

ModbusTCPServer modbusTCPServer;

void setup() {

  // Lampadine LED
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);

  // Uscita Vibro
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  // Ingressi - Sensori
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);

  //Avvio la comunicazione
  Serial.begin(9600);
  while (!Serial)
    ;
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("- Ethernet interface not found!");
    while (true)
      ;
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("- Ethernet cable not connected!");
  }

  ethServer.begin();
  if (!modbusTCPServer.begin()) {
    Serial.println("- Failed to start Modbus TCP Server!");
    while (1)
      ;
  }

  modbusTCPServer.configureCoils(0x00, 4);
  modbusTCPServer.configureDiscreteInputs(0x00, 2);
}

void loop() {
  EthernetClient client = ethServer.available();
  if (client) {
    Serial.println("- Client connected");

    modbusTCPServer.accept(client);

    // Instaurazione comunicazione
    while (client.connected()) {
      modbusTCPServer.poll();
      updateLED();

      sensorvalue0 = digitalRead(A0);
      sensorvalue1 = digitalRead(A1);

      if ((sensorvalue0 == LOW) || (sensorvalue1 == LOW)) {
        Serial.println("ON");
        updateInput();

        if (sw1 == 0) {
          tempo1 = millis();
          Serial.print("Tempo1 : ");
          Serial.println(tempo1);
          sw1 = 1;
        }
        unsigned long momento1 = millis();

        Serial.println(momento1 - tempo1);
        int diff = momento1 - tempo1;
        Serial.print("DIFF : ");
        Serial.println(diff);

        if (sw1 == 1 && diff <= 2000) {  // Attiva Pistone + Accende Lampadina
          digitalWrite(D1, HIGH);
          digitalWrite(LED_D1, HIGH);
        }
        if (sw1 == 1 && diff > 2000 && diff <= 4000) {  // Disattiva Pistone + Spegne Lampadina
          digitalWrite(D1, LOW);
          digitalWrite(LED_D1, LOW);
        }
        if (diff > 4000) {
          sw1 = 0;
        }
      } else {
        Serial.println("OFF");
        updateInput();
        digitalWrite(LED_D0, LOW);
        digitalWrite(LED_D1, LOW);
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
      }
      Serial.print(" sw1 : ");
      Serial.println(sw1);
    }
    Serial.println("- Client disconnected.");
  } else {
    Serial.println("- Client not present.");
  }
}

/**
*@param None
*/

void updateLED() {
  int coilValue = modbusTCPServer.coilRead(0x00);
  int coilValue1 = modbusTCPServer.coilRead(0x01);

  digitalWrite(LED_D0, coilValue ? HIGH : LOW);
  digitalWrite(D0, coilValue ? HIGH : LOW);  // Tramite un dispositivo si controlla il Vibro D0 e la lampadina
  digitalWrite(LED_D1, coilValue1 ? HIGH : LOW);
}

void updateInput() {
  if (sensorvalue0 == HIGH) {
    modbusTCPServer.discreteInputWrite(0x00, 1);
  } else {
    modbusTCPServer.discreteInputWrite(0x00, 0);
  }

  if (sensorvalue1 == HIGH) {
    modbusTCPServer.discreteInputWrite(0x01, 1);
  } else {
    modbusTCPServer.discreteInputWrite(0x01, 0);
  }
}