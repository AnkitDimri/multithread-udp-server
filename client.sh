#!/bin/bash

echo "Compiling client.cpp ..."
echo
g++ client.cpp -o client
echo "Client code compiled..."

# check for already running clients
# and give apropriate number to the new client

count=0
for pids in pidof client
do
  count=`expr $count + 1`
done

count=`expr $count + 1`

echo "Starting client $count ..."
echo
echo
./client
