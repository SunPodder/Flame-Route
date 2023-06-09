#!/usr/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR=$(dirname $SCRIPT_DIR)

# templates are in this directory for now
# will be moved to the build directory
# with our own build system
cd $ROOT_DIR/examples
$ROOT_DIR/build/examples/example &
sleep 1

curl -s http://localhost:8080 | grep "Hello Flame!" || exit 1

echo "Passed! ✅"
killall example

