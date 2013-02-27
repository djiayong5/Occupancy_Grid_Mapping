/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.cpp
 * Description: Stores member function declarations for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 26/02/2013
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "Occupancy_Grid.h"

using namespace std;

Occupancy_Grid::Occupancy_Grid() {
    robotX = 0;
    robotY = 0;
    columnWidth = 3;
    rowHeight = 3;

    grid.resize(START_COLUMNS);

    for (int counter = 0; counter < START_COLUMNS; counter++) {
        grid[counter].resize(START_ROWS);
    }
}

vector<vector<int> > Occupancy_Grid::getGrid() {
    return grid;
}

void Occupancy_Grid::shiftValuesDown() {
    for (int rowCounter =  rowHeight - 1; rowCounter > 0; rowCounter--) {
        for (int columnCounter = 0; columnCounter < columnWidth; columnCounter++) {
            grid[rowCounter][columnCounter] = grid[rowCounter - 1][columnCounter]; /* Copies value to the cell below. */
            grid[rowCounter - 1][columnCounter] = 0; /* Sets value of cell to 0. */
        }
    }
}

void Occupancy_Grid::shiftValuesRight() {    
    for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) {
        for (int columnCounter = 1; columnCounter < columnWidth; columnCounter++) {
            grid[rowCounter][columnCounter - 1] = grid[rowCounter][columnCounter]; /* Copies value to the cell to the right. */
            grid[rowCounter][columnCounter] = 0; /* Sets value of cell to 0. */
        }
    }
}

void Occupancy_Grid::resizeGrid(int directionToExpand) {
    if (directionToExpand == UP || directionToExpand == DOWN) rowHeight++;
    else if (directionToExpand == LEFT || directionToExpand == RIGHT) columnWidth++;

    grid.resize(rowHeight);

    for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) grid[rowCounter].resize(columnWidth);

    if (directionToExpand == UP) shiftValuesDown();
    else if (directionToExpand == LEFT) shiftValuesRight();
}

void Occupancy_Grid::printGrid() {
    cout << "Number of rows: " << rowHeight << endl;
    cout << "Number of columns: " << columnWidth << endl;

    for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) {
        for (int columnCounter = 0; columnCounter < columnWidth; columnCounter++) {
            cout << grid[rowCounter][columnCounter];
        }
        cout << endl;
    }
}

void Occupancy_Grid::gridUpdate(int direction) {
    if (direction == UP) {
        grid[robotX][robotY + 1]++;
    } else if (direction == DOWN) {
        grid[robotX][robotY - 1]++;
    } else if (direction == LEFT) {
        grid[robotX - 1][robotY]++;
    } else if (direction == RIGHT) {
        grid[robotX + 1][robotY]++;
    }
}

void Occupancy_Grid::mapRobotLocation(int direction) {
    if (direction == UP) {
        robotY++; /* Move robot up one cell on the grid. */
    } else if (direction == DOWN) {
        robotY--; /* Move robot down one cell on the grid. */
    } else if (direction == LEFT) {
        robotX++; /* Move robot left one cell on the grid. */
    } else if (direction == RIGHT) {
        robotX--; /* Move robot right one cell on the grid. */
    }
}