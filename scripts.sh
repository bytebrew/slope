#!/bin/bash
# Copyright (C) 2018  Elvis Teixeira
#
# This source code is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General
# Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any
# later version.
#
# This source code is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General
# Public License along with this program.
# If not, see <http://www.gnu.org/licenses/>.

## Usefull scripts to use while developing slope

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
    if [[ ! -z `command -v clang-format-5.0` ]]; then
        find . -iname *.cpp -o -iname *.hpp | xargs clang-format-5.0 -i
    elif [[ ! -z `command -v clang-format-3.9` ]]; then
        find . -iname *.cpp -o -iname *.hpp | xargs clang-format-3.9 -i
    else
        echo 'clang-format not found'
    fi
fi

# Perf command
# sudo perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations <AppName>

# builds slope in debug mode
if [[ $1 == 'build-debug' ]]; then
    if [[ -d build/Debug ]]; then
        rm -rf build/Debug
    fi
    mkdir -p build/Debug
    cd build/Debug
    cmake -DCMAKE_BUILD_TYPE=Debug ../../
    make -j4
fi

# builds slope in release mode
if [[ $1 == 'build-release' ]]; then
    if [[ -d build/Release ]]; then
        rm -rf build/Release
    fi
    mkdir -p build/Release
    cd build/Release
    cmake -DCMAKE_BUILD_TYPE=Release ../../
    make -j4
fi

if [[ $1 == 'test' ]]; then
    if [[ -d build/Debug ]]; then
        rm -rf build/Debug
    fi
    mkdir -p build/Debug
    cd build/Debug
    cmake -DCMAKE_BUILD_TYPE=Debug ../../
    make -j4
    for f in `ls tests`; do
        if [[ ${f: -5} == "_test" ]]; then
            valgrind "./tests/$f"
        fi
    done
fi
