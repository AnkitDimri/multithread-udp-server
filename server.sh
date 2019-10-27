#!/bin/bash

g++ server.cpp -o server -lpthread
echo "Server code compiled..."
echo "Starting server..."
echo
echo
./server
