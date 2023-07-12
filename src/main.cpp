#include "cert.h"
#include "firmwareFetch.h"
#include "firmwareUpdate.h"
#include "pins.h"
#include "repeatedCall.h"
#include "tempSensor.h"
#include "wifiConnect.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>

String FirmwareVer = { "2.0" };

void setup()
{
    Serial.begin(115200);
    Serial.print("Active firmware version:");
    Serial.println(FirmwareVer);

    connect_wifi();
    /*
    pinModeRelays();
    initRelays();
    */

    sensors.begin();
    fetchSensorAddresses();
}

void loop()
{

    // repeatedCall();

    // Send command to all the sensors for temperature conversion
    sensors.requestTemperatures();

    // Display temperature from each sensor
    for (int i = 0; i < deviceCount; i++) {
        Serial.print("Sensor ");
        Serial.print(i + 1);
        Serial.print(" : ");
        tempC = sensors.getTempCByIndex(i);
        Serial.print(tempC);
        Serial.print((char)176); // shows degrees character
        Serial.print("C  |  ");
        Serial.print(DallasTemperature::toFahrenheit(tempC));
        Serial.print((char)176); // shows degrees character
        Serial.println("F");
    }

    Serial.println("");
    delay(1000);
}
