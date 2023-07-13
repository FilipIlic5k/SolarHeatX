
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

#include "SPIFFS.h"
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

String FirmwareVer = { "2.0" };

#define HTTP_PORT 80

// Init Server
AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws");

struct Led {
    // state variables
    uint8_t pin;
    bool on;

    // methods
    void update() { digitalWrite(pin, on ? HIGH : LOW); }
};

Led onboard_led = { LED_BUILTIN, false };

void initSPIFFS()
{
    if (!SPIFFS.begin()) {
        Serial.println("Cannot mount SPIFFS volume...");
        while (1) {
            onboard_led.on = millis() % 200 < 50;
            onboard_led.update();
        }
    }
}

String processor(const String& var)
{
    /*
      if (var == "STATE") {
          if (digitalRead(led.on)) {
              ledState = "ON";
          } else {
              ledState = "OFF";
          }
          return ledState;
      } else if (var == "TEMP_SOLAR") {
          return tempProcessor1();
      } else if (var == "TEMP_PUFFER") {
          return tempProcessor2();
      } else if (var == "TEMP_BOILER") {
          return tempProcessor3();
      }*/
    return "No Data";
}

// Send Html site
void onRootRequest(AsyncWebServerRequest* request)
{
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
}

void initWebServer()
{
    server.on("/", onRootRequest);
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
}

void notifyClients()
{
    const uint8_t size = JSON_OBJECT_SIZE(10);
    StaticJsonDocument<size> json;
    /*
    json["status"] = led.on ? "on" : "off";
    json["TEMP_SOLAR"] = tempProcessor1();
    json["TEMP_PUFFER"] = tempProcessor2();
    json["TEMP_BOILER"] = tempProcessor3();
    */
    char data[500];
    size_t len = serializeJson(json, data);
    ws.textAll(data, len);
}

// Message Handler
void handleWebSocketMessage(void* arg, uint8_t* data, size_t len)
{
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

        const uint8_t size = JSON_OBJECT_SIZE(1);
        StaticJsonDocument<size> json;
        DeserializationError err = deserializeJson(json, data);
        if (err) {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
            return;
        }

        const char* action = json["action"];
        if (strcmp(action, "toggle") == 0) {
            // led.on = !led.on;
            notifyClients();
        }
    }
}

void onEvent(
    AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len)
{

    switch (type) {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

// Initializes a WebSocket
void initWebSocket()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

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

    delay(500);
    initSPIFFS();
    initWebSocket();
    initWebServer();
}

void loop()
{

    // repeatedCall();
    notifyClients();

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

    ws.cleanupClients();
    delay(1000);
}
