int ledPins[] = {3, 4, 5, 6, 7};

void setup() {
  // Инициализация пинов для светодиодов как выходы
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(9600); // Инициализация последовательного порта
}

void loop() {
  if (Serial.available() > 0) {
    int value = Serial.parseInt(); // Считывание полученного значения
    Serial.print("Received value: ");
    Serial.println(value);
    for (int i = 0; i < 5; i++) {
      if (value == i + 1) {
        digitalWrite(ledPins[i], HIGH);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }
}
