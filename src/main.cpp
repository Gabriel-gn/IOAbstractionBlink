#include <Arduino.h>
#include <IoAbstraction.h>

const int ledPin = LED_BUILTIN;
int ledState = HIGH;
IoAbstractionRef ioDevice = ioUsingArduino();

void toggle() {
    ledState = !ledState;  // toggle the LED state.
	ioDeviceDigitalWriteS(ioDevice, ledPin, ledState);
	//Serial.println(ledOn);
}

void setup() {
	Serial.begin(115200);

    ioDevicePinMode(ioDevice, ledPin, OUTPUT); // define como output o LED_BUILTIN
    ioDeviceDigitalWriteS(ioDevice, ledPin, ledState); // define o ledPin como ligado. Lógica inversa no esp8266. 

    taskManager.scheduleFixedRate(1000, toggle); //chama o toggle a cada 1 segundo
}

void loop() {
    // this is all we should do in loop when using task manager.
    taskManager.runLoop();
}