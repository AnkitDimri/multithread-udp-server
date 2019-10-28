#!/bin/bash

echo "Compiling client.cpp ..."
echo
g++ client.cpp -o client
echo "Client code compiled..."

# check for already running clients
# and give apropriate number to the new client

count = 0
for count in pidof client
do
  count = `expr $count + 1`
done


echo "Starting client $count ..."
echo
echo
./client
