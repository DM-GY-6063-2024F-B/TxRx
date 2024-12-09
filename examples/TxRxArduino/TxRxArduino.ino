//
// Pick one type of protocol/interface by uncommenting the corresponding lines below

// #include <SerialTxRx.h>
// SerialTxRx mTxRx;

// #include <WiFiTxRx.h>
// WiFiTxRx mTxRx(String("tgh8114"), String("abcd1234"));

// #include <BLETxRx.h>
// BLETxRx mTxRx(String("e416af28-c3e9-43c2-a538-b2e27d9dcfb4"));

void setup() {
  mTxRx.init(9600);

  mTxRx.registerDigital(0);
  mTxRx.registerDigital(1);
  mTxRx.registerDigital(2);
  mTxRx.registerDigital(3);
  mTxRx.registerDigital(4);
  mTxRx.registerDigital(5);

  mTxRx.registerAnalog(A0);
  mTxRx.registerAnalog(A3);
  mTxRx.registerAnalog(A4);
  mTxRx.registerAnalog(A5);
  mTxRx.registerAnalog(A6);
  mTxRx.registerAnalog(A7);

  mTxRx.registerOnReceive([](String s) {
    Serial.println(s);
  });
}

void loop() {
  mTxRx.txRx();
  delay(2);
}
