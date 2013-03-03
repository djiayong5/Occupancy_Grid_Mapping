/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.cpp
 * Description: Stores member function declarations for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 03/03/2013
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include "Occupancy_Grid.h"

using namespace std;

Occupancy_Grid::Occupancy_Grid() {
    robotX = 1;
    robotY = 1;
    columnWidth = 3;
    rowHeight = 3;

    pathStack.resize(1);
    grid.resize(START_COLUMNS);
    for (int counter = 0; counter < START_COLUMNS; counter++) grid[counter].resize(START_ROWS);

    for (int rowCounter = 0; rowCounter < START_ROWS; rowCounter++) {
        for (int columnCounter = 0; columnCounter < START_COLUMNS; columnCounter++) {
            initialiseCell(&grid[rowCounter][columnCounter]);
            cout << "Initial Cell: " << rowCounter << ", " << columnCounter << " Explored = "
                    << grid[rowCounter][columnCounter].isExplored << ", Neighbours Unexplored = "
                    << grid[rowCounter][columnCounter].neighboursUnexplored << endl; 
        }
    }
    
    cout << endl;
}

void Occupancy_Grid::initialiseCell(Cell *cell) {
    cell->isExplored = false;
    cell->neighboursUnexplored = 4;
}

vector<Cell*> Occupancy_Grid::getPathStack() {
    return pathStack;
}

vector<vector<Cell> > Occupancy_Grid::getGrid() {
    return grid;
}

void Occupancy_Grid::shiftValuesDown() {
    robotY++; /* Adjust robot's location to match grid shifting. */

    for (int rowCounter = rowHeight - 1; rowCounter > 0; rowCounter--) {
        for (int columnCounter = 0; columnCounter < columnWidth; columnCounter++) {
            grid[rowCounter][columnCounter].isExplored = grid[rowCounter - 1][columnCounter].isExplored; /* Copies value to the cell below. */
            grid[rowCounter][columnCounter].neighboursUnexplored = grid[rowCounter - 1][columnCounter].neighboursUnexplored;
            grid[rowCounter][columnCounter].obstacleValue = grid[rowCounter - 1][columnCounter].obstacleValue;
            grid[rowCounter][columnCounter].yCoord = grid[rowCounter - 1][columnCounter].yCoord;
            grid[rowCounter][columnCounter].xCoord = grid[rowCounter - 1][columnCounter].xCoord;
            initialiseCell(&grid[rowCounter - 1][columnCounter]);
        }
    }
}

void Occupancy_Grid::shiftValuesRight() {
    robotX++; /* Adjust robot's location to match grid shifting. */

    for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) {
        for (int columnCounter = columnWidth - 1; columnCounter > 0; columnCounter--) {
            grid[rowCounter][columnCounter] = grid[rowCounter][columnCounter - 1]; /* Copies value to the cell to the right. */
            grid[rowCounter][columnCounter].isExplored = grid[rowCounter][columnCounter - 1].isExplored; /* Copies value to the cell below. */
            grid[rowCounter][columnCounter].neighboursUnexplored = grid[rowCounter][columnCounter - 1].neighboursUnexplored;
            grid[rowCounter][columnCounter].obstacleValue = grid[rowCounter][columnCounter - 1].obstacleValue;
            grid[rowCounter][columnCounter].yCoord = grid[rowCounter][columnCounter - 1].yCoord;
            grid[rowCounter][columnCounter].xCoord = grid[rowCounter][columnCounter - 1].xCoord;
            initialiseCell(&grid[rowCounter][columnCounter - 1]);
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

    if (directionToExpand == UP) {
        for (int columnCounter = 0; columnCounter < columnWidth; columnCounter++) initialiseCell(&grid[0][columnCounter]);
    } else if (directionToExpand == DOWN) {
        for (int columnCounter = 0; columnCounter < columnWidth; columnCounter++) initialiseCell(&grid[rowHeight - 1][columnCounter]);
    } else if (directionToExpand == RIGHT) {
        for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) initialiseCell(&grid[rowCounter][columnWidth - 1]);
    } else if (directionToExpand == LEFT) {
        for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) initialiseCell(&grid[rowCounter][0]);
    }
}

void Occupancy_Grid::printGrid() {
    cout << "Robot's Y location on grid: " << robotY << endl;
    cout << "Robot's X location on grid: " << robotX << endl;
    cout << "Number of rows: " << rowHeight << endl;
    cout << "Number of columns: " << columnWidth << endl;

    for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) {
        for (int columnCounter = 0; columnCounter < columnWidth; columnCounter++) {
            cout << grid[rowCounter][columnCounter].obstacleValue;
        }
        cout << endl;
    }
}

void Occupancy_Grid::mapRobotLocation(int direction) {
    if (direction == UP) robotY--; /* Move robot up one cell on the grid. */
    else if (direction == DOWN) robotY++; /* Move robot down one cell on the grid. */
    else if (direction == LEFT) robotX--; /* Move robot left one cell on the grid. */
    else if (direction == RIGHT) robotX++; /* Move robot right one cell on the grid. */
}

void Occupancy_Grid::checkOccupancy(int gridY, int gridX) {
    if (grid[gridY][gridX].obstacleValue == 0) grid[gridY][gridX].isExplored = false;
    else grid[gridY][gridX].isExplored = true;
}

void Occupancy_Grid::incrementCellUp() {
    grid[robotY - 1][robotX].obstacleValue++;
    checkOccupancy((robotY - 1), robotX);
}

void Occupancy_Grid::incrementCellDown() {
    grid[robotY + 1][robotX].obstacleValue++;
    checkOccupancy((robotY + 1), robotX);
}

void Occupancy_Grid::incrementCellLeft() {
    grid[robotY][robotX - 1].obstacleValue++;
    checkOccupancy(robotY, (robotX - 1));
}

void Occupancy_Grid::incrementCellRight() {
    grid[robotY][robotX + 1].obstacleValue++;
    checkOccupancy(robotY, (robotX + 1));
}

void Occupancy_Grid::decrementCellUp() {
    grid[robotY - 1][robotX].obstacleValue--;
    checkOccupancy((robotY - 1), robotX);
}

void Occupancy_Grid::decrementCellDown() {
    grid[robotY + 1][robotX].obstacleValue--;
    checkOccupancy((robotY + 1), robotX);
}

void Occupancy_Grid::decrementCellLeft() {
    grid[robotY][robotX - 1].obstacleValue--;
    checkOccupancy(robotY, (robotX - 1));
}

void Occupancy_Grid::decrementCellRight() {
    grid[robotY][robotX + 1].obstacleValue--;
    checkOccupancy(robotY, (robotX + 1));
}

void Occupancy_Grid::checkNeighbours() {
    int neighboursExplored = 0;
    if (grid[robotY + 1][robotX].isExplored == true) neighboursExplored++;
    if (grid[robotY - 1][robotX].isExplored == true) neighboursExplored++;
    if (grid[robotY][robotX + 1].isExplored == true) neighboursExplored++;
    if (grid[robotY][robotX - 1].isExplored == true) neighboursExplored++;

    grid[robotY][robotX].neighboursUnexplored = neighboursExplored;
}

void Occupancy_Grid::evaluateSonarReading(double sonarReading, int sonarFacing) {
    if (sonarReading <= 0.70) {
        if (sonarFacing == UP) incrementCellUp();
        if (sonarFacing == DOWN) incrementCellDown();
        if (sonarFacing == LEFT) incrementCellLeft();
        if (sonarFacing == RIGHT) incrementCellRight();
    }
}

void Occupancy_Grid::addCellToPath(double yCoord, double xCoord) {
    pathStack.push_back(&grid[robotY][robotX]); /* Add pointer of current cell to path stack. */
    grid[robotY][robotX].yCoord = yCoord;
    grid[robotY][robotX].yCoord = xCoord;
    grid[robotY][robotX].isExplored = true;
}

void Occupancy_Grid::removeCellFromPath() {
    pathStack.pop_back();
}

int Occupancy_Grid::chooseNextCell() {
    int randomDirection;
    int gridY;
    int gridX;

    do {
        srand(time(NULL));
        randomDirection = (rand() % 4);
        //cout << "Random Number: " << randomDirection << endl;

        if (randomDirection == 0) {
            randomDirection = UP;
            gridY = robotY - 1;
            gridX = robotX;
           // cout << "Cell Up Explored = " << grid[gridY][gridX].isExplored << endl;
        } else if (randomDirection == 1) {
            randomDirection = RIGHT;         
            gridY = robotY;
            gridX = robotX + 1;
          //  cout << "Cell Right Explored = " << grid[gridY][gridX].isExplored << endl;
        } else if (randomDirection == 2) {
            randomDirection = DOWN;
            gridY = robotY + 1;
            gridX = robotX;
          //  cout << "Cell Down Explored = " << grid[gridY][gridX].isExplored << endl;
        } else if (randomDirection == 3) {
            randomDirection = LEFT;
            gridY = robotY;
            gridX = robotX - 1;
          //  cout << "Cell Left Explored = " << grid[gridY][gridX].isExplored << endl;
        }
    } while (grid[gridY][gridX].isExplored == true);

    return randomDirection;
}

int Occupancy_Grid::getPreviousCellDirection(double currentY, double currentX) {
    if (pathStack.back()->yCoord < currentY) return UP;
    if (pathStack.back()->yCoord > currentY) return DOWN;
    if (pathStack.back()->xCoord < currentX) return LEFT;
    if (pathStack.back()->xCoord > currentX) return RIGHT;
}