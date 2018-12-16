// CONFIG
var portName = '/dev/ttyACM0';
var options = {
    baudrate: 115200
    // baudrate: 9600
};
var fps = 30;
var dataPoints = 1000;
var dataLineHeight = 200;
var headerOffset = 30;

var pressureTriggerHeight = Math.floor(dataLineHeight / 3);

var debug = false;

// -------------------------------------------------------------------- //

var data = [];

// Analog pins
var a0 = 0;
var a1 = 0;
var a2 = 0;
var a3 = 0;
var real0 = 0;
var real1 = 0;
var real2 = 0;
var real3 = 0;

// Data arrays
var _a0 = [];
var _a1 = [];
var _a2 = [];
var _a3 = [];
var _real0 = [];
var _real1 = [];
var _real2 = [];
var _real3 = [];

var Serial;

var canvasWidth;
var heightOffset;
var padRight = 10;

function setup() {
    // Instantiate our SerialPort object
    Serial = new p5.SerialPort();

    // Get a list the ports available
    Serial.list(function (ports) {
        console.log(ports);
    });

    // Assuming our Arduino is connected, let's open the connection to it
    Serial.open(portName, options);

    // EVENTS
    // When our Serial port is opened and ready for read/write
    Serial.on('open', gotOpen);
    Serial.on('connected', serverConnected);
    Serial.on('data', gotData);
    Serial.on('error', gotError);

    canvasWidth = windowWidth - padRight;
    canvasHeight = dataLineHeight * 4 + headerOffset + 40;

    // Create the canvas p5 draws on
    createCanvas(canvasWidth, canvasHeight);

    // The number of times draw() executes in each second
    frameRate(fps);

    // Write empty data
    initData();
}

function initData() {
    for (i = 0; i < dataPoints - 1; i++) {
        data.push(0);
        _a0.push(0);
        _a1.push(0);
        _a2.push(0);
        _a3.push(0);
        _real0.push(0);
        _real1.push(0);
        _real2.push(0);
        _real3.push(0);
    }
}

// Connected to our Serial device
function gotOpen() {
    console.log("Serial Port is Open");
}

function gotError(error) {
    console.log(error);
}

function serverConnected() {
    console.log("Serial Connected");
}

// There is data available to work with from the Serial port
function gotData() {
    var incoming = Serial.readLine().trim();  // read the incoming string and trim it
    if (!incoming) return;

    var sensorData = incoming.split(';');
    //console.log("sensorData", sensorData);

    a0 = Math.min(parseInt(sensorData[0]) / 3, dataLineHeight);
    a1 = Math.min(parseInt(sensorData[1]) / 3, dataLineHeight);
    a2 = Math.min(parseInt(sensorData[2]) / 3, dataLineHeight);
    a3 = Math.min(parseInt(sensorData[3]) / 3, dataLineHeight);

    real0 = Math.min(parseInt(sensorData[4]) / 3, dataLineHeight);
    real1 = Math.min(parseInt(sensorData[5]) / 3, dataLineHeight);
    real2 = Math.min(parseInt(sensorData[6]) / 3, dataLineHeight);
    real3 = Math.min(parseInt(sensorData[7]) / 3, dataLineHeight);

    data.push(incoming);
    _a0.push(a0);
    _a1.push(a1);
    _a2.push(a2);
    _a3.push(a3);
    _real0.push(real0);
    _real1.push(real1);
    _real2.push(real2);
    _real3.push(real3);

    //console.log("a0", a0);

    var i = _a0.length;

    // remove first entries from data arrays
    if (dataPoints - 1 < i) {
        data.shift();
        _a0.shift();
        _a1.shift();
        _a2.shift();
        _a3.shift();
        _real0.shift();
        _real1.shift();
        _real2.shift();
        _real3.shift();
    }

    if (debug) console.log(incoming);
}

// Methods available
// Serial.read() returns a single byte of data (first in the buffer)
// Serial.readChar() returns a single char 'A', 'a'
// Serial.readBytes() returns all of the data available as an array of bytes
// Serial.readBytesUntil('\n') returns all of the data available until a '\n' (line break) is encountered
// Serial.readString() retunrs all of the data available as a string
// Serial.readStringUntil('\n') returns all of the data available as a string until a specific string is encountered
// Serial.readLine() calls readStringUntil with "\r\n" typical linebreak carriage return combination
// Serial.last() returns the last byte of data from the buffer
// Serial.lastChar() returns the last byte of data from the buffer as a char
// Serial.clear() clears the underlying Serial buffer
// Serial.available() returns the number of bytes available in the buffer
// Serial.write(somevar) writes out the value of somevar to the Serial device

function draw() {
    background(255); // set white bg

    fill(0);
    noStroke();
    text('Fps:', 20, 20);
    text(frameRate(), 60, 20);

    heightOffset = headerOffset;

    heightOffset += dataLineHeight + 5;
    drawInputArea('Input 1', a0, _a0, real0, _real0, heightOffset, dataLineHeight, canvasWidth, pressureTriggerHeight);

    heightOffset += dataLineHeight + 5;
    drawInputArea('Input 2', a1, _a1, real1, _real1, heightOffset, dataLineHeight, canvasWidth, pressureTriggerHeight);

    heightOffset += dataLineHeight + 5;
    drawInputArea('Input 3', a2, _a2, real2, _real2, heightOffset, dataLineHeight, canvasWidth, pressureTriggerHeight);

    heightOffset += dataLineHeight + 5;
    drawInputArea('Input 4', a3, _a3, real3, _real3, heightOffset, dataLineHeight, canvasWidth, pressureTriggerHeight);
}

function drawInputArea(inputName, inputValue, inputDataArray, realValue, realDataArray, heightOffset, dataLineHeight, canvasWidth, pressureTriggerHeight) {
    stroke(150, 0, 0);
    line(0, heightOffset - dataLineHeight, canvasWidth, heightOffset - dataLineHeight);

    fill(0);
    noStroke();
    text(inputName + ':', 20, heightOffset - dataLineHeight + 20);

    fill(248);
    rect(0, heightOffset - pressureTriggerHeight, canvasWidth, pressureTriggerHeight);

    stroke(200);
    line(0, heightOffset - pressureTriggerHeight, canvasWidth, heightOffset - pressureTriggerHeight);

    stroke(150, 0, 0);
    line(0, heightOffset, canvasWidth, heightOffset);

    stroke(0);
    noFill();
    // draw vertex line for input
    beginShape();
    for (var i = 0; i < inputDataArray.length; i++) {
        var x = map(i, 0, inputDataArray.length, 0, canvasWidth) - 20; // Re-maps a number from one range to another. map(value,start1,stop1,start2,stop2)
        var y = heightOffset - inputDataArray[i];
        vertex(x, y); // create vertex point to coodinates x, y
    }
    endShape();

    stroke(255, 150, 150);
    noFill();
    // draw vertex line for real sensor values
    beginShape();
    for (var i = 0; i < realDataArray.length; i++) {
        var x = map(i, 0, realDataArray.length, 0, canvasWidth) - 20; // Re-maps a number from one range to another. map(value,start1,stop1,start2,stop2)
        var y = heightOffset - realDataArray[i];
        vertex(x, y); // create vertex point to coodinates x, y
    }
    endShape();

    // draw input ellipse
    fill(150, 255, 150);
    ellipse(canvasWidth - 20, heightOffset - inputValue, 10, 10);
    noFill();

    fill(200, 200, 200);
    ellipse(canvasWidth - 20, heightOffset - realValue, 10, 10);
    noFill();
}
