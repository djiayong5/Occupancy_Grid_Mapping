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
    xLength = 3;
    yLength = 3;

    pathStack.resize(1);
    grid.resize(START_X_LENGTH);
    for (int counter = 0; counter < START_X_LENGTH; counter++) grid[counter].resize(START_Y_LENGTH);

    for (int xCounter = 0; xCounter < START_X_LENGTH; xCounter++) {
        for (int yCounter = 0; yCounter < START_Y_LENGTH; yCounter++) {
            initialiseCell(&grid[xCounter][yCounter]);
            cout << "Initial Cell: " << xCounter << ", " << yCounter << " Explored = "
                    << grid[xCounter][yCounter].isExplored << ", Neighbours Unexplored = "
                    << grid[xCounter][yCounter].neighboursUnexplored << endl;
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

/* Member function to shift all the values in the occupancy grid 1 cell up.*/
void Occupancy_Grid::shiftValuesUp() {
    robotY--; /* Adjust robot's location to match grid shifting. */

    for (int xCounter = 0; xCounter <= xLength; xCounter++) {
        for (int yCounter = yLength - 1; yCounter >= 0; yCounter--) {
            grid[xCounter][yCounter].isExplored = grid[xCounter][yCounter - 1].isExplored; //Copies value to the cell above.
            grid[xCounter][yCounter].neighboursUnexplored = grid[xCounter][yCounter - 1].neighboursUnexplored;
            grid[xCounter][yCounter].obstacleValue = grid[xCounter][yCounter - 1].obstacleValue;
            grid[xCounter][yCounter].yCoord = grid[xCounter][yCounter - 1].yCoord;
            grid[xCounter][yCounter].xCoord = grid[xCounter][yCounter - 1].xCoord;
            initialiseCell(&grid[xCounter][yCounter - 1]);
        }
    }
}

/* Member function to shift all the values in the occupancy grid right 1 cell. */
void Occupancy_Grid::shiftValuesRight() {
    robotX++; //Adjust robot's location to match grid shifting.

    for (int xCounter = xLength - 1; xCounter >= 0; xCounter--) {
        for (int yCounter = 0; yCounter <= yLength; yCounter++) {
            grid[xCounter][yCounter] = grid[xCounter - 1][yCounter]; //Copies value to the cell to the right.
            grid[xCounter][yCounter].isExplored = grid[xCounter - 1][yCounter].isExplored; //Copies value to the cell to the right.
            grid[xCounter][yCounter].neighboursUnexplored = grid[xCounter - 1][yCounter].neighboursUnexplored;
            grid[xCounter][yCounter].obstacleValue = grid[xCounter - 1][yCounter].obstacleValue;
            grid[xCounter][yCounter].yCoord = grid[xCounter - 1][yCounter].yCoord;
            grid[xCounter][yCounter].xCoord = grid[xCounter - 1][yCounter].xCoord;
            initialiseCell(&grid[xCounter - 1][yCounter]);
        }
    }
}

/* Member function to resize the occupancy grid in the direction passed in by 1 row/column.
 * Also calls relevant member functions to shift values when expanding UP or LEFT. 
 * @param directionToExpand The direction to expand the occupancy grid.
 */
void Occupancy_Grid::resizeGrid(int directionToExpand) {
    if (directionToExpand == ZERO || directionToExpand == ONE_EIGHTY) xLength++;
    else if (directionToExpand == NIGHTY || directionToExpand == MINUS_NIGHTY) xLength++;

    grid.resize(xLength); //Resizes grid xs.
    for (int xCounter = 0; xCounter < xLength; xCounter++) grid[xCounter].resize(yLength); //Resize grid ys.

    if (directionToExpand == NIGHTY) shiftValuesRight(); //Calls member function to shift the values right one.
    else if (directionToExpand == ONE_EIGHTY) shiftValuesUp(); //Calls member function to shift the values up one.

    if (directionToExpand == ZERO) {
        robotY++;
        for (int xCounter = 0; xCounter < xLength; xCounter++) initialiseCell(&grid[xCounter][yLength - 1]);
    } else if (directionToExpand == ONE_EIGHTY) {
        robotY--;
        for (int xCounter = 0; xCounter < xLength; xCounter++) initialiseCell(&grid[xCounter][0]);
    } else if (directionToExpand == MINUS_NIGHTY) {
        robotX++;
        for (int yCounter = 0; yCounter < yLength; yCounter++) initialiseCell(&grid[xLength - 1][yCounter]);
    } else if (directionToExpand == NIGHTY) {
        robotX--;
        for (int yCounter = 0; yCounter < yLength; yCounter++) initialiseCell(&grid[0][yCounter]);
    }
}

/* Member function to shrink grid. Only used at start to correct extra row/column being added.
 * @param directionToShrink The direction in which to shrink the grid by 1 whole row/column.
 */
void Occupancy_Grid::shrinkGrid(int directionToShrink) {
    if (directionToShrink == ZERO || directionToShrink == ONE_EIGHTY) yLength--;
    else if (directionToShrink == NIGHTY || directionToShrink == MINUS_NIGHTY) xLength--;

    grid.resize(xLength);
    for (int xCounter = 0; xCounter < xLength; xCounter++) grid[xCounter].resize(yLength);
}

/* Member function to print out the whole occupancy grid with the cells' obstacle values.
 * A '0' represents no obstacle present/empty cell and anything else represents an obstacle present/full cell.
 */
void Occupancy_Grid::printGrid() {
    cout << "Robot's X location on grid: " << robotX << endl;
    cout << "Robot's Y location on grid: " << robotY << endl;
    cout << "x Length: " << xLength << endl;
    cout << "y Length: " << yLength << endl;

    for (int xCounter = 0; xCounter < xLength; xCounter++) {
        for (int yCounter = 0; yCounter < yLength; yCounter++) {
            cout << grid[xCounter][yCounter].obstacleValue;
        }
        cout << endl;
    }

    cout << endl;
}

/* Member function to move the robot on the grid after it has moved in the real world.
 * @param direction The direction the robot moved.
 */
void Occupancy_Grid::mapRobotLocation(int direction) {
    if (direction == ZERO) robotY++; //Move robot up one cell on the grid.
    else if (direction == ONE_EIGHTY) robotY--; //Move robot down one cell on the grid.
    else if (direction == NIGHTY) robotX--; //Move robot left one cell on the grid.
    else if (direction == MINUS_NIGHTY) robotX++; //Move robot right one cell on the grid.
}

/* Member function to check the occupancy value of a cell and if the value >0, mark it as explored
 * to prevent the robot from exploring the cell/colliding into the obstacle.
 */
void Occupancy_Grid::checkOccupancy(int gridX, int gridY) {
    if (grid[gridX][gridY].obstacleValue > 0) grid[gridX][gridY].isExplored = true;
}

/* Member function to increment the cell's obstacleValue field value by 1.
 * @param gridY the 'y' index of the grid cell.
 * @param gridX the 'x' index of the grid cell.
 */
void Occupancy_Grid::incrementCell(int gridX, int gridY) {
    grid[gridX][gridY].obstacleValue++;
}

/* Member function to decrement the cell's obstacleValue field value 1.
 * @param gridY the 'y' index of the grid cell.
 * @param gridX the 'x' index of the grid cell.
 */
void Occupancy_Grid::decrementCell(int gridX, int gridY) {
    if (grid[gridX][gridY].obstacleValue > 0) {
        grid[gridX][gridY].obstacleValue--; //Decrements obstacle value of cell if it had a obstacle value >0.
        checkOccupancy(gridX, gridY); //Double checks occupancy after obstacle value decremented.
        if (grid[gridX][gridY].obstacleValue == 0) grid[gridX][gridY].isExplored = false; //Makes cell now explorable.
    }
}

/* Member function to return the isExplored field value of the cell the robot currently occupies.*/
bool Occupancy_Grid::getIsExplored() {
    return grid[robotX][robotY].isExplored;
}

/* Member function to check the neighbours of the cell the robot currently occupies for their exploration state.*/
void Occupancy_Grid::checkNeighbours() {
    int neighboursUnexplored = 0;
    if (grid[robotX + 1][robotY].isExplored == false) neighboursUnexplored++;
    if (grid[robotX - 1][robotY].isExplored == false) neighboursUnexplored++;
    if (grid[robotX][robotY + 1].isExplored == false) neighboursUnexplored++;
    if (grid[robotX][robotY - 1].isExplored == false) neighboursUnexplored++;

    grid[robotX][robotY].neighboursUnexplored = neighboursUnexplored;
}

/* Member function to return the neighboursUnexplored field value of the cell the robot currently occupies.*/
int Occupancy_Grid::getNeighboursUnexplored() {
    return grid[robotX][robotY].neighboursUnexplored;
}

/* Member function to evaluate the sonar reading and calling the relevant increment/decrement member functions.
 * @param sonarReading The sonar's reading.
 * @param sonarFacing The direction the sonar is facing.
 */
void Occupancy_Grid::evaluateSonarReading(double sonarReading, int sonarFacing) {
    cout << "Sonar Facing " << sonarFacing << ", Reading: " << sonarReading << endl;

    if (sonarReading <= 0.65) {
        if (sonarFacing == ZERO) incrementCell(robotX, robotY + 1);
        if (sonarFacing == ONE_EIGHTY) incrementCell(robotX, robotY - 1);
        if (sonarFacing == NIGHTY) incrementCell(robotX - 1, robotY);
        if (sonarFacing == MINUS_NIGHTY) incrementCell(robotX + 1, robotY);
    } else {
        if (sonarFacing == ZERO) decrementCell(robotX, robotY + 1);
        if (sonarFacing == ONE_EIGHTY) decrementCell(robotX, robotY - 1);
        if (sonarFacing == NIGHTY) decrementCell(robotX - 1, robotY);
        if (sonarFacing == MINUS_NIGHTY) decrementCell(robotX + 1, robotY);
    }
}

/* Member function to add the cell the robot currently occupies to the pathStack. */
void Occupancy_Grid::addCellToPath() {
    pathStack.push_back(&grid[robotX][robotY]); //Add pointer of current cell to path stack.
    grid[robotX][robotY].isExplored = true;
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
            randomDirection = ZERO;
            gridX = robotX;
            gridY = robotY + 1;           
        } else if (randomDirection == 1) {
            randomDirection = MINUS_NIGHTY;
            gridX = robotX + 1;
            gridY = robotY;
        } else if (randomDirection == 2) {
            randomDirection = ONE_EIGHTY;
            gridX = robotX;
            gridY = robotY - 1;
        } else if (randomDirection == 3) {
            randomDirection = NIGHTY;
            gridX = robotX - 1;
            gridY = robotY;    
        }
    } while (grid[gridX][gridY].isExplored == true);

    return randomDirection;
}

/* Member function to set the direction that*/
void Occupancy_Grid::setCellDirectionToComeFrom(int direction) {
    if (direction == ZERO) grid[robotX][robotY].directionToComeFrom = ONE_EIGHTY;
    else if (direction == ONE_EIGHTY) grid[robotX][robotY].directionToComeFrom = ZERO;
    else if (direction == NIGHTY) grid[robotX][robotY].directionToComeFrom = MINUS_NIGHTY;
    else if (direction == MINUS_NIGHTY) grid[robotX][robotY].directionToComeFrom = NIGHTY;
}