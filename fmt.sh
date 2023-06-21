#!/usr/bin/env bash

# if arguments are given format them
if [[ ! -z "$1" ]]; then
	files=$@
else
	# get all changed files
	files=$(git diff --name-only HEAD | grep -E '\.(cpp|hpp)$' | grep -v 'src/mime' | grep -v "json_impl" | grep -v "inja")
fi

for file in $files; do
	clang-format -i $file
done

