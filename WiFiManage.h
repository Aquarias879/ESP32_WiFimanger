// WiFiConfig.h
#ifndef WIFIMANAGE_H
#define WIFIMANAGE_H

#include <WiFi.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <Update.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>


class WiFiConfig {
public:
	WiFiConfig(const char* host);
	void autoconnect();
	void ConnectMode();
	void loadAPIHostname(String& apiHostname);
	void handleOTA();
	void handleClient();
	void handleClientStop();

private:
    void saveCredentials(const String& ssid, const String& password, const String& apiHostname);
    void handleRoot();
    void handleWifiConfig(); 
	void loadCredentials(String& ssid, String& password);
	void setupServer();
	void startSoftAP();

	const char* PARAM_SSID = "ESP32-Config";
	const char* PARAM_PASSWORD = "12345678";
	const int ssidAddress = 0;
	const int passwordAddress = 32;
    const int apiHostnameAddress = 96;
	const char* _host;
	WebServer _server;
	String _config,_style,_loginIndex,_serverIndex;
};
#endif  // WIFICONFIG_H
