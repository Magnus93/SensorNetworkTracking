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

### installing contiki on ubuntu

First we need to install the necessary toolchain to start compiling the Z1 environment, basically we need the following libraries:

    GCC (latest version is 4.4.4).
    Binutils (latest version is 2.20.1)
    Libc support.
    Python-serial support (BSL). 
    
if it states that you are missing "python2" use sudo apt-get install python2.7-minimal
