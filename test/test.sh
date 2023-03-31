#!/usr/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR=$(dirname $SCRIPT_DIR)
cd $ROOT_DIR/examples
$ROOT_DIR/build/examples/example &
sleep 1

curl localhost:8080 | grep "Hello Flame!" || exit 1

echo "Passed! ✅"
killall example
