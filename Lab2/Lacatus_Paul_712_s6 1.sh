#!/bin/bash

if [ $# -lt 2 ]; then
  echo "Bitte geben Sie mindestens 2 Parameter."
  exit 1
fi

sed "s/[^A-Z]/$1/g" $2

