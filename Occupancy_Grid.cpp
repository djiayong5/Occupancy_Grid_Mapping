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
    robotX = 1;
    robotY = 1;
    columnWidth = 3;
    rowHeight = 3;

    grid.resize(START_COLUMNS);

    for (int counter = 0; counter < START_COLUMNS; counter++) grid[counter].resize(START_ROWS);
}

vector<vector<int> > Occupancy_Grid::getGrid() {
    return grid;
}

void Occupancy_Grid::shiftValuesDown() {
    robotY++; /* Adjust robot's location to match grid shifting. */

    for (int rowCounter = rowHeight - 1; rowCounter > 0; rowCounter--) {
        for (int columnCounter = 0; columnCounter < columnWidth; columnCounter++) {
            grid[rowCounter][columnCounter] = grid[rowCounter - 1][columnCounter]; /* Copies value to the cell below. */
            grid[rowCounter - 1][columnCounter] = 0; /* Sets value of cell to 0. */
        }
    }
}

void Occupancy_Grid::shiftValuesRight() {
    robotX++; /* Adjust robot's location to match grid shifting. */

    for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) {
        for (int columnCounter = columnWidth - 1; columnCounter > 0; columnCounter--) {
            grid[rowCounter][columnCounter] = grid[rowCounter][columnCounter - 1]; /* Copies value to the cell to the right. */
            grid[rowCounter][columnCounter - 1] = 0; /* Sets value of cell to 0. */
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
    cout << "Robot's Y location on grid: " << robotY << endl;
    cout << "Robot's X location on grid: " << robotX << endl;
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
    if (direction == UP) grid[robotX][robotY - 1]++;
    else if (direction == DOWN) grid[robotX][robotY + 1]++;
    else if (direction == LEFT) grid[robotX - 1][robotY]++;
    else if (direction == RIGHT)grid[robotX + 1][robotY]++;
}

void Occupancy_Grid::mapRobotLocation(int direction) {
    if (direction == UP) robotY--; /* Move robot up one cell on the grid. */
    else if (direction == DOWN) robotY++; /* Move robot down one cell on the grid. */
    else if (direction == LEFT) robotX--; /* Move robot left one cell on the grid. */
    else if (direction == RIGHT) robotX++; /* Move robot right one cell on the grid. */
}

void Occupancy_Grid::incrementCellUp() {
    grid[robotY - 1][robotX]++;
}

void Occupancy_Grid::incrementCellDown() {
    grid[robotY + 1][robotX]++;
}

void Occupancy_Grid::incrementCellLeft() {
    grid[robotY][robotX - 1]++;
}

void Occupancy_Grid::incrementCellRight() {
    grid[robotY][robotX + 1]++;
}

void Occupancy_Grid::decrementCellUp() {
    grid[robotY - 1][robotX]--;
}

void Occupancy_Grid::decrementCellDown() {
    grid[robotY + 1][robotX]--;
}

void Occupancy_Grid::decrementCellLeft() {
    grid[robotY][robotX - 1]--;
}

void Occupancy_Grid::decrementCellRight() {
    grid[robotY][robotX + 1]--;
}

void Occupancy_Grid::evaluateSonarReading(double sonarReading, int sonarFacing) {
    if (sonarReading <= 0.70) {
        if (sonarFacing == UP) incrementCellUp();
        if (sonarFacing == DOWN) incrementCellDown();
        if (sonarFacing == LEFT) incrementCellLeft();
        if (sonarFacing == RIGHT) incrementCellRight();
    }
}