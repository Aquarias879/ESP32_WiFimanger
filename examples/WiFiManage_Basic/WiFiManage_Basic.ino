#include <WiFiManage.h>

String api;
const char* host = "esp32";

WiFiConfig cf(host);
const int buttonPin = 5;  // Pin number where the push button is connected
int buttonState = HIGH;   // Current state of the button
int lastButtonState = HIGH; // Previous state of the button
unsigned long startTime = 0; // Time when the button is pressed
unsigned long elapsedTime = 0; // Elapsed time since the button press
const unsigned long debounceTime = 50; // Debounce time to avoid multiple readings due to button jitter
unsigned long lastPrintTime = 0; // Last time the API hostname was printed
const unsigned long printInterval = 60000; // Print interval in milliseconds (1 minute)

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  cf.autoconnect();
  cf.ConnectMode();  
  cf.handleOTA();
}

void loop() {
  // Read the state of the button
  cf.handleClient();
  // Print API hostname every 1 minute
  if (millis() - lastPrintTime >= printInterval) {
    cf.loadAPIHostname(api);
    Serial.println(api.c_str());
    lastPrintTime = millis(); // Update last print time
  }

  delay(1);
}
