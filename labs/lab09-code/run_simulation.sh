#!/bin/bash
# 
# usage: run_smulations.sh 20
# 
# Start an em_server then run multiple simultaneous em_clients
# connecting to find various emails. Accepts a command line argument
# which is the number of clients to run.

nclients=10

if (( $# >= 1 )); then
    nclients=$1
fi

names=(
    "Chris Kauffman"
    "Christopher Jonathan"
    "Amy Larson"
    "Chris Dovolis"
    "Dan Knights"
    "George Karypis"
    "Steven Jensen"
    "Daniel Keefe"
    "Michael W. Whalen"
    "Catherine Qi Zhao"
    "Dan Challou"
    "Steven Wu"
    "Michael Steinbach"
    "Jon Weissman"
    "Victoria Interrante"
    "Shana Watters"
    "James Parker"
    "James Moen"
    "Daniel Giesel"
    "Jon Read"
    "Sara Stokowski"
)

len=${#names[*]}

# kill em_server
./em_server &
em_server_pid=$!

# start em_clients
pids=()
for i in `seq 0 $nclients`; do
    idx=$((i % len))
    # echo "${names[idx]}"
    ./em_client "${names[idx]}" &
    pids[i]=$!
done

# echo "${pids[*]}"

# wait for jobs to finish
for pid in ${pids[*]}; do
    wait $pid
done

kill $em_server_pid
