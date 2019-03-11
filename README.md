# SensorNetworkTracking

## installing contiki on ubuntu

First we need to install the necessary toolchain to start compiling the Z1 environment, basically we need the following libraries:

    GCC (latest version is 4.4.4).
    Binutils (latest version is 2.20.1)
    Libc support.
    Python-serial support (BSL). 
    
if it states that you are missing python2" use `sudo apt-get --reinstall install python-minimal`

If it states that serial is not found use `sudo apt-get install python-serial python3-serial`

To install msp430:
```
sudo apt-get install gcc-msp430
```

## See USB-port connections
```
dmesg | grep "tty"
```

should find something like this:
```
[ 4055.572591] usb 2-2.1: cp210x converter now attached to ttyUSB0
```

or


```
motelist
```
if the above doesn't work try 

```
sudo apt-get install tinyos-tools
```

## Compile code
```
make client
```
or 
```
make basestation
```

## Upload code
```
make TARGET=z1 MOTES=/dev/ttyUSB0 client.upload
```
or
```
make TARGET=z1 MOTE=n client.upload (n is the n'th connected node)
```

## Listen to device

### Add dialout (for listening):
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


### Listen with "make login"
listen with:
```
sudo make TARGET=z1 login
sudo make TARGET=z1 MOTE=n login (to listen to the n'th connected node)
```

If you are running on a 64-bit system then this command will have to be written:
```
sudo apt-get install libc6-i386
```
More info here if "make login" does not work:
http://anrg.usc.edu/contiki/index.php/Troubleshooting

To make target without `sudo` use:
```
sudo chmod 666 /dev/ttyUSB0
```

### COOJA TROUBLES

if 
```
ant run 
```

does not work  adding the following lines in build.xml made Cooja start and simulate:
```
<compilerarg line="--add-modules java.xml.bind"/>
```

Hence, the final compile command would be:

```
<target name="compile" depends="init">
    <mkdir dir="${build}"/>
    <javac srcdir="${java}" destdir="${build}" debug="on"
           includeantruntime="false"
           encoding="utf-8">
      <compilerarg line="--add-modules java.xml.bind"/>
      <classpath>
        <pathelement path="."/>
        <pathelement location="lib/jdom.jar"/>
        <pathelement location="lib/log4j.jar"/>
        <pathelement location="lib/jsyntaxpane.jar"/>
        <pathelement location="lib/swingx-all-1.6.4.jar"/>
      </classpath>
    </javac>
  </target>
  ```


