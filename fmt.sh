#!/usr/bin/env bash

# get all changed files
files=$(git diff --name-only HEAD | grep -E '\.(cpp|hpp)$' | grep -v 'src/mime' | grep -v "json_impl" | grep -v "inja")

# format all changed files
for file in $files; do
	clang-format -i $file
done

