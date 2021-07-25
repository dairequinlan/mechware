#!/bin/bash
stty -F /dev/ttyACM0 1200
echo waiting
while [ ! -d /media/daire/RPI-RP2 ]; do sleep 0.1; done
sleep 0.5
echo copying
cp $1 /media/daire/RPI-RP2
echo done
