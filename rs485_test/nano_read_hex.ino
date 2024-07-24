#include <SoftwareSerial.h>

// Определите пины для программного последовательного порта
#define RX_PIN 11
#define TX_PIN 9

// Создайте объект SoftwareSerial
SoftwareSerial rs485Serial(RX_PIN, TX_PIN);

void setup() {
  // Настройте скорость аппаратного последовательного порта
  Serial.begin(9600);
  
  // Настройте скорость программного последовательного порта
  rs485Serial.begin(9600);
  
  Serial.println("RS485 Receiver ready");
}

void loop() {
  // Проверка наличия данных в буфере RS485
  if (rs485Serial.available()) {
    while (rs485Serial.available()) {
      int byteReceived = rs485Serial.read();
      Serial.print("0x");
      if (byteReceived < 16) {
        Serial.print("0");
      }
      Serial.print(byteReceived, HEX);
      Serial.print(" ");
    }
    Serial.println(); // Перенос строки после получения пакета
  }
}
