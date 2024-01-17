#include <WiFiManage.h>

const int modeSelectPin = 5;
int counter = 1;
int lastButtonState = LOW;

const char* host = "esp32";
WiFiConfig config(host);

void setup() {
  Serial.begin(9600);
  pinMode(modeSelectPin, INPUT_PULLDOWN);
  config.autoconnect();
  config.ConnectMode();
  config.handleOTA();
}

void loop() {
  int buttonState = digitalRead(modeSelectPin);

  if (buttonState != lastButtonState) {
    handleButtonStateChange(buttonState);
    delay(50);
  }

  lastButtonState = buttonState;

  switch (counter) {
    case 1:
      
      break;
    case 2:
      handleMode1();
    case 3:
      config.handleClient();
      break;
  }
}

void handleButtonStateChange(int buttonState) {
  if (buttonState == HIGH) {
    counter = (counter % 3) + 1;
    Serial.print("Number of button pushes: ");
    Serial.println(counter);
  }
}

void handleMode1() {
  Serial.println("Mode 1");
}
