 
#!/bin/bash

# Überprüfen, ob 2 Parameter gegeben sind
if [ $# -lt 2 ]; then
  echo "Bitte geben Sie mindestens eine shell Datei."
  exit 1
fi

# Output von dem ersten File
for param in $@; do
  if [ $(( $param % 2 )) -eq 0 ]; then
     awk '{print}' "$param"
  else
     vektor=($(awk -F: '{for (i=1; i<=NF; i++) print $i}' "$param"))

      # Output von dem zweiten File umgekehrt
      for ((i=${#vektor[@]}-1; i>=0; i--)); do
           echo -n  "${vektor[i]} "
      done
   fi
done
