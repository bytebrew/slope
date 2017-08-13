#!/bin/bash
# Usefull scripts to use while developing slope

if [[ $EUID -eq 0 ]]; then
    echo 'It is not recommened to run these commands as root'
    exit -1
fi

if [[ $# -eq 0 ]]; then
    echo 'You have to provide a command to run'
    exit -1
fi

# clang-format adjusts source code format (braces placement, indentation, etc)
if [[ $1 == 'clang-format' ]]; then
    find . -iname *.h -o -iname *.c | xargs clang-format-3.9 -i
fi

# builds slope in debug mode
if [[ $1 == 'build-debug' ]]; then
    if [[ -d build/Debug ]]; then
        # Yes this may increase your compile times
        rm -rf build/Debug
    fi
    mkdir -p build/Debug
    cd build/Debug
    cmake -DCMAKE_BUILD_TYPE=Debug ../../
    make
fi

# builds slope in release mode
if [[ $1 == 'build-release' ]]; then
    if [[ -d build/Release ]]; then
        # Yes this may increase your compile times
        rm -rf build/Release
    fi
    mkdir -p build/Release
    cd build/Release
    cmake -DCMAKE_BUILD_TYPE=Release ../../
    make
fi
