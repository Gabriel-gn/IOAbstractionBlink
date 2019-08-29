#include <Arduino.h>
#include <IoAbstraction.h>

const int ledPin = LED_BUILTIN;
int ledOn = HIGH;
IoAbstractionRef ioDevice = ioUsingArduino();


void toggle() {
	ledOn = !ledOn; // toggle the LED state.
}

void setup() {
	ioDevicePinMode(ioDevice, ledPin, OUTPUT);
  ioDeviceDigitalWriteS(ioDevice, ledPin, ledOn);
	taskManager.scheduleFixedRate(1000, toggle);
}


void loop() {
	// this is all we should do in loop when using task manager.
	taskManager.runLoop();
}