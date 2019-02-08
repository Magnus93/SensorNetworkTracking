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
sudo adduser admin dialout
```
