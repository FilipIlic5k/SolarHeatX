#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <WiFi.h>
#include <credentials.h>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PW;

void connect_wifi()
{
    Serial.println("Waiting for WiFi");
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
