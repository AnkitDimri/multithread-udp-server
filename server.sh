#!/bin/bash

echo "Compiling server.cpp ..."
echo
g++ server.cpp -o server -lpthread
echo "Server code compiled..."
echo "Starting server..."
echo
echo
./server
