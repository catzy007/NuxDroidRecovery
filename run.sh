#!/bin/bash
if [ $(dpkg-query -W -f='${Status}' xterm 2>/dev/null | grep -c "ok installed") -eq 0 ]
then
	sudo apt update && sudo apt install xterm -y
fi
if [ $(dpkg-query -W -f='${Status}' foremost 2>/dev/null | grep -c "ok installed") -eq 0 ]
then
	sudo apt update && sudo apt install foremost -y
fi
if [ $(dpkg-query -W -f='${Status}' adb 2>/dev/null | grep -c "ok installed") -eq 0 ]
then
	sudo apt update && sudo apt install adb -y
fi
if [ $(dpkg-query -W -f='${Status}' netcat 2>/dev/null | grep -c "ok installed") -eq 0 ]
then
	sudo apt update && sudo apt install netcat -y
fi
if [ $(dpkg-query -W -f='${Status}' build-essential 2>/dev/null | grep -c "ok installed") -eq 0 ]
then
	sudo apt update && sudo apt build-essential xterm -y
fi
gcc AndroReco.c -o AndroReco
gcc nckiller.c -o nckiller
./AndroReco
