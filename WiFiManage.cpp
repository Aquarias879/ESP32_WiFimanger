// WiFiConfig.cpp
#include "WiFiManage.h"

WiFiConfig::WiFiConfig(const char* host) : _host(host),_server(80) {
    _style =  "<style>#file-input, input { width: 100%; height: 44px; border-radius: 4px; margin: 10px auto; font-size: 15px; }"
          "input { background: #f1f1f1; border: 0; padding: 0 15px; }"
          "body { background: #3498db; font-family: sans-serif; font-size: 14px; color: #777; }"
          "#file-input { padding: 0; border: 1px solid #ddd; line-height: 44px; text-align: left; display: block; cursor: pointer; }"
          "#bar, #prgbar { background-color: #f1f1f1; border-radius: 10px; }"
          "#bar { background-color: #3498db; width: 0%; height: 10px; }"
          "form { background: #fff; max-width: 258px; margin: 75px auto; padding: 30px; border-radius: 5px; text-align: left; }"
          ".btn { background: #3498db; color: #fff; cursor: pointer; }</style>";

    /*"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
              "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
              "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
              "#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
              "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
              ".btn{background:#3498db;color:#fff;cursor:pointer}</style>";*/

    _config = "<form name='wifiConfigForm' action='/config' method='post'>"
              "<h1>ESP32 WiFi Configuration</h1>"
              "<label for='apiHostname'>API Hostname:</label>"
              "<input type='text' id='apiHostname' name='apiHostname' required><br><br>"
              "<label for='ssid'>SSID:</label>"
              "<input type='text' id='ssid' name='ssid' required><br><br>"
              "<label for='password'>Password:</label>"
              "<input type='password' id='password' name='password' required><br><br>"
              "<input type='submit' class='btn' value='Submit'>"
              "</form>" + _style;

    _loginIndex =  "<form name=loginForm>"
                    "<h1>ESP32 Login</h1>"
                    "<input name=userid placeholder='User ID'> "
                    "<input name=pwd placeholder=Password type=Password> "
                    "<input type=submit onclick=check(this.form) class=btn value=Login></form>"
                    "<script>"
                    "function check(form) {"
                    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
                    "{window.open('/serverIndex')}"
                    "else"
                    "{alert('Error Password or Username')}"
                    "}"
                    "</script>" + _style;

    _serverIndex =  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
                    "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
                    "<h1>upload your firmware file(.bin)</h1>"
                    "<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
                    "<label id='file-input' for='file'>   Choose file...</label>"
                    "<input type='submit' class=btn value='Update'>"
                    "<br><br>"
                    "<div id='prg'></div>"
                    "<br><div id='prgbar'><div id='bar'></div></div><br></form>"
                    "<script>"
                    "function sub(obj){"
                    "var fileName = obj.value.split('\\\\');"
                    "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
                    "};"
                    "$('form').submit(function(e){"
                    "e.preventDefault();"
                    "var form = $('#upload_form')[0];"
                    "var data = new FormData(form);"
                    "$.ajax({"
                    "url: '/update',"
                    "type: 'POST',"
                    "data: data,"
                    "contentType: false,"
                    "processData:false,"
                    "xhr: function() {"
                    "var xhr = new window.XMLHttpRequest();"
                    "xhr.upload.addEventListener('progress', function(evt) {"
                    "if (evt.lengthComputable) {"
                    "var per = evt.loaded / evt.total;"
                    "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
                    "$('#bar').css('width',Math.round(per*100) + '%');"
                    "}"
                    "}, false);"
                    "return xhr;"
                    "},"
                    "success:function(d, s) {"
                    "console.log('success!') "
                    "},"
                    "error: function (a, b, c) {"
                    "}"
                    "});"
                    "});"
                    "</script>" + _style;
}


void WiFiConfig::handleWifiConfig() {
    String ssid = _server.arg("ssid");  // Use the actual name specified in the HTML form
    String password = _server.arg("password");  // Use the actual name specified in the HTML form
    String apiHostname = _server.arg("apiHostname");  // Use the actual name specified in the HTML form

    saveCredentials(ssid, password, apiHostname);

    WiFi.begin(ssid.c_str(), password.c_str());

    int maxAttempts = 50;
    for (int attempts = 0; attempts < maxAttempts && WiFi.status() != WL_CONNECTED; attempts++) {
        delay(1000);
        Serial.println("Connecting to " + ssid);
    }

    if (WiFi.status() == WL_CONNECTED) {
        WiFi.softAPdisconnect(true);
        Serial.println("Connected to " + ssid);
        Serial.println("IP address: " + WiFi.localIP().toString());
        Serial.println("API Hostname: " + apiHostname);
    }
}


void WiFiConfig::autoconnect() {
    String savedSSID, savedPassword, savedAPIHostname;
    loadCredentials(savedSSID, savedPassword);
    WiFi.begin(savedSSID.c_str(), savedPassword.c_str());
    
    Serial.print("Connecting to " + savedSSID + "");

    const int maxAttempts = 30;
    for (int attempts = 0; attempts < maxAttempts && WiFi.status() != WL_CONNECTED; ++attempts) {
        delay(1000);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected to " + savedSSID + "");
        Serial.println("IP address: " + WiFi.localIP().toString());
        Serial.println("API Hostname: " + savedAPIHostname);
    } else {
        Serial.println("\nNot Found Saved SSID");
    }
}

void WiFiConfig::ConnectMode() {
    if (WiFi.status() != WL_CONNECTED) {
        startSoftAP();
    }
}

void WiFiConfig::startSoftAP() {
    WiFi.softAP(PARAM_SSID, PARAM_PASSWORD);

    for (; WiFi.softAPgetStationNum() == 0; delay(1000)) {
        Serial.println("Waiting for a client to connect...");
    }

    setupServer();
}
void WiFiConfig::setupServer() {
    _server.on("/", HTTP_GET, [this]() {
        _server.send(200, "text/html", _config);
    });

    _server.on("/config", HTTP_POST, [this]() {
        _server.send(200, "text/html", _config);
        handleWifiConfig();  // Process form data

        // Stop the server after processing the form data
        _server.stop();
    });

    _server.begin();
    while (true) {
        _server.handleClient();
        if (WiFi.status() == WL_CONNECTED) {
            break;
        }
        delay(10);
    }
}

void WiFiConfig::loadAPIHostname(String &apiHostname) {
    EEPROM.begin(128);
    apiHostname = "";

    for (int i = 0; i < 64; ++i) {
        char c = EEPROM.read(apiHostnameAddress + i);
        if (c == '\0') {
            break;
        }
        apiHostname += c;
    }

    EEPROM.end();
}

void WiFiConfig::loadCredentials(String &ssid, String &password) {
    EEPROM.begin(64);
    ssid = "";
    password = "";

    for (int i = 0; i < 32; ++i) {
        char c = EEPROM.read(ssidAddress + i);
        if (c == '\0') {
            break;
        }
        ssid += c;
    }

    for (int i = 0; i < 64; ++i) {
        char c = EEPROM.read(passwordAddress + i);
        if (c == '\0') {
            break;
        }
        password += c;
    }

    EEPROM.end();
}

void WiFiConfig::saveCredentials(const String& ssid, const String& password, const String& apiHostname) {
    EEPROM.begin(128);

    // Save SSID
    for (int i = 0; i < ssid.length(); ++i) {
        EEPROM.write(ssidAddress + i, ssid[i]);
    }
    EEPROM.write(ssidAddress + ssid.length(), '\0');

    // Save password
    for (int i = 0; i < password.length(); ++i) {
        EEPROM.write(passwordAddress + i, password[i]);
    }
    EEPROM.write(passwordAddress + password.length(), '\0');

    // Save API Hostname
    for (int i = 0; i < apiHostname.length(); ++i) {
        EEPROM.write(apiHostnameAddress + i, apiHostname[i]);
    }
    EEPROM.write(apiHostnameAddress + apiHostname.length(), '\0');

    EEPROM.commit();
    EEPROM.end();
}

void WiFiConfig::handleOTA(){
  if (!MDNS.begin(_host)) { // http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

    /* use mdns for host name resolution 
  
  /* return index page which is stored in serverIndex */
  _server.on("/", HTTP_GET, [this]() {
    _server.sendHeader("Connection", "close");
    _server.send(200, "text/html", _config);
  });

  _server.on("/serverIndex", HTTP_GET, [this]() {
    _server.sendHeader("Connection", "close");
    _server.send(200, "text/html", _serverIndex);
  });

  _server.on("/upload", HTTP_GET, [this]() {
    _server.sendHeader("Connection", "close");
    _server.send(200, "text/html", _loginIndex); // Replace this with your custom HTML content
  });

  /* handling uploading firmware file */
  _server.on("/update", HTTP_POST, [this]() {
    _server.sendHeader("Connection", "close");
    _server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, [this]() {
    HTTPUpload& upload = _server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { // start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP */
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { // true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });

  _server.begin();

}

void WiFiConfig::handleClient() {
  _server.handleClient();
}


void WiFiConfig::handleClientStop(){
  _server.stop();
  MDNS.end();
}