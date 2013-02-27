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
    //grid = new int*[3];
    /*grid = calloc(grid, sizeof (int*) * 3); /* Allocates memory for 3 columns of the grid and sets values to 0. */

    /* for (counter = 0; counter < columnWidth; counter++) {
        grid[counter] = calloc(grid[counter], sizeof (int) * 3); /* Allocates memory for 3 rows of the grid and sets values to 0. */
    /*}*/
}

vector<vector<int> > Occupancy_Grid::getGrid() {
    return grid;
}

void Occupancy_Grid::shiftValuesDown() {
    int columnCounter = 0;
    int rowCounter;

    for (columnCounter; columnCounter < columnWidth; columnCounter++) {
        for (rowCounter = 1; rowCounter < rowHeight; rowCounter++) {
            grid[columnCounter][rowCounter - 1] = grid[columnCounter][rowCounter]; /* Copies value to the cell below. */
            //grid[columnCounter][rowCounter] = 0; /* Sets value of cell to 0. */
        }
    }
}

void Occupancy_Grid::shiftValuesRight() {
    int columnCounter;
    int rowCounter = 0;

    for (columnCounter = columnWidth - 1; columnCounter >= 0; columnCounter--) {
        for (rowCounter; rowCounter < rowHeight; rowCounter++) {
            grid[columnCounter + 1][rowCounter] = grid[columnCounter][rowCounter]; /* Copies values to the cell to the right. */
            //grid[columnCounter][rowCounter] = 0; /* Sets value of cell to 0. */
        }
    }
}

void Occupancy_Grid::resizeGrid(int directionToExpand) {
    if (directionToExpand == UP || directionToExpand == DOWN) {
        rowHeight++;
        grid.resize(rowHeight);
        //grid = realloc(grid, sizeof (int*) * rowHeight); /* Increases grid's row height by 1, allocating memory for it. */

        for (int counter = 0; counter < columnWidth; counter++) {
            grid[counter][rowHeight - 1] = 0; /* Set values of new cells to 0. */
        }

        if (directionToExpand == DOWN) shiftValuesDown();
    } else if (directionToExpand == LEFT || directionToExpand == RIGHT) {
        columnWidth++;
        grid.resize(columnWidth);
        //grid = realloc(grid, sizeof (int*) * columnWidth); /* Increases grid's column length by 1, allocating memory for it. */

        for (int counter = 0; counter < rowHeight; counter++) {
            grid[columnWidth - 1][counter] = 0; /* Set values of new cells to 0. */
        }

        if (directionToExpand == LEFT) shiftValuesRight();
    }
}

void Occupancy_Grid::printGrid() {
    cout << "Number of columns: " << columnWidth << endl;
    cout << "Number of rows: " << rowHeight << endl;

    for (int columnCounter = 0; columnCounter < columnWidth; columnCounter++) {
        for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) {
            cout << grid[columnCounter][rowCounter];
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