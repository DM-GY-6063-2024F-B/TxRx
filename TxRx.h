#pragma once

#include <ArduinoJson.h>

class TxRx {

private:
  int analogCount = 0;
  int digitalCount = 0;
  int analogPins[NUM_ANALOG_INPUTS];
  int digitalPins[NUM_DIGITAL_PINS];
  String jsonTextTx;

protected:
  std::function< void(String) > onReceiveFn;

protected:
  TxRx() {}

public:
  virtual void init(int baudRate) = 0;
  virtual void txRx() = 0;

  void startSerial(int baudRate) {
    Serial.begin(baudRate);
    while (!Serial) {}
  }

  void registerAnalog(int pin) {
    if (analogCount >= NUM_ANALOG_INPUTS) return;
    if (pin == RX || pin == TX) return;

    analogPins[analogCount] = pin;
    analogCount++;
  }

  void registerDigital(int pin) {
    if (digitalCount >= NUM_DIGITAL_PINS) return;
    if (pin == RX || pin == TX) return;

    pinMode(pin, INPUT);
    digitalPins[digitalCount] = pin;
    digitalCount++;
  }

  void registerOnReceive(std::function< void(String) >&& fn) {
    onReceiveFn = fn;
  }


protected:
  const String& getJsonTextTx() {
    StaticJsonDocument<960> mJson;
    JsonObject data = mJson.createNestedObject("data");

    for (int di = 0; di < digitalCount; di++) {
      int pin = digitalPins[di];
      data[String("D") + String(pin)] = digitalRead(pin);
    }

    for (int ai = 0; ai < analogCount; ai++) {
      int pin = analogPins[ai];
      data[String("A") + String(pin)] = analogRead(pin);
    }

    jsonTextTx = "";
    serializeJson(mJson, jsonTextTx);

    return jsonTextTx;
  }
};
