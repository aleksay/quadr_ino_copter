#!/bin/bash -x

NO_ARGS=0
E_OPTERROR=85

BOARD="diecimila"
CHIP="atmega168"
ARDUINOIDE_PATH="/opt/arduino"
PORT="/dev/ttyUSB0"
BAUD="19200"

PRJPATH="$(pwd)"



if [ $# -eq "$NO_ARGS" ]    # Script invoked with no command-line args?
then
  echo "Usage: `basename $0` options (-mbcpA)"
  exit $E_OPTERROR          # Exit and explain usage.
                            # Usage: scriptname -options
                            # Note: dash (-) necessary
fi

while getopts "m:b:c:p:A:" Option
do
  case $Option in
    m) BOARD="$OPTARG";;
    c) CHIP="$OPTARG";;
    p) PORT="$OPTARG";;
    b) BAUD="$OPTARG";;
    A) ARDUINOIDE_PATH="$OPTARG";;
    *) echo "Unimplemented option chosen.";;   # Default.
  esac
done

shift $(($OPTIND - 1))
#  Decrements the argument pointer so it points to next argument.
#  $1 now references the first non-option item supplied on the command-line
#+ if one exists.

ino clean;
ino build -m $BOARD;
avrdude -C$ARDUINOIDE_PATH/hardware/tools/avrdude.conf -v -v -v -v -p$CHIP -cstk500v1 -P$PORT -b$BAUD -D -Uflash:w:$PRJPATH/.build/$BOARD/firmware.hex:i 
 