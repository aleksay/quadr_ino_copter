#!/bin/bash -x


BOARD="diecimila"
CHIP="atmega168"
ARDUINOIDE_PATH="/opt/arduino"
PORT="/dev/ttyUSB0"

PRJPATH="$(pwd)"

ino clean;
ino build -m $BOARD;
avrdude -C$ARDUINOIDE_PATH/hardware/tools/avrdude.conf -v -v -v -v -p$CHIP -cstk500v1 -P$PORT -b19200 -D -Uflash:w:$PRJPATH/.build/$BOARD/firmware.hex:i 
 
