#!/bin/bash
# Works like cat but handles SIGTERM gracefully

handle_term() {
   exit 0
}

trap 'handle_term' TERM

while read line
do
  echo "$line"
done < "${1:-/dev/stdin}"

