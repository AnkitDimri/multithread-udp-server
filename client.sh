#!/bin/bash

echo "Compiling client.cpp ..."
echo
g++ client.cpp -o client
echo "Client code compiled..."
echo "Starting client..."
echo
echo
./client
