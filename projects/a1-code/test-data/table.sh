#!/bin/bash

# Generate a table of squares and cubes. Used for testing output
#
# usage: table.sh [limit=30] [sleep=0]
#
# Prints squares/cubes up to limit then sleeps given amount. Both
# params optional.

limit=30
if (($# >= 1)); then
   limit=$1
fi

sleeptime=0
if (($# >= 2)); then
    sleeptime=$2
fi

for i in `seq $limit`; do
    printf "i^1= %6d  i^2= %6d  i^3= %6d\n" $((i)) $((i*i)) $((i*i*i))
done

sleep $sleeptime
