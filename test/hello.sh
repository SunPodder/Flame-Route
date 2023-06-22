#!/usr/bin/env bash

curl -s $BASE_URL | grep "Hello Flame!" &> /dev/null || FAILED=true

if [ "$FAILED" = true ]; then
	echo "Hello - Failed! ❌"
else
	echo "Hello - Passed! ✅"
fi

