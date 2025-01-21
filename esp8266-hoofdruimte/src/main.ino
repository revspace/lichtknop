/*
    It's finally here!
    Edit by f0x, original code from Fastled, Sebastius, Juerd
*/

#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>

#include <PubSubClient.h> //https://github.com/knolleary/pubsubclient/releases/tag/2.4

void onMqttMessage(char* topic, byte * payload, unsigned int length);
boolean reconnect();

#define LIGHTSTATE_L_PIN D0
#define LIGHTSTATE_L_TOPIC "revspace/lightstate/hoofdruimte_tv/set"

#define LIGHTSTATE_R_PIN D1
#define LIGHTSTATE_R_TOPIC "revspace/lightstate/hoofdruimte_bar/set"

// WiFi settings
char ssid[] = "revspace-pub-2.4ghz";    //    your network SSID (name)
char pass[] = "";             // your network password

// MQTT Server settings and preparations
const char* mqtt_server = "10.42.42.1";
WiFiClient espClient;

PubSubClient client(mqtt_server, 1883, onMqttMessage, espClient);
long lastReconnectAttempt = 0;

int8_t lights_l = -1;
int8_t lights_r = -1;

void on(int pin) {
    digitalWrite(pin, LOW);
    delay(20);
    digitalWrite(pin, HIGH);
}

void off(int pin) {
    digitalWrite(pin, HIGH);
    delay(20);
    digitalWrite(pin, LOW);
}

void setup() {
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    Serial.println();
    Serial.println(ESP.getChipId());
    pinMode(LIGHTSTATE_L_PIN, OUTPUT);
    pinMode(LIGHTSTATE_R_PIN, OUTPUT);


    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
        Serial.print(".");
    }
    Serial.println("");

    Serial.println("WiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    reconnect();
}

boolean reconnect() {
    if (client.connect("lightstate-hoofdruimte")) {
        client.subscribe(LIGHTSTATE_L_TOPIC);
        client.subscribe(LIGHTSTATE_R_TOPIC);
        client.loop();
    }
    return client.connected();
}

void onMqttMessage(char* topic, byte * payload, unsigned int length) {
    Serial.print("received topic: ");
    Serial.println(topic);
    Serial.print("length: ");
    Serial.println(length);
    Serial.println();

    // NOMZ because we are hungry! Lets join the blinking lights parade!
    if (strcmp(topic, LIGHTSTATE_L_TOPIC) == 0) {
        if (strncmp((char*) payload, "on", length) == 0) {
            on(LIGHTSTATE_L_PIN);
            return;
        }
        if (strncmp((char*) payload, "off", length) == 0) {
            off(LIGHTSTATE_L_PIN);
            return;
        }
        return;
    }
    if (strcmp(topic, LIGHTSTATE_R_TOPIC) == 0) {
        if (strncmp((char*) payload, "on", length) == 0) {
            on(LIGHTSTATE_R_PIN);
            return;
        }
        if (strncmp((char*) payload, "off", length) == 0) {
            off(LIGHTSTATE_R_PIN);
            return;
        }
        return;
    }
}


void loop() {
    if (!client.connected()) {
        long verstreken = millis();
        if (verstreken - lastReconnectAttempt > 5000) {
            lastReconnectAttempt = verstreken;
            // Attempt to reconnect
            if (reconnect()) {
                lastReconnectAttempt = 0;
            }
        }
    } else {
        // Client connected
        client.loop();
    }
}
