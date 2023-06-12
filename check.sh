#!/bin/bash

if ! [[ $(command -v include-what-you-use && command -v cppcheck) ]]; then
    echo "Dependencies not found!\nExiting..."
    exit
fi

# list all changed .cpp and .hpp files
changed_files=$(git diff --name-only HEAD~1 | grep -E "\.(cpp|hpp)$")

if [[ -z "$changed_files" ]]; then
	exit
else
	echo "Changed files:"
	echo "$changed_files"
fi


echo "Ignore \`<sstream>\` include error of \`src/http/request.cpp\`. Don't remove that header."
echo ""

for file in $changed_files; do
    include-what-you-use -I./src $file
    cppcheck $file
done

printf "\n\n\n"

