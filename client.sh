#!/bin/bash

echo "Compiling client.cpp ..."
echo
g++ client.cpp -o client
echo "Client code compiled..."

# check for already running clients
# and give apropriate number to the new client

count=0
ps_running=($(pidof client))
for pids in $ps_running
do
  echo $pids
  count=`expr $count + 1`
done

count=`expr $count + 1`

echo "Starting client $count ..."
echo
echo
./client
