#!/bin/bash

if ! [[ $(command -v include-what-you-use && command -v cppcheck) ]]; then
    echo "Dependencies not found!\nExiting..."
    exit
fi

for file in $(find ./src -name *.cpp | grep -v mime); do
    include-what-you-use -I./src $file
    cppcheck $file
done
