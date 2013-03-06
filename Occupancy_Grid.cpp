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

/* Default constructor. */
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

/* Member function to initialise the cell passed in.*/
void Occupancy_Grid::initialiseCell(Cell *cell) {
    cell->isExplored = false;
    cell->neighboursUnexplored = 4;
}

/* Member function to return the pathStack. */
vector<Cell*> Occupancy_Grid::getPathStack() {
    return pathStack;
}

/* Member function to return the grid. */
vector<vector<Cell> > Occupancy_Grid::getGrid() {
    return grid;
}

/* Member function to shift all the values in the occupancy grid 1 cell down.*/
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

/* Member function to shift all the values in the occupancy grid right 1 cell. */
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

/* Member function to resize the occupancy grid in the direction passed in by 1 row/column.
 * Also calls relevant member functions to shift values when expanding UP or LEFT. 
 * @param directionToExpand The direction to expand the occupancy grid.
 */
void Occupancy_Grid::resizeGrid(int directionToExpand) {
    if (directionToExpand == UP || directionToExpand == DOWN) rowHeight++;
    else if (directionToExpand == LEFT || directionToExpand == RIGHT) columnWidth++;

    grid.resize(rowHeight); //Resizes grid rows.
    for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) grid[rowCounter].resize(columnWidth); //Resize grid columns.

    if (directionToExpand == UP) shiftValuesDown(); //Calls member function to shift the values down one.
    else if (directionToExpand == LEFT) shiftValuesRight(); //Calls member function to shift the values right one.

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

/* Member function to shrink grid. Only used at start to correct extra row/column being added.
 * @param directionToShrink The direction in which to shrink the grid by 1 whole row/column.
 */
void Occupancy_Grid::shrinkGrid(int directionToShrink) {
    if (directionToShrink == UP || directionToShrink == DOWN) rowHeight--;
    else if (directionToShrink == LEFT || directionToShrink == RIGHT) columnWidth--;

    grid.resize(rowHeight);
    for (int rowCounter = 0; rowCounter < rowHeight; rowCounter++) grid[rowCounter].resize(columnWidth);
}

/* Member function to print out the whole occupancy grid with the cells' obstacle values.
 * A '0' represents no obstacle present/empty cell and anything else represents an obstacle present/full cell.
 */
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

    cout << endl;
}

/* Member function to move the robot on the grid after it has moved in the real world.
 * @param direction The direction the robot moved.
 */
void Occupancy_Grid::mapRobotLocation(int direction) {
    if (direction == UP) robotY--; /* Move robot up one cell on the grid. */
    else if (direction == DOWN) robotY++; /* Move robot down one cell on the grid. */
    else if (direction == LEFT) robotX--; /* Move robot left one cell on the grid. */
    else if (direction == RIGHT) robotX++; /* Move robot right one cell on the grid. */
}

/* Member function to check the occupancy value of a cell and if the value >0, mark it as explored
 * to prevent the robot from exploring the cell/colliding into the obstacle.
 */
void Occupancy_Grid::checkOccupancy(int gridY, int gridX) {
    if (grid[gridY][gridX].obstacleValue > 0) grid[gridY][gridX].isExplored = true;
}

/* Member function to increment the cell's obstacleValue field value by 1.
 * @param gridY the 'y' index of the grid cell.
 * @param gridX the 'x' index of the grid cell.
 */
void Occupancy_Grid::incrementCell(int gridY, int gridX) {
    grid[gridY][gridX].obstacleValue++;
}

/* Member function to decrement the cell's obstacleValue field value 1.
 * @param gridY the 'y' index of the grid cell.
 * @param gridX the 'x' index of the grid cell.
 */
void Occupancy_Grid::decrementCell(int gridY, int gridX) {
    if (grid[gridY][gridX].obstacleValue > 0) {
        grid[gridY][gridX].obstacleValue--; //Decrements obstacle value of cell if it had a obstacle value >0.
        checkOccupancy(gridY, gridX); //Double checks occupancy after obstacle value decremented.
        if (grid[gridY][gridX].obstacleValue == 0) grid[gridY][gridX].isExplored = false; //Makes cell now explorable.
    }
}

/* Member function to return the isExplored field value of the cell the robot currently occupies.*/
bool Occupancy_Grid::getIsExplored() {
    return grid[robotY][robotX].isExplored;
}

/* Member function to check the neighbours of the cell the robot currently occupies for their exploration state.*/
void Occupancy_Grid::checkNeighbours() {
    int neighboursUnexplored = 0;
    if (grid[robotY + 1][robotX].isExplored == false) neighboursUnexplored++;
    if (grid[robotY - 1][robotX].isExplored == false) neighboursUnexplored++;
    if (grid[robotY][robotX + 1].isExplored == false) neighboursUnexplored++;
    if (grid[robotY][robotX - 1].isExplored == false) neighboursUnexplored++;

    grid[robotY][robotX].neighboursUnexplored = neighboursUnexplored;
}

/* Member function to return the neighboursUnexplored field value of the cell the robot currently occupies.*/
int Occupancy_Grid::getNeighboursUnexplored() {
    return grid[robotY][robotX].neighboursUnexplored;
}

/* Member function to evaluate the sonar reading and calling the relevant increment/decrement member functions.
 * @param sonarReading The sonar's reading.
 * @param sonarFacing The direction the sonar is facing.
 */
void Occupancy_Grid::evaluateSonarReading(double sonarReading, int sonarFacing) {
    cout << "Sonar Facing " << sonarFacing << ", Reading: " << sonarReading << endl;

    if (sonarReading <= 0.65) {
        if (sonarFacing == UP) incrementCell(robotY - 1, robotX);
        if (sonarFacing == DOWN) incrementCell(robotY + 1, robotX);
        if (sonarFacing == LEFT) incrementCell(robotY, robotX - 1);
        if (sonarFacing == RIGHT) incrementCell(robotY, robotX + 1);
    } else {
        if (sonarFacing == UP) decrementCell(robotY - 1, robotX);
        if (sonarFacing == DOWN) decrementCell(robotY + 1, robotX);
        if (sonarFacing == LEFT) decrementCell(robotY, robotX - 1);
        if (sonarFacing == RIGHT) decrementCell(robotY, robotX + 1);
    }
}

/* Member function to add the cell the robot currently occupies to the pathStack. */
void Occupancy_Grid::addCellToPath() {
    pathStack.push_back(&grid[robotY][robotX]); /* Add pointer of current cell to path stack. */
    grid[robotY][robotX].isExplored = true;
}

/* Member function to remove the cell that the robot currently occupies from the pathStack. */
void Occupancy_Grid::removeCellFromPath() {
    pathStack.pop_back();
}

/* Member function to random choose the next direction to travel in while making sure that the cell in the
 * direction randomised has not already been explored.
 */
int Occupancy_Grid::chooseNextCell() {
    int randomDirection;
    int gridY;
    int gridX;

    do {
        srand(time(NULL));
        randomDirection = (rand() % 4);

        if (randomDirection == 0) {
            randomDirection = UP;
            gridY = robotY - 1;
            gridX = robotX;
        } else if (randomDirection == 1) {
            randomDirection = RIGHT;
            gridY = robotY;
            gridX = robotX + 1;
        } else if (randomDirection == 2) {
            randomDirection = DOWN;
            gridY = robotY + 1;
            gridX = robotX;
        } else if (randomDirection == 3) {
            randomDirection = LEFT;
            gridY = robotY;
            gridX = robotX - 1;
        }
    } while (grid[gridY][gridX].isExplored == true);

    return randomDirection;
}

/* Member function to set the direction that*/
void Occupancy_Grid::setCellDirectionToComeFrom(int direction) {
    if (direction == UP) grid[robotY][robotX].directionToComeFrom = DOWN;
    else if (direction == DOWN) grid[robotY][robotX].directionToComeFrom = UP;
    else if (direction == LEFT) grid[robotY][robotX].directionToComeFrom = RIGHT;
    else if (direction == RIGHT) grid[robotY][robotX].directionToComeFrom = LEFT;
}