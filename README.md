# mckyla-fsr

## Web UI

This is a server that allows changing sensor thresholds via a web interface. 

### Dependencies
* Python 2
* pyserial

### How to run
``` cd path/to/web-ui
  python server.py
```
To navigate to pad setup, open `http://localhost:80/pad1/pads.py` or `http://localhost:80/pad2/pads.py`. 

You should see an image of the pad. If you do not see one, check the console for SerialException. Make sure your pads are plugged in, if the error still occurs, see Board setup below.

### Board setup

* Make sure the board is running a sketch from `arduino/pad` directory.
* Check that the pin numbers in the sketch match where the FSRs are plugged in on your board.
* If the board is not recognized as a serial device, choose Tools > USB Type > Serial in Arduino IDE.
