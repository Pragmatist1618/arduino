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
    String received = rs485Serial.readStringUntil('\n');  // Чтение данных из последовательного порта
    Serial.print("Received: ");
    Serial.println(received);  // Отправка ответа обратно в последовательный порт
    rs485Serial.print("Echo: ");
    rs485Serial.println(received);  // Отправка эхо-ответа обратно
  }
}
