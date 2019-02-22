#!/bin/bash

if [ "$1" == "" ]; then 
    echo "Expected using compiler name (emcc or gcc)"
    exit 1
fi

echo "Building using $1..."

if [ "$1" == "emcc" ]; then
    make -f emscripten/Makefile
    exit
fi

echo "Building using $1 are now supported now!"
exit 1