#ifndef PINS_H
#define PINS_H

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

bool canBeOutput(uint8_t pin) { return pin < 34; }

// Define your relay pins here
const uint8_t RELAY_PINS[] = { 32, 25, 26, 27, 14, 12, 13 };

// Assign names to each relay
enum RelayNames {
    RELAY_1 = 0,
    RELAY_2,
    RELAY_3,
    RELAY_4,
    RELAY_5,
    RELAY_6,
    RELAY_7,
    RELAY_8,
    NUM_RELAYS // This keeps track of the number of relays
};

int pinModeRelays()
{
    for (int i = 0; i < NUM_RELAYS; i++) {
        if (canBeOutput(RELAY_PINS[i])) {
            pinMode(RELAY_PINS[i], OUTPUT);
            digitalWrite(RELAY_PINS[i], LOW);
        } else {
            Serial.print("Pin ");
            Serial.print(RELAY_PINS[i]);
            Serial.println(" cannot be output");
            return 1;
        }
    }
    return 0;
}

void initRelays()
{
    for (int i = 0; i < NUM_RELAYS - 1; i++) {
        digitalWrite(RELAY_PINS[i], LOW);
        delay(300);
        digitalWrite(RELAY_PINS[i], HIGH);
    }
}

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 15

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

int deviceCount = 0;
float tempC;

void fetchSensorAddresses()
{
    Serial.print("Locating devices...");
    Serial.print("Found ");
    deviceCount = sensors.getDeviceCount();
    Serial.print(deviceCount, DEC);
    Serial.println(" devices.");
    Serial.println("");
}

// Define your pins here
const uint8_t BUTTON_PIN = 0;
const uint8_t LED_PIN = LED_BUILTIN;

#endif
