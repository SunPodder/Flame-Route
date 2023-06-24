#!/bin/bash

if ! [[ $(command -v include-what-you-use && command -v cppcheck) ]]; then
    printf "Dependencies not found!\nExiting...\n"
    exit
fi

# list all changed .cpp and .hpp files
if [[ -z "$*" ]]; then
	changed_files=$(git diff --name-only HEAD~1 | grep -E "\.(cpp|hpp)$")
else
	changed_files=($@)
fi

if [[ -z "${changed_files[*]}" ]]; then
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

