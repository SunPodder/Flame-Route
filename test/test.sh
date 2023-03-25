#!/usr/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

$SCRIPT_DIR/../build/examples/example &
sleep 1

curl localhost:8080 | grep "Welcome to Home" || exit 1

echo "Passed! ✅"
killall example
