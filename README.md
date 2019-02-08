# SensorNetworkTracking


### See USB-port connections
```
dmesg | grep "tty"
```

should find something like this:
```
[ 4055.572591] usb 2-2.1: cp210x converter now attached to ttyUSB0
```

### compile code
```
make client
```
or 
```
make basestation
```

### Upload code
```
make TARGET=z1 MOTES=/dev/ttyUSB0 client.upload
```

### Add dialout:
```
sudo adduser username dialout
```

### listen to USB0
Goto the `src` folder
Add execute permission to sh-file
```
chmod +x ./listen0.sh
```

Listen output from device
```
./listen0.sh
```
this will listen on USB0.


