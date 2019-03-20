stty -F /dev/ttyUSB0 115200
while true; do
	read -n 50 LINE </dev/ttyUSB0
	echo $LINE
	done

