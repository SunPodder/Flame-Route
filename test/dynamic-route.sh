#!/usr/bin/env bash

for i in {1..3}; do
	curl -s "$BASE_URL/user/$i" | grep "ID: $i" &> /dev/null || FAILED=true
	curl -s "$BASE_URL/user/user$i" | grep "User: user$i" &> /dev/null || FAILED=true
done

if [ "$FAILED" = true ]; then
	echo "Dynamic Route - Failed! ❌"
else
	echo "Dynamic Route - Passed! ✅"
fi

