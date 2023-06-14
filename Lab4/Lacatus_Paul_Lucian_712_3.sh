#!/bin/bash

awk -F, '{
    if (FILENAME == "studenten.csv") {
        map[$1] = $2
    }
    if (FILENAME == "notenSommer.csv") {
        sum=0
        for (i = 2; i <= NF; i++) {
            sum += $i
        }
	  avg = sum + map[$1]
	  avg = avg / NF
        map[$1] = avg
    }
} END {
    for (key in map) {
        print key "," map[key] > "temp.csv"
    }
    system("sort -t, -k2nr temp.csv > studenten.csv")
    system("rm temp.csv")
}' studenten.csv notenSommer.csv
