#!/bin/bash
echo waiting
while [ ! -d /media/daire/RPI-RP2 ]; do sleep 0.1; done
sleep 0.5
echo copying
cp compile/pico-firmware-left.uf2 /media/daire/RPI-RP2
echo done
