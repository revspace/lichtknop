/*
    It's finally here!
    Edit by f0x, original code from Fastled, Sebastius, Juerd
*/

#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>

#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/2.4

boolean reconnect();

// WiFi settings
char ssid[] = "revspace-pub-2.4ghz";    //    your network SSID (name)
char pass[] = "";             // your network password

// MQTT Server settings and preparations
const char* mqtt_server = "mosquitto.space.revspace.nl";
WiFiClient espClient;

PubSubClient client(mqtt_server, 1883, NULL, espClient);
long lastReconnectAttempt = 0;

void setup() {
    WiFi.mode(WIFI_STA);
    Serial.begin(115200);
    Serial.println();
    Serial.println(ESP.getChipId());


    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
        Serial.print(".");
    }
    Serial.println("");

    Serial.print("WiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("MQTT name: ");
    Serial.println("lightstate-" ROOM);
}

boolean reconnect() {
    client.setKeepAlive(5);
    if (client.connect("lightstate-" ROOM, NULL, NULL, "revspace/lightstate/" ROOM, 0, true, "off")) {
        client.publish("revspace/lightstate/" ROOM, "on", true);
        client.loop();
    }
    return client.connected();
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
