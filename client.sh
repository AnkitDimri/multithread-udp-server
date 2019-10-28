#!/bin/bash

echo "Compiling client.cpp ..."
echo
g++ client.cpp -o client
echo "Client code compiled..."

# check for already running clients
# and give apropriate number to the new client

# Before running multiple clients on same
# client machine comment the command in
# client.cpp that assigns port number to client process
# or assign random port numbers
# (no two process can have same port number)

count=0
ps_running=$(pidof client)
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
