#include "mqtt.h"

WiFiClient espClient;
JsonDocument doc;

const char* ssid = "Rang Dong Lab_2G";
const char* password = "ktdt123456";
PubSubClient client(espClient);
String id = "123456";

const char *mqtt_broker= "192.168.31.41";
const char *topic_pub = "BLE_Scan/";
// const char *topic_sub = "pi/esp32/123456";
const char *mqtt_username = "admin";
const char *mqtt_password = "admin";
const int mqtt_port = 1883;

void callback(char *topic, byte *payload, unsigned int length);
static void Mqtt_Send(String message);

/**
 * @brief Connect Wifi and Mqtt
 * 
 */
void Mqtt_Init(void) 
{
  // Setup Wifi
  while (WiFi.status() != WL_CONNECTED){
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    delay(1000);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup Mqtt
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  // Mqtt Connecting
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the mqtt broker\n", client_id.c_str());
    if(client.connect(client_id.c_str(), mqtt_username, mqtt_password)){
    //   Serial.print("Subscribe:");
    //   Serial.println(topic_sub);
    //   client.subscribe(topic_sub);
      client.publish(topic_pub, "12345");
    } 
    else{
      Serial.print("failed with state ");
      Serial.print(client.state());
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  // Serial.print(". Message: ");
  String message_str = (const char*) payload;
  // Serial.println(message_str);
//   Message_Receive(message_str);
}

bool Mqtt_Check(void){
  return client.connected();
} 

void Mqtt_Reconnect(void){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    //   client.subscribe(topic_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void Mqtt_Send(String message){
  client.publish(topic_pub, message.c_str());
}

void Mqtt_Listen(void){
  client.loop();
}

String Send_Message(String Address, int Rssi)
{
    String message = "{\"action\": \"Put\", \"RSSI\" :[]}";
    doc.clear();
    deserializeJson(doc, message);
    doc["RSSI"][0] = Rssi;
    serializeJson(doc, message); 
    String full_topic = topic_pub + Address;
    client.publish(full_topic.c_str(), message.c_str());
    return message;
}