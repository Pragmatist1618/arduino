import serial
import time

# Установка параметров соединения
arduino_port = 'COM6'  # Замените на ваш порт
baud_rate = 9600  # Скорость передачи данных, должна совпадать с настройками Arduino

# Инициализация соединения
ser = serial.Serial(arduino_port, baud_rate, timeout=1)


# Функция для отправки данных
def send_data(data):
    ser.write(data.encode())  # Преобразование строки в байты и отправка


try:
    # Даем время на установку соединения
    time.sleep(2)

    # Отправка данных
    send_data("Hello, Arduino!")
    print("Data sent to Arduino")

    # Опционально: Чтение ответа от Arduino
    while True:
        if ser.in_waiting > 0:
            response = ser.readline().decode('utf-8').rstrip()
            print(f"Received from Arduino: {response}")
            break

finally:
    # Закрытие соединения
    ser.close()
