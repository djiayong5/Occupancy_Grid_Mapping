/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.cpp
 * Description: Stores member function declarations for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 07/04/2013
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "Occupancy_Grid.h"

using namespace std;

/** Default constructor. */
Occupancy_Grid::Occupancy_Grid() {
    mapConfigure();
}

void Occupancy_Grid::mapConfigure() {
    robotX = 1;
    robotY = 1;
    xLength = 3;
    yLength = 3;
    pathLength = 0;
    anomalyFound = false;

    grid.resize(START_X_LENGTH);
    for (int counter = 0; counter < START_X_LENGTH; counter++) grid[counter].resize(START_Y_LENGTH);

    for (int xCounter = 0; xCounter < START_X_LENGTH; xCounter++) {
        for (int yCounter = 0; yCounter < START_Y_LENGTH; yCounter++) {
            initialiseCell(&grid[xCounter][yCounter]);
        }
    }

    cout << endl;
}

int Occupancy_Grid::getXLength() {
    return xLength;
}

int Occupancy_Grid::getYLength() {
    return yLength;
}

int Occupancy_Grid::getRobotX() {
    return robotX;
}

int Occupancy_Grid::getRobotY() {
    return robotY;
}

bool Occupancy_Grid::getAnomalyFound() {
    return anomalyFound;
}

/** Member function to initialise the cell passed in.*/
void Occupancy_Grid::initialiseCell(Cell *cell) {
    cell->isExplored = false;
    cell->neighboursUnexplored = 4;
    cell->obstacleValue = 0;
}

/** Member function to return the pathStack. */
vector<Tracker> Occupancy_Grid::getPathStack() {
    return pathStack;
}

/** Member function to return the grid. */
vector<vector<Cell> > Occupancy_Grid::getGrid() {
    return grid;
}

void Occupancy_Grid::shiftTrackers(int xShift, int yShift) {
    if (!pathStack.empty()) {
        for (int counter = 0; counter < pathLength; counter++) {
            pathStack.at(counter).xPos += xShift;
            pathStack.at(counter).yPos += yShift;
        }
    }
}

/** Member function to shift all the values in the occupancy grid 1 cell up.*/
void Occupancy_Grid::shiftValuesUp() {
    for (int xCounter = 0; xCounter < xLength; xCounter++) {
        for (int yCounter = yLength - 1; yCounter > 0; yCounter--) {
            grid[xCounter][yCounter].isExplored = grid[xCounter][yCounter - 1].isExplored; //Copies value to the cell above.
            grid[xCounter][yCounter].neighboursUnexplored = grid[xCounter][yCounter - 1].neighboursUnexplored;
            grid[xCounter][yCounter].obstacleValue = grid[xCounter][yCounter - 1].obstacleValue;
            initialiseCell(&grid[xCounter][yCounter - 1]);
        }
    }
    robotY++; //Corrects robot's position on grid.
}

/** Member function to shift all the values in the occupancy grid right 1 cell. */
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

/** Member function that expands the grid in the zero direction by 1 if needed. */
void Occupancy_Grid::resizeZero() {
    yLength++;
    for (int counter = 0; counter < xLength; counter++) grid[counter].resize(yLength); //Resize grid ys.
    for (int counter = 0; counter < xLength; counter++) initialiseCell(&grid[counter][yLength - 1]);
}

/** Member function that expands the grid in the one eighty direction by 1 if needed. */
void Occupancy_Grid::resizeOneEighty() {
    yLength++;
    for (int counter = 0; counter < xLength; counter++) grid[counter].resize(yLength); //Resize grid ys
    shiftValuesUp();
    shiftTrackers(0, 1);
}

/** Member function that expands the grid in the nighty direction by 1 if needed. */
void Occupancy_Grid::resizeNighty() {
    xLength++;
    grid.resize(xLength); //Resizes grid xs.
    for (int counter = 0; counter < xLength; counter++) grid[counter].resize(yLength); //Resize grid ys.
    shiftValuesRight();
    shiftTrackers(1, 0);
}

/** Member function that expands the grid in the minus nighty direction by 1 if needed. */
void Occupancy_Grid::resizeMinusNighty() {
    xLength++;
    grid.resize(xLength); //Resizes grid xs.
    for (int counter = 0; counter < xLength; counter++) grid[counter].resize(yLength); //Resize grid ys.   
    for (int counter = 0; counter < yLength; counter++) initialiseCell(&grid[xLength - 1][counter]);
}

/** Member function to evaluate if the grid needs expanding.
   @param currentDirection the current direction the robot is facing.
 */
void Occupancy_Grid::checkResizeNeeded(int currentDirection) {
    if (robotX == xLength - 1 || robotX == 0 || robotY == yLength - 1 || robotY == 0) resizeGrid(currentDirection);
}

/** Member function to evaluate the direction to expand the grid and calls the relevant function.
 * @param directionToExpand The direction to expand the occupancy grid.
 */
void Occupancy_Grid::resizeGrid(int directionToExpand) {
    if (directionToExpand == ZERO) resizeZero();
    else if (directionToExpand == ONE_EIGHTY) resizeOneEighty();
    else if (directionToExpand == MINUS_NIGHTY) resizeMinusNighty();
    else if (directionToExpand == NIGHTY) resizeNighty();
}

void Occupancy_Grid::moveRobotOnGrid(int direction) {
    if (direction == ZERO) robotY++;
    else if (direction == ONE_EIGHTY) robotY--;
    else if (direction == NIGHTY) robotX--;
    else if (direction == MINUS_NIGHTY) robotX++;
}

/** Member function to print out the whole occupancy grid with the cells' obstacle values.
 * A '0' represents no obstacle present/empty cell and anything else represents an obstacle present/full cell.
 */
void Occupancy_Grid::printGrid() {
    cout << "Robot's X location on grid: " << robotX << endl;
    cout << "Robot's Y location on grid: " << robotY << endl;
    cout << "x Length: " << xLength << endl;
    cout << "y Length: " << yLength << endl;

    for (int xCounter = 0; xCounter < xLength; xCounter++) {
        for (int yCounter = 0; yCounter < yLength; yCounter++) {
            if (xCounter == robotX && yCounter == robotY) {
                cout << "R";
            } else if (grid[xCounter][yCounter].isExplored == true && grid[xCounter][yCounter].obstacleValue == 0) {
                cout << " ";
            } else if (grid[xCounter][yCounter].isExplored == false && grid[xCounter][yCounter].obstacleValue == 0) {
                cout << "~";
            } else {
                //cout << grid[xCounter][yCounter].obstacleValue;
                cout << "#";
            }
        }
        cout << endl;
    }
    cout << endl;
}

/** Member function to check the occupancy value of a cell and if the value >0, mark it as explored
 * to prevent the robot from exploring the cell/colliding into the obstacle.
 */
void Occupancy_Grid::checkOccupancy(int gridX, int gridY) {
    if (grid[gridX][gridY].obstacleValue > 0) grid[gridX][gridY].isExplored = true;
}

/** Member function to increment the cell's obstacleValue field value by 1.
 * @param gridY the 'y' index of the grid cell.
 * @param gridX the 'x' index of the grid cell.
 */
void Occupancy_Grid::incrementCell(int gridX, int gridY) {
    grid[gridX][gridY].obstacleValue++;
    checkOccupancy(gridX, gridY);
}

/** Member function to decrement the cell's obstacleValue field value 1.
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

/** Member function to set the current cell's isExplored field value to true. */
void Occupancy_Grid::setIsExploredTrue() {
    grid[robotX][robotY].isExplored = true;
    checkPathExploration();
}

/** Member function to check the neighbours of the cell the robot currently occupies for their exploration state.*/
void Occupancy_Grid::checkNeighbours(int xPos, int yPos) {
    int neighboursUnexplored = 0;
    if (grid[xPos + 1][yPos].isExplored == false) neighboursUnexplored++;
    if (grid[xPos - 1][yPos].isExplored == false) neighboursUnexplored++;
    if (grid[xPos][yPos + 1].isExplored == false) neighboursUnexplored++;
    if (grid[xPos][yPos - 1].isExplored == false) neighboursUnexplored++;
    grid[xPos][yPos].neighboursUnexplored = neighboursUnexplored;
}

void Occupancy_Grid::checkCellNeighbours() {
    checkNeighbours(robotX, robotY);
}

void Occupancy_Grid::checkPathExploration() {
    for (int counter = 0; counter < pathLength; counter++) {
        checkNeighbours(pathStack.at(counter).xPos, pathStack.at(counter).yPos);
    }
}

/** Member function to return the neighboursUnexplored field value of the cell the robot currently occupies.*/
int Occupancy_Grid::getNeighboursUnexplored() {
    return grid[robotX][robotY].neighboursUnexplored;
}

/** Member function to evaluate the sonar reading and calling the relevant increment/decrement member functions.
 * @param sonarReading The sonar's reading.
 * @param sonarFacing The direction the sonar is facing.
 */
void Occupancy_Grid::calculateCellToChange(int sonarFacing, bool obstaclePresent, bool seekMode) {
    if (obstaclePresent == true) {
        if (seekMode == false) {
            if (sonarFacing == ZERO) incrementCell(robotX, robotY + 1);
            if (sonarFacing == ONE_EIGHTY) incrementCell(robotX, robotY - 1);
            if (sonarFacing == NIGHTY) incrementCell(robotX - 1, robotY);
            if (sonarFacing == MINUS_NIGHTY) incrementCell(robotX + 1, robotY);
            if (sonarFacing == FOURTY_FIVE) incrementCell(robotX - 1, robotY + 1);
            if (sonarFacing == ONE_THIRTY_FIVE) incrementCell(robotX - 1, robotY - 1);
            if (sonarFacing == MINUS_FOURTY_FIVE) incrementCell(robotX + 1, robotY + 1);
            if (sonarFacing == MINUS_ONE_THIRTY_FIVE) incrementCell(robotX + 1, robotY - 1);
        } else {
            if (sonarFacing == ZERO) detectAnomaly(robotX, robotY + 1);
            if (sonarFacing == ONE_EIGHTY) detectAnomaly(robotX, robotY - 1);
            if (sonarFacing == NIGHTY) detectAnomaly(robotX - 1, robotY);
            if (sonarFacing == MINUS_NIGHTY) detectAnomaly(robotX + 1, robotY);
            if (sonarFacing == FOURTY_FIVE) detectAnomaly(robotX - 1, robotY + 1);
            if (sonarFacing == ONE_THIRTY_FIVE) detectAnomaly(robotX - 1, robotY - 1);
            if (sonarFacing == MINUS_FOURTY_FIVE) detectAnomaly(robotX + 1, robotY + 1);
            if (sonarFacing == MINUS_ONE_THIRTY_FIVE) detectAnomaly(robotX + 1, robotY - 1);
        }
    } else {
        if (sonarFacing == ZERO) decrementCell(robotX, robotY + 1);
        if (sonarFacing == ONE_EIGHTY) decrementCell(robotX, robotY - 1);
        if (sonarFacing == NIGHTY) decrementCell(robotX - 1, robotY);
        if (sonarFacing == MINUS_NIGHTY) decrementCell(robotX + 1, robotY);
        if (sonarFacing == FOURTY_FIVE) decrementCell(robotX - 1, robotY + 1);
        if (sonarFacing == ONE_THIRTY_FIVE) decrementCell(robotX - 1, robotY - 1);
        if (sonarFacing == MINUS_FOURTY_FIVE) decrementCell(robotX + 1, robotY + 1);
        if (sonarFacing == MINUS_ONE_THIRTY_FIVE) decrementCell(robotX + 1, robotY - 1);
    }
}

void Occupancy_Grid::mapPath(int direction) {
    addCellToPath(direction, robotX, robotY);
}

/** Member function to add the cell the robot currently occupies to the pathStack. */
void Occupancy_Grid::addCellToPath(int direction, int xPos, int yPos) {
    pathStack.resize(++pathLength); //Add pointer of current cell to path stack.
    pathStack.back().xPos = xPos;
    pathStack.back().yPos = yPos;
    pathStack.back().directionLeftFrom = direction;
}

/** Member function to set the direction that*/
int Occupancy_Grid::getDirectionOfLastCell() {
    int direction;

    if (pathStack.back().directionLeftFrom == ZERO) direction = ONE_EIGHTY;
    else if (pathStack.back().directionLeftFrom == ONE_EIGHTY) direction = ZERO;
    else if (pathStack.back().directionLeftFrom == NIGHTY) direction = MINUS_NIGHTY;
    else if (pathStack.back().directionLeftFrom == MINUS_NIGHTY) direction = NIGHTY;

    pathStack.pop_back();
    pathLength--;
    return direction;
}

/** Member function to randomly choose the next direction to travel in while making sure that the cell in the
 * direction randomised has not already been explored.
 */
int Occupancy_Grid::chooseNextCell(int currentDirection) {
    int randomDirection;
    int gridX;
    int gridY;

    if (currentDirection == ZERO && grid[robotX][robotY + 1].isExplored == false) return ZERO;
    else if (currentDirection == ONE_EIGHTY && grid[robotX][robotY - 1].isExplored == false) return ONE_EIGHTY;
    else if (currentDirection == NIGHTY && grid[robotX - 1][robotY].isExplored == false) return NIGHTY;
    else if (currentDirection == MINUS_NIGHTY && grid[robotX + 1][robotY].isExplored == false) return MINUS_NIGHTY;

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

/** Member function to evaluate if the robot has finished mapping an area. */
bool Occupancy_Grid::checkFinished() {
    cout << "Checking if finished..." << endl;

    for (int counter = 0; counter < pathLength; counter++) {
        if (grid[pathStack.at(counter).xPos][pathStack.at(counter).yPos].isExplored == true &&
                grid[pathStack.at(counter).xPos][pathStack.at(counter).yPos].obstacleValue == 0
                && grid[pathStack.at(counter).xPos][pathStack.at(counter).yPos].neighboursUnexplored != 0) return false;
    }

    pathLength = 0;
    pathStack.erase(pathStack.begin(), pathStack.end());
    return true;
}

int Occupancy_Grid::attemptLocalisation(Occupancy_Grid *temp) {
    int possibleAreas = 0;

    for (int xCounter = 0; xCounter < (xLength - temp->getXLength()); xCounter++) {
        for (int yCounter = 0; yCounter < (yLength - temp->getYLength()); yCounter++) {
            if (compareArea(temp, xCounter, yCounter)) possibleAreas++;
            if (possibleAreas > 1) return 2; //More than one possible area.
        }
    }

    if (possibleAreas == 1) return 3; //True.
    else return 1; //False.
}

bool Occupancy_Grid::compareCells(Cell cell1, Cell cell2) {
    if (cell1.obstacleValue > 0 && cell2.obstacleValue > 0) return true;
    else if (cell2.obstacleValue == 0) return true;

    return false;
}

bool Occupancy_Grid::compareArea(Occupancy_Grid *temp, int xCounter, int yCounter) {
    int xEnd = xCounter + temp->getXLength() - 1;
    int yEnd = yCounter + temp->getYLength() - 1;
    int tempXCounter = 0;
    int tempYCounter = 0;

    while (xCounter <= xEnd && yCounter <= yEnd) {
        if (!compareCells(grid[xCounter][yCounter], temp->getGrid()[tempXCounter][tempYCounter])) {
            possibleRobotX = 0;
            possibleRobotY = 0;
            return false;
        }

        if (tempXCounter == temp->getRobotX() && tempYCounter == temp->getRobotY()) {
            possibleRobotX = xCounter;
            possibleRobotY = yCounter;
        }

        if (xCounter >= temp->getXLength() - 1) {
            yCounter++;
            tempYCounter++;
        } else {
            xCounter++;
            tempXCounter++;
        }
    }

    return true;
}

void Occupancy_Grid::switchGrid() {
    robotX = possibleRobotX;
    robotY = possibleRobotY;
}

void Occupancy_Grid::seekConfigure() {
    for (int xCounter = 0; xCounter < xLength; xCounter++) {
        for (int yCounter = 0; yCounter < yLength; yCounter++) {
            if (grid[xCounter][yCounter].obstacleValue == 0 && grid[xCounter][yCounter].isExplored == true) {
                grid[xCounter][yCounter].isExplored = false;
            }
        }
    }
}

bool Occupancy_Grid::detectAnomaly(int xPos, int yPos) {
    if (grid[xPos][yPos].obstacleValue == 0) {
        cout << "Anomaly found at grid co-ordinates (" << xPos << ", " << yPos << ")" << endl;
        anomalyFound = true;
        return true;
    } else return false;
}

void Occupancy_Grid::getHideLocation() {
    if (!findLocation(3)) {
        cout << "Could not find a location with 3 walls surrounding. Trying 2 wall locations..." << endl;

        if (!findLocation(2)) {
            cout << "Could not find a location with 2 walls surrounding. Trying 1 wall locations..." << endl;
            findLocation(1);
        }
    }
}

bool Occupancy_Grid::findLocation(int numberOfWalls) {
    for (int xCounter = 0; xCounter < xLength; xCounter++) {
        for (int yCounter = 0; yCounter < yLength; yCounter++) {
            if (grid[xCounter][yCounter].obstacleValue == 0 && grid[xCounter][yCounter].isExplored == true) {
                if (getNumberOfWalls(xCounter, yCounter) == numberOfWalls) {
                    frontier.resize(++frontierLength);
                    frontier.back().xPos = robotX;
                    frontier.back().yPos = robotY;
                    frontier.back().pathValue = 0;
                    if (plotPath(robotX, robotY, xCounter, yCounter, 0)) {
                        cout << "Found a location surrounded by " << numberOfWalls << " walls and plotted path." << endl;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

int Occupancy_Grid::getNumberOfWalls(int xPos, int yPos) {
    int numberOfWalls = 0;

    if (grid[xPos + 1][yPos].obstacleValue > 0) numberOfWalls++;
    if (grid[xPos - 1][yPos].obstacleValue > 0) numberOfWalls++;
    if (grid[xPos][yPos + 1].obstacleValue > 0) numberOfWalls++;
    if (grid[xPos][yPos - 1].obstacleValue > 0) numberOfWalls++;

    return numberOfWalls;
}

bool Occupancy_Grid::plotPath(int currentX, int currentY, int targetX, int targetY, int cost) {
    int distanceRemaining = (targetX - currentX) + (targetY - currentY);
    addCellToHidePath(frontier.back().directionCameFrom, frontier.back().xPos, frontier.back().yPos);

    if (currentX == targetX && currentY == targetY) {
        neighbours.erase(neighbours.begin(), neighbours.end());
        frontier.erase(frontier.begin(), frontier.end());
        return true;
    } else {
        frontier.pop_back();
        getExplorableNeighbours(currentX, currentY, cost, distanceRemaining);

        for (int counter = neighboursLength - 1; counter >= 0; counter--) {
            addNodesToFrontier();
            if (plotPath(frontier.back().xPos, frontier.back().yPos, targetX, targetY, cost + 1) == true) {
                return true;
            } else {
                hideStack.pop_back();
                pathLength--;
            }
        }
    }

    hideStack.pop_back();
    pathLength--;
    neighbours.erase(neighbours.begin(), neighbours.end());
    neighboursLength = 0;
    frontier.erase(frontier.begin(), frontier.end());
    frontierLength = 0;
    return false;
}

void Occupancy_Grid::getExplorableNeighbours(int xPos, int yPos, int cost, int distanceRemaining) {
    if (grid[xPos + 1][yPos].obstacleValue == 0) addExplorableNeighbour(xPos + 1, yPos, cost, distanceRemaining, NIGHTY);
    if (grid[xPos - 1][yPos].obstacleValue == 0) addExplorableNeighbour(xPos - 1, yPos, cost, distanceRemaining, MINUS_NIGHTY);
    if (grid[xPos][yPos + 1].obstacleValue == 0) addExplorableNeighbour(xPos, yPos + 1, cost, distanceRemaining, ONE_EIGHTY);
    if (grid[xPos][yPos - 1].obstacleValue == 0) addExplorableNeighbour(xPos, yPos - 1, cost, distanceRemaining, ZERO);
}

void Occupancy_Grid::addExplorableNeighbour(int xPos, int yPos, int cost, int distanceRemaining, int direction) {

    int pathValue = (++cost) + distanceRemaining;

    if (neighboursLength > 1) {
        for (int counter = neighboursLength - 1; counter >= 0; counter--) {
            if (pathValue < neighbours.at(counter).pathValue) {
                neighbours.resize(++neighboursLength);
                neighbours.back().xPos = xPos;
                neighbours.back().yPos = yPos;
                neighbours.back().pathValue = pathValue;
                neighbours.back().directionCameFrom = direction;
            } else {
                neighbours.insert(neighbours.begin() + counter, Node());
                neighbours.at(counter).xPos = xPos;
                neighbours.at(counter).yPos = yPos;
                neighbours.at(counter).pathValue = pathValue;
                neighbours.at(counter).directionCameFrom = direction;
            }
        }
    }
}

void Occupancy_Grid::addNodesToFrontier() {
    for (int counter = neighboursLength - 1; counter >= 0; counter--) {
        frontier.resize(++frontierLength);
        frontier.back().xPos = neighbours.back().xPos;
        frontier.back().yPos = neighbours.back().yPos;
        frontier.back().pathValue = neighbours.back().pathValue;
        frontier.back().directionCameFrom = neighbours.back().directionCameFrom;
        neighbours.pop_back();
        neighboursLength--;
    }
}

void Occupancy_Grid::addCellToHidePath(int direction, int xPos, int yPos) {
    hideStack.resize(++pathLength);
    hideStack.back().xPos = xPos;
    hideStack.back().yPos = yPos;
    hideStack.back().directionCameFrom = direction;
}

int Occupancy_Grid::getNextCellDriection() {
    int direction = hideStack.front().directionCameFrom;
    hideStack.erase(hideStack.begin());
    return direction;
}

vector<Node> Occupancy_Grid::getHideStack() {
    return hideStack;
}