#define LED_PIN 2
#define BUTTON_PIN A0

int pezzi = 0;
unsigned long momento;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Simulazione avviata...");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    digitalWrite(LED_PIN, HIGH);
    momento = millis();

    // Resta in attesa finché è premuto
    while (digitalRead(BUTTON_PIN) == LOW) {
      if ((millis() - momento) > 1250) {
        Serial.println("Errore: Ritardo dell'avanzamento pezzo");
        digitalWrite(LED_PIN, LOW);
        
        // Aspetta rilascio
        while (digitalRead(BUTTON_PIN) == LOW);
        delay(50); // debounce
        return;
      }
    }

    digitalWrite(LED_PIN, LOW);
    pezzi++;
    Serial.print("Pezzi: ");
    Serial.println(pezzi);

    delay(150); // debounce
  }
}
