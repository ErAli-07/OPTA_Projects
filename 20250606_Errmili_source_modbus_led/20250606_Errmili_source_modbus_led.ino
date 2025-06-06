#include <Arduino_BuiltIn.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

//Switch x temporizzatore
int sw = 0;
int sw1 = 0;


unsigned long tempo = 0;
unsigned long tempo1 = 0;



const int ledPin = LED_RESET;
int ledState = LOW;

int sensorvalue0;
int sensorvalue1;
int sensorvalue2;
int sensorvalue3;
int cont0 = 0;
int cont1 = 0;
int cont2 = 0;
int cont3 = 0;

IPAddress ip(192, 168, 1, 23);  // Indirizzo IP

EthernetServer ethServer(502);  // Numero di Porta

ModbusTCPServer modbusTCPServer;

const int led3Pin = LED_D3;

void setup() {

  //START LEDS
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);
  pinMode(LED_D2, OUTPUT);

  //START RELAYS
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);

  //Inizializzo gli ingressi (li considero tutti digitali)
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(LEDR, LOW);




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

  pinMode(led3Pin, OUTPUT);

  modbusTCPServer.configureCoils(0x00, 1);
}

void loop() {

  EthernetClient client = ethServer.available();
  if (client) {
    Serial.println("- Client connected");

    modbusTCPServer.accept(client);

    while (client.connected()) {
      modbusTCPServer.poll();
      updateLED();

      sensorvalue0 = digitalRead(A0);
      sensorvalue1 = digitalRead(A1);
      sensorvalue2 = digitalRead(A2);
      sensorvalue3 = digitalRead(A3);

      Serial.print("Sens 1 : ");
      Serial.print(sensorvalue0);
      Serial.print(" Sens 2 : ");
      Serial.println(sensorvalue1);


      if ((sensorvalue0 == LOW) || (sensorvalue1 == LOW)) {  // Se non viene rilevata la presenza di un cuscinetto
        //delay(3000);
        Serial.println("OK");
        digitalWrite(LED_D0, HIGH);
        digitalWrite(D0, HIGH);  // Accende la cassa per la vibrazione + Lampada Gialla
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

        /*
  if (diff >= 2000) {
    Serial.print("STACCA!!!!");
    digitalWrite(D1, LOW); 
    delay (2000);
    sw1 = 0;
  }

  if (sw1 == 1) { 
    delay(500);
    digitalWrite(D1, HIGH);
  } else {
    digitalWrite(D1, LOW);
  
*/
        if (sw1 == 1 && diff <= 2000) {
          digitalWrite(D1, HIGH);  // Attiva Pistone
        }
        if (sw1 == 1 && diff > 2000 && diff <= 4000) {  // Disattiva Pistone
          digitalWrite(D1, LOW);
        }
        if (diff > 4000) {
          sw1 = 0;
        }
      } else {
        Serial.println("OFF");
        digitalWrite(LED_D0, LOW);
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
      }

      Serial.print(" sw : ");
      Serial.println(sw);
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

  digitalWrite(led3Pin, coilValue ? HIGH : LOW);
}