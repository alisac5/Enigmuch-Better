let fs = require('fs')
// Require the serialport node module
var Serialport = require('serialport');
var connTimer;
var serialDesc = null;
var port;
var serialConnect = function () {
    return new Promise(function (resolve, reject) {

        Serialport.list(function (err, ports) {
            jsonl(ports);
            var foundArduino = false;
            ports.forEach(listedPort => {
                // if (listedPort.comName.substring(0, 16) === "/dev/tty.raspber"){
                if (listedPort.comName.substring(0, 16) === "/dev/tty.usbmode") {
                    foundArduino = true;
                    aport = new Serialport(listedPort.comName, {
                        baudRate: 9600
                    }, function (err) {
                        reject(err);
                    });

                    if (aport != null) {
                        resolve(aport);
                    }
                }

            });
            if (foundArduino == false) {
                reject(new Error("Did not find Arduino. Please check power/connections"));
            }
        });
    });
}

var doSerial = function () {
    serialConnect().then(function (aport) {
        if(port && port.isOpen){
            Reveal.slide(2, 0, 0);
            return;
        }
        port = aport;
        // Read the port data
        port.on("open", () => {
            console.log('serial port open');
            setTimeout(function(){ Reveal.slide(2, 0, 0); setArduinoSlide(20); }, 2000);
        });

        var Readline = Serialport.parsers.Readline; // make instance of Readline parser
        var parser = new Readline(); // make a new parser to read ASCII lines
        port.pipe(parser); // pipe the serial stream to the parser

        parser.on('data', data => {
            console.log('got word from arduino:', data);
        });

        port.on('error', error => {
            console.error(error);
        });

        port.on('close', close => {
            console.warn("Arduinio Disconnected");
        });


    }).catch((err) => {
        alert("Failed to connect to arduino. Please check power/connections." + err.message);
        console.log(err);
    });
}

function setArduinoSlide( slide){
    port.write('SLD ' + slide +'\n', (err) => {
        if (err) {
          return console.log('Error on write: ', err.message);
        }
        console.log('message written');
      });
}
var isTestingHW= false; 
function testHardwareHandler(){
    var button = document.getElementById("hwtstbtn");

    if(isTestingHW == false){
        // make button active 
        button.className+=" highlightbtn";

        isTestingHW = true; 
        setArduinoSlide(100);
    }else{
        button.classList.remove('highlightbtn');
       // button.classList.remove(':hover');

       setArduinoSlide(20);
        isTestingHW = false; 
    }

}

//const { version } = require('../package.json')
//const args = require('commander')

Reveal.addEventListener('slidechanged', function (event) {
    // event.previousSlide, event.currentSlide, event.indexh, event.indexv

    if (event.indexh == 0 || event.indexh == 1) {
        // Slide every five seconds
        toggleNav(false);
    } else {
        // Slide every five seconds
        toggleNav(true);

    }
    if(event.indexh == 1){
        doSerial();
    }

    if(event.indexh == 2){
        setArduinoSlide(20);
    }

    if(event.indexh == 3){
        setArduinoSlide(30);

    }

});


function jsonl(ports) {
    ports.forEach(port => {
        console.log(JSON.stringify(port))
    })
}








function toggleNav(b) {
    Reveal.configure({
        controls: b,
        progress: b,
        keyboard: b,
        overview: b,
        touch: b
    });
}

function errorOnArduinoConnect(err) {

    if (err) {
        alert("Failed to connect to arduino. Please check power/connections." + err.message);

    } else {
        alert("Failed to connect to arduino. Please check power/connections.");
    }
    clearTimeout(connTimer);
    Reveal.slide(0, 0, 0);
    throw err;
}
