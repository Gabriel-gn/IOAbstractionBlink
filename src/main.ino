/*
*
*	IMPORTAÇÕES
*
*/

#include <Arduino.h>
#include <IoAbstraction.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <WiFiClient.h>

#include <DNSServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

/*
*
* DECLARAÇÕES GLOBAIS
*
*/

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

WiFiClient client;

const int ledPin = LED_BUILTIN;
int ledState = HIGH;
IoAbstractionRef ioDevice = ioUsingArduino();

/*
*
* ARQUIVOS ARDUINO
*
*/

void setup()
{
    Serial.begin(115200);

    ioDevicePinMode(ioDevice, ledPin, OUTPUT); // define como output o LED_BUILTIN
    ioDeviceDigitalWriteS(ioDevice, ledPin, ledState); // define o ledPin como ligado. Lógica inversa no esp8266.

    taskManager.scheduleOnce(1000, wifiInit);

    taskManager.scheduleFixedRate(1000, ledBlink); // chama o ledBlink a cada 1 segundo
}

void loop()
{
    // this is all we should do in loop when using task manager.
    taskManager.runLoop();
}

void ledBlink()
{
    ledState = !ledState; // toggle the LED state.
    ioDeviceDigitalWriteS(ioDevice, ledPin, ledState);
    // Serial.println(ledOn);
}

void wifiInit()
{
    WiFiManager wifiManager;
    wifiManager.autoConnect("AutoConnectAP");
    Serial.println("Conectado");
}
