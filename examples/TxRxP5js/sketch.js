//
// Pick one type of protocol/interface by uncommenting the corresponding lines below

let mTxRx;
let connectButton;

function initTxRx() {
  // mTxRx = new SerialTxRx(9600);
  // mTxRx = new WiFiTxRx("http://192.168.1.100");
  // mTxRx = new BLETxRx("bead0000-dead-beef-feed-abacadaba000");
  mTxRx.registerOnReceive((d) => print(d));
  connectButton.hide();
}

function setup() {
  createCanvas(windowWidth, windowHeight);

  connectButton = createButton("Connect To Arduino");
  connectButton.position(width / 2, height / 2);
  connectButton.mousePressed(initTxRx);
}

function draw() {
  background(0);

  if (mTxRx) {
    mTxRx.rx();
  }
}

function mousePressed() {
  if (mTxRx) {
    mTxRx.tx({ data: { x: mouseX, y: mouseY } });
  }
}
