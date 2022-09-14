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

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define LIGHTS_OUT_DURATION 25000
#define RELAY_PIN D2

// WiFi settings
char ssid[] = "revspace-pub-2.4ghz";    //    your network SSID (name)
char pass[] = "";             // your network password

// MQTT Server settings and preparations
const char* mqtt_server = "mosquitto.space.revspace.nl";
WiFiClient espClient;

PubSubClient client(mqtt_server, 1883, onMqttMessage, espClient);
long lastReconnectAttempt = 0;

void setup() {
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    Serial.println();
    Serial.println(ESP.getChipId());
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(D4, OUTPUT);
    // pinMode(D0, OUTPUT);

    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
        Serial.print(".");
    }
    Serial.println("");

    Serial.print("WiFi connected");
    Serial.println(ssid);

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

boolean reconnect() {
    if (client.connect("Ledspace")) {
        // Once connected, publish an announcement...
        client.publish("f0x/tubes/01", "online");
        // ... and resubscribe
        client.subscribe("revspace/button/nomz");
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
    if (strcmp(topic, "revspace/button/nomz") == 0) {
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("Turning off lights due to nomz");

        delay(LIGHTS_OUT_DURATION);

        digitalWrite(RELAY_PIN, LOW);
        Serial.println("Turning lights back on");
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

    digitalWrite(D4, millis() % 1000 > 500);
    // digitalWrite(D0, millis() % 1000 <= 500);
}
