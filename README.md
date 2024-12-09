## TxRx Library

### Helps transfer JSON data between esp32 arduino and p5js sketches using different interfaces.

---

## Dependencies:

### Arduino

- [ArduinoJson](https://arduinojson.org/)
- [arduino-esp32](https://github.com/espressif/arduino-esp32/)
- [ArduinoBLE](https://www.arduino.cc/reference/en/libraries/arduinoble/)

### P5js

- [p5.webserial](https://github.com/gohai/p5.webserial)
- [p5.ble](https://itpnyu.github.io/p5ble-website/)

## Use:

### Arduino

Include the header file for the interface you would like to use and instantiate a corresponding object:

```cpp
#include <SerialTxRx.h>
SerialTxRx mTxRx;
```

OR

```cpp
#include <WiFiTxRx.h>
WiFiTxRx mTxRx(String("WiFiID"), String("WiFiPassword"));
```

OR

```cpp
#include <BLETxRx.h>
BLETxRx mTxRx(String("00000000-0000-0000-0000-000000000000"));
```

In `setup()`: init the object, register the pins you'd like to monitor and register a callback for any received data:

```cpp
mTxRx.init(9600);

mTxRx.registerDigital(0);
mTxRx.registerDigital(1);
// etc

mTxRx.registerAnalog(A0);
mTxRx.registerAnalog(A3);
// etc

mTxRx.registerOnReceive([](String s) {
  Serial.println(s);
});
```

In `loop()`: call `txRx()` to update/process any sends/receives:

```cpp
mTxRx.txRx();
```

### P5js

Include the library file in `index.html`:

```html
<script src="path/to/TxRx.js"></script>
```

In `setup()`: instantiate the interface you would like to use, along with a callback to process received data (this might have to happen in a function invoked by the user):

```js
mTxRx = new SerialTxRx(9600);
mTxRx.registerOnReceive((data) => print(data));
```

OR

```js
mTxRx = new WiFiTxRx("http://arduino.ip.address");
mTxRx.registerOnReceive((data) => print(data));
```

OR

```js
mTxRx = new BLETxRx("00000000-0000-0000-0000-000000000000");
mTxRx.registerOnReceive((data) => print(data));
```

In `draw()`: Check for any received data:

```js
mTxRx.rx();
```

When appropriate, send data:

```js
mTxRx.tx({ data: { x: mouseX, y: mouseY } });
```
