#!/usr/bin/env bash

FAILED=false
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR=$(dirname $SCRIPT_DIR)
BASE_URL="http://localhost:8080"

# templates are in this directory for now
# will be moved to the build directory
# with our own build system in future
cd $ROOT_DIR/examples
$ROOT_DIR/build/examples/example &> /dev/null &
sleep 1


. $ROOT_DIR/test/hello.sh
. $ROOT_DIR/test/dynamic-route.sh
. $ROOT_DIR/test/memory.sh


# clean up
killall example

if [ "$FAILED" = true ]; then
	exit 1
else
	exit 0
fi

