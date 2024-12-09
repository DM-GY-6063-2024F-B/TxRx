#include <WebServer.h>
#include <WiFi.h>

#include "TxRx.h"

class WiFiTxRx : public TxRx {

private:
  WebServer mServer;
  String jsonTextRx;

  String ssid;
  String password;

public:
  WiFiTxRx(String ssid, String password)
    : mServer(80),
      ssid(ssid),
      password(password) {}

  void init(int baudRate) {
    startSerial(baudRate);

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int tryConnectCount = 0;
    while (WiFi.status() != WL_CONNECTED && tryConnectCount < 60) {
      delay(500);
      Serial.print(".");
      tryConnectCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nIP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nWiFi is NOT connected! 💩");
    }

    mServer.enableCORS();

    // inspired by: https://stackoverflow.com/a/29286957
    mServer.on("/data", HTTP_GET, [&]() {
      handleData();
    });

    mServer.on("/data", HTTP_POST, [&]() {
      handlePost();
    });

    mServer.onNotFound([&]() {
      handleNotFound();
    });

    mServer.begin();
  }

  void txRx() {
    mServer.handleClient();
  }

private:
  void handleData() {
    String jsonTextTx = getJsonTextTx();
    mServer.send(200, "application/json", jsonTextTx);
  }

  // WebServer "documentation":
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/src/WebServer.h
  void handlePost() {
    jsonTextRx = String(mServer.arg(0));

    if (onReceiveFn) {
      onReceiveFn(jsonTextRx);
    }

    mServer.send(200, "text/plain", "POSTed");
  }

  // Handle CORS pre-flight on POST requests, according to this:
  // https://stackoverflow.com/a/74116471
  void handleNotFound() {
    if (mServer.method() == HTTP_OPTIONS) {
      mServer.send(200);
    } else {
      mServer.send(404, "text/plain", "404! ADDRESS NOT FOUND");
    }
  }
};
