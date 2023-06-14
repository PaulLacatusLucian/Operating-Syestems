#!/bin/bash

# Uberprufen, ob mindestens ein Parameter vorhanden ist
if [ $# -lt 1 ]; then
  echo "Bitte geben Sie mindestens eine shell Datei."
  exit 1
fi

# Uberprufen, ob es sich um Schell-Skripte handelt
for file_name in "$@"; do
  file "$file_name" | grep '.*shell script.*' | echo "$file_name"
done | sort
