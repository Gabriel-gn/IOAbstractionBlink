/*
*
*	IMPORTACOES
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
* DECLARACOES GLOBAIS
*
*/

IoAbstractionRef ioDevice = ioUsingArduino();

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

WiFiClient client;
WiFiManager wifiManager;

/*
*
* VARIAVEIS GLOBAIS
*
*/

const int ledPin = LED_BUILTIN;
int ledState = HIGH;
boolean connectionState = false;

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

/*
*
* DEBUG
*
*/

boolean debug = true;

void debugPrint(String message){
    if(debug){
        Serial.print(message);
    }
}

void debugPrintln(String message){
    if(debug){
        Serial.println(message);
    }
}

void ledBlink()
{   
    debugPrint("Estado de conexão: "); debugPrintln(String(connectionState));
    if (connectionState) {
        ledState = HIGH; // não pisca
        ioDeviceDigitalWriteS(ioDevice, ledPin, ledState);
        // Serial.println(ledOn);
    } else {
        ledState = !ledState; // pisca
        ioDeviceDigitalWriteS(ioDevice, ledPin, ledState);
    }
}

/*
*
* CONFIGS WIFI
*
*/

void configureWifiManager(){
    wifiManager.setDebugOutput(debug);
}

void wifiInit()
{   
    configureWifiManager();
    if (!wifiManager.autoConnect("AutoConnectAP")) {
        debugPrintln("Iniciando ponto de acesso AP");
        connectionState = false;
    } else {
        connectionState = true;
        debugPrintln("Conectado ao roteador");
    }
    //wifiManager.resetSettings();
}