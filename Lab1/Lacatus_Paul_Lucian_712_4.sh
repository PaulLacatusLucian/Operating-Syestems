#!/bin/bash

# Überprüfen, ob ein Ordner angegeben wurde
if [ -z "$1" ]; then
  echo "Bitte geben Sie den Ordner an, in dem Sie die Dateien umbenennen mochten."
  exit 1
fi

# wir gehen in "$1" rein
cd "$1"

# Umbenennen von .txt-Dateien in .ascii-Dateien
for file in $(find . -type f -name '*.txt'); do
  if [ -f "$file" ]; then
    mv "$file" "${file%.txt}.ascii"
  fi
done

for file in $(find . -type d); do
  if [ -d "$file" ]; then
    cd "$file" 
    for file in $(find . -type f -name '*.txt'); do
       if [ -f "$file" ]; then
         mv "$file" "${file%.txt}.ascii"
       fi
     done
  fi
done
