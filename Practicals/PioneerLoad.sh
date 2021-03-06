#!/bin/bash

#Compile the recently edited c++ file
echo "Compiling..."

g++ "Occupancy_Grid.cpp"
g++ -o simulator `pkg-config --cflags playerc++` Pioneer.cpp `pkg-config --libs playerc++`

#Open the application
echo "Opening the application using a new terminal..."
gnome-terminal -e "./LoadApp.sh"

#Wait for the application to load
echo "Waiting for the application..."
sleep 3s

#opening the simulation
echo "Opening the simulation..."
./simulator
