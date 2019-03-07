stty -F /dev/ttyUSB1 115200
while true; do
	read -n 50 LINE </dev/ttyUSB1
	echo $LINE
	done

