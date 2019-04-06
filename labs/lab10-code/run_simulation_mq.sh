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

len=${#names[*]}                # length of the array of names

./em_server_mq &                # start the server
em_server_pid=$!

# sleep 0.25                      # short pause to ensure server starts up

nclients=$((nclients-1))        # decrement as we are starting at 0

pids=()                         # track em_clients pids
for i in `seq 0 $nclients`; do
    idx=$((i % len))            # index of email addressfish out one emal address
    email="${names[idx]}"       # fish out an email address
    # echo "${names[idx]}"
    ./em_client_mq "$email" &   # start client with email address
    pids[i]=$!                  # keep track of pid of last process: client
done

# echo "${pids[*]}"

# wait for jobs to finish
for pid in ${pids[*]}; do       # wait for all clients to finish
    wait $pid
done

kill $em_server_pid             # kill the server
