/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.cpp
 * Description: Stores member function declarations for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 08/03/2013
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

    grid.resize(START_X_LENGTH);
    for (int counter = 0; counter < START_X_LENGTH; counter++) grid[counter].resize(START_Y_LENGTH);

    for (int xCounter = 0; xCounter < START_X_LENGTH; xCounter++) {
        for (int yCounter = 0; yCounter < START_Y_LENGTH; yCounter++) {
            initialiseCell(&grid[xCounter][yCounter]);
        }
    }

    cout << endl;
}

/* Member function to initialise the cell passed in.*/
void Occupancy_Grid::initialiseCell(Cell *cell) {
    cell->isExplored = false;
    cell->neighboursUnexplored = 4;
    cell->obstacleValue = 0;
}

/* Member function to return the pathStack. */
vector<int> Occupancy_Grid::getPathStack() {
    return pathStack;
}

/* Member function to return the grid. */
vector<vector<Cell> > Occupancy_Grid::getGrid() {
    return grid;
}

/* Member function to shift all the values in the occupancy grid 1 cell up.*/
void Occupancy_Grid::shiftValuesUp() {
    for (int xCounter = 0; xCounter < xLength; xCounter++) {
        for (int yCounter = yLength - 1; yCounter > 0; yCounter--) {
            grid[xCounter][yCounter].isExplored = grid[xCounter][yCounter - 1].isExplored; //Copies value to the cell above.
            grid[xCounter][yCounter].neighboursUnexplored = grid[xCounter][yCounter - 1].neighboursUnexplored;
            grid[xCounter][yCounter].obstacleValue = grid[xCounter][yCounter - 1].obstacleValue;
            initialiseCell(&grid[xCounter][yCounter -1]);
        }
    }
    robotY++; //Corrects robot's position on grid.
}

/* Member function to shift all the values in the occupancy grid right 1 cell. */
void Occupancy_Grid::shiftValuesRight() {
    for (int xCounter = xLength - 1; xCounter > 0; xCounter--) {
        for (int yCounter = 0; yCounter < yLength; yCounter++) {
            grid[xCounter][yCounter].isExplored = grid[xCounter - 1][yCounter].isExplored; //Copies value to the cell to the right.
            grid[xCounter][yCounter].neighboursUnexplored = grid[xCounter - 1][yCounter].neighboursUnexplored;
            grid[xCounter][yCounter].obstacleValue = grid[xCounter - 1][yCounter].obstacleValue;
            initialiseCell(&grid[xCounter - 1][yCounter]);
        }
    }
    robotX++; //Corrects robot's position on grid.
}

/* Member function that expands the grid in the zero direction by 1 if needed. */
void Occupancy_Grid::resizeZero() {
    if (robotY == yLength - 1) {
        yLength++;
        grid.resize(xLength); //Resizes grid xs.
        for (int xCounter = 0; xCounter < xLength; xCounter++) grid[xCounter].resize(yLength); //Resize grid ys.
        for (int xCounter = 0; xCounter < xLength; xCounter++) initialiseCell(&grid[xCounter][yLength - 1]);
    }
}

/* Member function that expands the grid in the one eighty direction by 1 if needed. */
void Occupancy_Grid::resizeOneEighty() {
    if (robotY == 0) {
        yLength++;        for (int xCounter = 0; xCounter < xLength; xCounter++) initialiseCell(&grid[xCounter][0]);
        grid.resize(xLength); //Resizes grid xs.
        for (int xCounter = 0; xCounter < xLength; xCounter++) grid[xCounter].resize(yLength); //Resize grid ys
        shiftValuesUp();
    }   
}

/* Member function that expands the grid in the nighty direction by 1 if needed. */
void Occupancy_Grid::resizeNighty() {
    if (robotX == 0) {
        xLength++;
        grid.resize(xLength); //Resizes grid xs.
        for (int xCounter = 0; xCounter < xLength; xCounter++) grid[xCounter].resize(yLength); //Resize grid ys.
        shiftValuesRight();
    }
}

/* Member function that expands the grid in the minus nighty direction by 1 if needed. */
void Occupancy_Grid::resizeMinusNighty() {
    if (robotX == xLength - 1) {
        xLength++;
        grid.resize(xLength); //Resizes grid xs.
        for (int xCounter = 0; xCounter < xLength; xCounter++) grid[xCounter].resize(yLength); //Resize grid ys.   
        for (int yCounter = 0; yCounter < yLength; yCounter++) initialiseCell(&grid[xLength - 1][yCounter]);
    }
}

/* Member function to evaluate the direction to expand the grid and calls the relevant function.
 * @param directionToExpand The direction to expand the occupancy grid.
 */
void Occupancy_Grid::resizeGrid(int directionToExpand) {
    if (directionToExpand == ZERO) resizeZero();
    else if (directionToExpand == ONE_EIGHTY) resizeOneEighty();
    else if (directionToExpand == MINUS_NIGHTY) resizeMinusNighty();
    else if (directionToExpand == NIGHTY) resizeNighty();
}

/* Member function to shrink grid. Only used at start to correct extra row/column being added.
 * @param directionToShrink The direction in which to shrink the grid by 1 whole row/column.
 */
void Occupancy_Grid::shrinkGrid(int directionToShrink) {
    if (directionToShrink == ZERO || directionToShrink == ONE_EIGHTY) {
        robotY--;
        yLength--;
    } else if (directionToShrink == NIGHTY || directionToShrink == MINUS_NIGHTY) {
        robotX--;
        xLength--;
    }

    grid.resize(xLength);
    for (int xCounter = 0; xCounter < xLength; xCounter++) grid[xCounter].resize(yLength);
}

void Occupancy_Grid::moveRobotOnGrid(int direction) {
    if (direction == ZERO) robotY++;
    else if (direction == ONE_EIGHTY) robotY--;
    else if (direction == NIGHTY) robotX--;
    else if (direction == MINUS_NIGHTY) robotX++;
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
    checkOccupancy(gridX, gridY);
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

/* Member function to set the current cell's isExplored field value to true. */
void Occupancy_Grid::setIsExploredTrue() {
    grid[robotX][robotY].isExplored = true;
}

/* Member function to check the neighbours of the cell the robot currently occupies for their exploration state.*/
void Occupancy_Grid::checkNeighbours() {
    int neighboursUnexplored = 0;
    if (grid[robotX + 1][robotY].isExplored == false) neighboursUnexplored++;
    if (grid[robotX - 1][robotY].isExplored == false) neighboursUnexplored++;
    if (grid[robotX][robotY + 1].isExplored == false) neighboursUnexplored++;
    if (grid[robotX][robotY - 1].isExplored == false) neighboursUnexplored++;
    cout << "Cell 0 explored = " << grid[robotX][robotY + 1].isExplored << endl;
    cout << "Cell 180 explored = " << grid[robotX][robotY - 1].isExplored << endl;
    cout << "Cell 90 explored = " << grid[robotX - 1][robotY].isExplored << endl;
    cout << "Cell -90 explored = " << grid[robotX + 1][robotY].isExplored << endl << endl;
    

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

    if (sonarReading <= SONAR_OBSTACLE_RANGE) {
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
void Occupancy_Grid::addCellToPath(int direction) {
    pathStack.push_back(direction); //Add pointer of current cell to path stack.
}

/* Member function to set the direction that*/
int Occupancy_Grid::getDirectionOfLastCell() {
    int direction;
    
    if (pathStack.back() == ZERO) direction = ONE_EIGHTY;
    else if (pathStack.back() == ONE_EIGHTY) direction = ZERO;
    else if (pathStack.back() == NIGHTY) direction = MINUS_NIGHTY;
    else if (pathStack.back() == MINUS_NIGHTY) direction = NIGHTY;
    
    pathStack.pop_back();
    return direction;
}

/* Member function to randomly choose the next direction to travel in while making sure that the cell in the
 * direction randomised has not already been explored.
 */
int Occupancy_Grid::chooseNextCell() {
    int randomDirection;
    int gridX;
    int gridY;

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