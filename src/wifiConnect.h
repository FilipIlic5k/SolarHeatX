#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <WiFi.h>
#include <credentials.h>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PW;

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 187);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

void connect_wifi()
{
    Serial.println("Waiting for WiFi");
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

#endif
