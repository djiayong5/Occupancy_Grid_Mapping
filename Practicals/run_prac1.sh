#!/bin/bash

#compile the recently edited c++ file
echo "compiling..."

g++ -o simulator `pkg-config --cflags playerc++` simple.cpp `pkg-config --libs playerc++`

#open the application
echo "Opening the application using a new terminal..."
gnome-terminal -e "./loadapp.sh"

#Wait for the application to load
echo "Waiting for the application..."
sleep 3s

#opening the simulation
echo "Opening the simulation..."
./simulator
