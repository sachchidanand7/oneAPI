#!/bin/bash
source /opt/intel/inteloneapi/setvars.sh
cd /home/u187178/oneAPI/
if [ $# -ne 1 ]
then
    rm -rf build
    build="$PWD/build"
    [ ! -d "$build" ] && mkdir -p "$build"
    cd build &&
	cmake ../src &&
	cmake --build . &&
	make run
else
    cd build && make run
fi


