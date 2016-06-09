var queue = new CBuffer(10);

var eps = 0.9       // filter time constant
var filter = 0.0    // filtered value
var nloop = 100.0   // number of loops accumulated
var amp = 25.0      // difference amplitude

function logMsg(msg) {
    messages.value = msg + "\n" + messages.value; 
}

function onReceive(info) {
    var view = new Uint8Array(info.data);
    //console.log("Received: " + info.data.byteLength + " bytes");
    for (i = 0; i < info.data.byteLength; i++) {
        queue.push(view[i]);
    }
    
    if (queue.size < 8) return; 
    if (queue.shift() != 1) return;
    if (queue.shift() != 2) return;
    if (queue.shift() != 3) return;
    if (queue.shift() != 4) return;
    
    onLow = queue.shift();
    onHigh = queue.shift();
    onValue = (256 * onHigh + onLow) / nloop;
    
    offLow = queue.shift();
    offHigh = queue.shift();
    offValue = (256 * offHigh + offLow) / nloop;
    
    filter = (1 - eps) * filter + eps * amp * (onValue - offValue);

    console.log(filter + " | " + onValue + " | " + offValue);
};

onload = function() {
    chrome.serial.getDevices(function(ports) {
        var eligiblePorts = ports.filter(function(port) {
            return !port.path.match(/[Bb]luetooth/) && port.path.match(/\/dev\/tty/);
        });
        
        if (eligiblePorts.length < 1) {
            logMsg("No ports found");
            return;
        } else if (eligiblePorts.length > 1) {
            logMsg("More than 1 port found, connecting to the first one: " + eligiblePorts[0].path);
        } else {
            logMsg("Connecting to " + eligiblePorts[0].path);
        }
        
        port = eligiblePorts[0];
        
        chrome.serial.connect(port.path, function(connInfo) {
            logMsg("Connected to " + port.path);

            chrome.serial.onReceive.addListener(onReceive);
        });
    });
};