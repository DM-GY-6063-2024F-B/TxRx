#include "TxRx.h"

class SerialTxRx : public TxRx {

private:
  bool hasConnected{false};
  long lastConnected{0};

public:
  SerialTxRx() {}

  void init(int baudRate) {
    startSerial(baudRate);
  }

  void txRx() {
    if(!hasConnected && millis() - lastConnected > 500) {
      Serial.println("A");
      Serial.flush();
      lastConnected = millis();
    }

    if (Serial.available() > 0) {
      hasConnected = true;
      int byteIn = Serial.read();
      if (byteIn == 'G') {
        Serial.flush();
        String jsonTextTx = getJsonTextTx();
        Serial.println(jsonTextTx);
      } else if (byteIn == 'P' && onReceiveFn) {
        String jsonTextRx = Serial.readStringUntil('\n');
        onReceiveFn(jsonTextRx);
      }
    }
  }
};
