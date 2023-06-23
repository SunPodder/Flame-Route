#!/usr/bin/env bash

pid=$(ps aux | grep example | grep -v grep | awk '{print $2}')

if [ -z "$pid" ]; then
	echo "Memory - Failed! ❌"
	exit 1
fi

initial_mem=$(ps -p "$pid" -o rss=)

echo "Initial memory: $initial_mem"

for i in {1..1000}; do
	curl -s "$BASE_URL" -o /dev/null &
done

# ensure all requests are finished
requests=$(ps aux | grep curl | grep -v grep | wc -l)
while [ "$requests" -gt 0 ]; do
	sleep 0.1
	requests=$(ps aux | grep curl | grep -v grep | wc -l)
done

sleep 0.5
mem=$(ps -p "$pid" -o rss=)
echo "Memory after 1000 requests: $mem"

# tolerable limit: 300 KB
mem=$((mem - 300))

if [ "$mem" -gt "$initial_mem" ]; then
	#FAILED=true
	echo "Memory - Failed! ❌"
else
	echo "Memory - Passed! ✅"
fi

unset pid
unset initial_mem
unset mem

