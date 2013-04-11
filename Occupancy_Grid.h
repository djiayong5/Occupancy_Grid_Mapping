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
#define TOP 0
#define TOP_LEFT 45
#define LEFT 90
#define BOTTOM_LEFT 135
#define BOTTOM 180
#define BOTTOM_RIGHT -135
#define RIGHT -90
#define TOP_RIGHT -45
#endif

#define CELL_WIDTH 0.7
#define START_X_LENGTH 3
#define START_Y_LENGTH 3

using namespace std;

struct Cell {
    bool isExplored; //The cell's state of exploration.
    int neighboursUnexplored; //The number of neighbours around the cell unexplored.
    int obstacleValue; //The current obstacle value of the cell.
};

struct Tracker {
    int directionCameFrom;
    int xPos;
    int yPos;
};

struct Node {
    int xPos;
    int yPos;
    int pathValue;
    int directionCameFrom;
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
    int anomalyFound;
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
    void addNodesToFrontier();
    bool compareCells(Cell cell1, Cell cell2);
    bool compareArea(Occupancy_Grid *temp, int xCounter, int yCounter);
    bool findLocation(int numberOfWalls);
    int getNumberOfWalls(int xPos, int yPos);
    int calculateCost(int currentX, int currentY, int newX, int newY, int cost);
    void getExplorableNeighbours(int xPos, int yPos, int cost, int distanceRemaining);
    void addExplorableNeighbour(int xPos, int yPos, int cost, int distanceRemaining, int direction);

public:
    /* Member Function Prototypes: */
    Occupancy_Grid();
    void mapConfigure();
    vector<Tracker> getPathStack();
    vector<vector<Cell> > getGrid();
    int getXLength();
    int getYLength();
    int getRobotX();
    int getRobotY();
    bool getAnomalyFound();
    void checkResizeNeeded(int currentDirection);
    void moveRobotOnGrid(int direction);
    void printGrid();
    void calculateCellToChange(int sonarFacing, bool obstaclePresent, bool seekMode);
    int getDirectionOfLastCell();
    void setIsExploredTrue();
    int getNeighboursUnexplored();
    void checkCellNeighbours();
    int chooseNextCell(int currentDirection);
    bool checkFinished();
    int attemptLocalisation(Occupancy_Grid *temp);
    void switchGrid();
    void seekConfigure();
    bool detectAnomaly(int xPos, int yPos);
    void getHideLocation();
    bool plotPath(int currentX, int currentY, int targetX, int targetY, int cost);
    int getNextCellDriection(); 
};
#endif