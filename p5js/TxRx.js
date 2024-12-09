class BLETxRx {
  mBLE;
  txCharacteristic;
  rxCharacteristic;

  requestData;
  onReceiveFn = null;

  // https://developer.mozilla.org/en-US/docs/Web/API/Bluetooth/requestDevice
  constructor(serviceUUID) {
    this.mBLE = new p5ble();
    this.mBLE.connect(serviceUUID, this.gotCharacteristics);

    this.requestData = false;
  }

  gotCharacteristics = (error, characteristics) => {
    if (error) {
      print("error: ", error);
      return;
    }

    for (let i = 0; i < characteristics.length; i++) {
      let mC = characteristics[i];

      if (mC.properties.write) {
        this.txCharacteristic = characteristics[i];
      }

      if (mC.properties.read) {
        this.rxCharacteristic = characteristics[i];
      }
    }

    this.readyToRead = true;
  };

  gotValue = (error, data) => {
    if (error || data.charAt(0) != "{") {
      print("error: ", error || data);
      this.readyToRead = true;
      return;
    }

    if (this.onReceiveFn) {
      this.onReceiveFn(data);
    }
    this.readyToRead = true;
  };

  registerOnReceive(fn) {
    this.onReceiveFn = fn;
  }

  rx() {
    if (this.rxCharacteristic && this.readyToRead) {
      this.readyToRead = false;
      this.mBLE.read(this.rxCharacteristic, "string", this.gotValue);
    }
  }

  tx(data) {
    if (this.txCharacteristic) {
      this.mBLE.write(this.txCharacteristic, JSON.stringify(data));
    }
  }
}

class WiFiTxRx {
  serverAddress;

  requestData;
  onReceiveFn = null;

  constructor(serverAddress) {
    this.serverAddress = serverAddress;
    this.requestData = true;
  }

  handleResponse = (res) => {
    let data = res.data;
    if (this.onReceiveFn) {
      this.onReceiveFn(data);
    }
    this.requestData = true;
  }

  registerOnReceive(fn) {
    this.onReceiveFn = fn;
  }

  rx() {
    if (this.requestData) {
      this.requestData = false;
      loadJSON(this.serverAddress + "/data", this.handleResponse);
    }
  }

  tx(data) {
    httpPost(this.serverAddress + "/data", data);
  }
}

class SerialTxRx {
  mSerial;

  lastRequest;
  requestConnect;
  requestData;
  onReceiveFn = null;

  constructor(baudRate) {
    this.mSerial = createSerial();
    this.mSerial.open(baudRate);
    this.requestData = false;
    this.requestConnect = true;
    this.lastRequest = millis();
  }

  readSerial() {
    let line = this.mSerial.readUntil("\n");
    line = trim(line);
    if (!line) return;

    if (line.length > 0) {
      this.requestConnect = false;
    }

    if (line.charAt(0) != "{") {
      print("error: ", line);
      this.requestData = true;
      return;
    }

    let data = JSON.parse(line).data;
    if (this.onReceiveFn) {
      this.onReceiveFn(data);
    }
    this.requestData = true;
  }

  registerOnReceive(fn) {
    this.onReceiveFn = fn;
  }

  rx() {
    if (this.mSerial.opened()) {
      let now = millis();
      let tryConnect = this.requestConnect && now - this.lastRequest > 500;
      let tryData = !this.requestConnect && this.requestData;

      if (tryConnect || tryData) {
        this.requestData = false;
        this.mSerial.clear();
        this.mSerial.write("G");
        this.lastRequest = millis();
      } else if (this.mSerial.availableBytes() > 0) {
        this.readSerial();
      }
    }
  }

  tx(data) {
    if (this.mSerial.opened()) {
      this.mSerial.write("P");
      this.mSerial.write(JSON.stringify(data));
      this.mSerial.write("\n");
    }
  }
}
