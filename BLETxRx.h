#include <ArduinoBLE.h>

#include "TxRx.h"

class BLETxRx : public TxRx {

private:
  static String randomUUID;
  static String txUUID;
  static String rxUUID;

private:
  String serviceUUID;
  BLEService* mService;
  BLECharacteristic txCharacteristic;
  BLECharacteristic rxCharacteristic;

public:
  BLETxRx(String serviceUUID)
    : serviceUUID(serviceUUID),
      txCharacteristic(txUUID.c_str(), BLERead, 128),
      rxCharacteristic(rxUUID.c_str(), BLEWrite, 128) {}

  void init(int baudRate) {
    startSerial(baudRate);
    String deviceName("TxRx");

    // TODO: move this to constructor? ... kept giving errors
    mService = new BLEService(serviceUUID.c_str());

    BLE.begin();
    BLE.setDeviceName(deviceName.c_str());
    BLE.setLocalName(deviceName.c_str());
    BLE.setAdvertisedService(*mService);
    mService->addCharacteristic(txCharacteristic);
    mService->addCharacteristic(rxCharacteristic);
    BLE.addService(*mService);
    BLE.advertise();
  }

  // https://github.com/arduino-libraries/ArduinoBLE/blob/master/examples/Peripheral/LED/LED.ino
  void txRx() {
    BLEDevice central = BLE.central();
    String jsonTextTx = getJsonTextTx();
    txCharacteristic.writeValue(jsonTextTx.c_str(), jsonTextTx.length());

    if (central && central.connected()) {
      if (rxCharacteristic.written()) {
        String jsonTextRx = String(rxCharacteristic.value(), rxCharacteristic.valueLength());
        if (onReceiveFn) {
          onReceiveFn(jsonTextRx);
        }
      }
    }
  }
};

String BLETxRx::randomUUID = String(random(1000, 9999));
String BLETxRx::txUUID = String("250b0d01-1b4f-dead-bea7-f63c733d" + randomUUID);
String BLETxRx::rxUUID = String("250b0d02-1b4f-dead-bea7-f63c733d" + randomUUID);
