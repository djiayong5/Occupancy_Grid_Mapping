/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.h
 * Description: Stores member function prototypes and class definition for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 07/04/2013
 */

#ifndef OCCUPANCY_GRID_H
#define OCCUPANCY_GRID_H

#include <cstdio>
#include <cstdlib>
#include <vector>

#ifndef DIRECTIONS
#define DIRECTIONS
#define ZERO 0
#define FOURTY_FIVE 45
#define NIGHTY 90
#define ONE_THIRTY_FIVE 135
#define ONE_EIGHTY 180
#define MINUS_ONE_THIRTY_FIVE -135
#define MINUS_NIGHTY -90
#define MINUS_FOURTY_FIVE -45
#endif

#define CELL_WIDTH 0.6
#define START_X_LENGTH 3
#define START_Y_LENGTH 3

using namespace std;

struct Cell {
    bool isExplored; //The cell's state of exploration.
    int neighboursUnexplored; //The number of neighbours around the cell unexplored.
    int obstacleValue; //The current obstacle value of the cell.
};

struct Tracker {
    int directionLeftFrom;
    int xPos;
    int yPos;
};

struct Node {
    int xPos;
    int yPos;
    int pathValue;
    int directionLeftFrom;
};

/* Occupancy_Grid class definition */
class Occupancy_Grid {
private:
    vector<Tracker> pathStack;
    vector<vector<Cell> > grid;
    vector<Node> frontier;
    vector<Node> neighbours;
    int yLength;
    int xLength;
    int robotY;
    int robotX;
    int pathLength;
    int frontierLength;
    int neighboursLength;
    int possibleRobotX;
    int possibleRobotY;
    void initialiseCell(Cell *cell);
    void shiftTrackers(int xShift, int yShift);
    void shiftValuesUp();
    void shiftValuesRight();
    void resizeGrid(int directionToExpand);
    void resizeZero();
    void resizeOneEighty();
    void resizeNighty();
    void resizeMinusNighty();
    void checkOccupancy(int gridX, int gridY);
    void incrementCell(int gridX, int gridY);
    void decrementCell(int gridX, int gridY);
    void checkNeighbours(int xPos, int yPos);
    void addCellToPath(int direction, int xPos, int yPos);
    void checkPathExploration();
    int calculateCost(int currentX, int currentY, int newX, int newY, int cost);
    void getExplorableNeighbours(int xPos, int yPos, int cost, int distanceRemaining);
    void addExplorableNeighbour(int xPos, int yPos, int cost, int distanceRemaining, int direction);
    void addNodesToFrontier();
    bool compareCells(Cell cell1, Cell cell2);
    bool compareArea(vector<vector<Cell > > temp, int tempXLength, int tempYLength, int tempRobotX, int tempRobotY, int xStart, int yStart);
    void switchGrid();
    
public:
    /* Member Function Prototypes: */
    Occupancy_Grid();
    vector<Tracker> getPathStack();
    vector<vector<Cell> > getGrid();
    void checkResizeNeeded(int currentDirection);
    void moveRobotOnGrid(int direction);
    void printGrid();
    void calculateCellToChange(int sonarFacing, bool obstaclePresent);
    void mapPath(int direction);
    int getDirectionOfLastCell();
    void setIsExploredTrue();
    int getNeighboursUnexplored();
    void checkCellNeighbours();
    int chooseNextCell(int currentDirection);
    bool checkFinished();
    bool plotPath(int currentX, int currentY, int targetX, int targetY, int cost);
    int attemptLocalisation(vector<vector<Cell > > temp, int tempRobotX, int tempRobotY, int tempXLength, int tempYLength);
};
#endif