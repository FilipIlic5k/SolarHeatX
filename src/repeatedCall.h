#ifndef REPEATEDCALL_H
#define REPEATEDCALL_H

#include "firmwareFetch.h"
#include "firmwareUpdate.h"
#include "wifiConnect.h"

unsigned long previousMillis = 0; // will store last time LED was updated
unsigned long previousMillis_2 = 0;
const long interval = 12000;
const long mini_interval = 1000;

void repeatedCall()
{
    static int num = 0;
    unsigned long currentMillis = millis();
    if ((currentMillis - previousMillis) >= interval) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        if (FirmwareVersionCheck()) {
            firmwareUpdate();
        }
    }
    if ((currentMillis - previousMillis_2) >= mini_interval) {
        previousMillis_2 = currentMillis;
        Serial.print("idle loop...");
        Serial.print(num++);
        Serial.print(" Active fw version:");
        Serial.println(FirmwareVer);
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("wifi connected");
        } else {
            connect_wifi();
        }
    }
}

#endif