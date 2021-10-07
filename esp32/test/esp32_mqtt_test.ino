#include "WiFi.h"
#include <PubSubClient.h>
#include <ESP32Servo.h>

/////////////////// SETTINGS /////////////////////////////

// Wi-Fi
const char* ssid = "HONOR 20 PRO";
const char* password = "Artyom123456710";

// MQTT
const char* mqtt_server = "M8.WQTT.RU";
const int mqtt_port = 10322;
const char* mqtt_user = "u_68RFXO";
const char* mqtt_password = "HErq0ou5";

// RELAY
const String relay_topic = "test/socket";
#define relay_status_topic "test/socket/status"
const int RELAY = 2;

//SERVO
#define servo_topic "test/servo"
const String servo_control_topic = "test/servo/control";
#define servo_status_topic "test/servo/status"
#define servo_info_topic "test/servo/info"
const int SERVO = 12;

/////////////////////////////////////////////////////////



WiFiClient espClient;
PubSubClient client(espClient);
Servo myservo;

bool relay_on = false;
int angle = 0;



void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void updateStatePins(void){
//  RELAY
    if(relay_on){
      digitalWrite(RELAY, HIGH);
    }else{
      digitalWrite(RELAY, LOW);
    }
}

void servoGo(int angle)
{
    myservo.write(angle);    // tell servo to go to position in variable 'pos'
    delay(15);  
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(String(topic) + ": ");
   
  String data_pay;
  for (int i = 0; i < length; i++) {
    data_pay += String((char)payload[i]);
  }

    
   Serial.println(data_pay);

    
  if( String(topic) == relay_topic ){
        if(data_pay == "ON") {
          relay_on = true;
          client.publish(relay_status_topic, (char*)"ON");
        }
        if(data_pay == "OFF") {
          relay_on = false;
          client.publish(relay_status_topic, (char*)"OFF");
        }
    }

    if( String(topic) == servo_control_topic ){
      Serial.println("data_pay = " + data_pay);
      angle = data_pay.toInt();
      servoGo(angle); 
      client.publish(servo_info_topic, (char*)data_pay.toInt());
    }
    
    updateStatePins();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266-" + WiFi.macAddress();
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password) ) {
      Serial.println("connected");
      
      client.subscribe( (relay_topic + "/#").c_str() );
      client.subscribe( (servo_control_topic + "/#").c_str() );

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(SERVO, 500, 2400); // attaches the servo on pin 18 to the servo object
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
