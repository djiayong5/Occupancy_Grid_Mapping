/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.h
 * Description: Stores member function prototypes and class definition for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 04/04/2013
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

/* Occupancy_Grid class definition */
class Occupancy_Grid {
private:
    vector<int> pathStack;
    vector<vector<Cell> > grid;
    int yLength;
    int xLength;
    int robotY;
    int robotX;
    void resizeGrid(int directionToExpand);
    void resizeZero();
    void resizeOneEighty();
    void resizeNighty();
    void resizeMinusNighty();
    void checkOccupancy(int gridX, int gridY);

public:
    /* Member Function Prototypes: */
    Occupancy_Grid();
    void initialiseCell(Cell *cell);
    vector<int> getPathStack();
    vector<vector<Cell> > getGrid();
    void checkResizeNeeded(int currentDirection);
    void shrinkGrid(int directionToShrink);
    void shiftValuesUp();
    void shiftValuesRight();
    void moveRobotOnGrid(int direction);
    void printGrid();
    void incrementCell(int gridX, int gridY);
    void decrementCell(int gridX, int gridY);
    void calculateCellToChange(int sonarFacing, bool obstaclePresent);
    void addCellToPath(int direction);
    int getDirectionOfLastCell();
    void setIsExploredTrue();
    void checkNeighbours();
    int getNeighboursUnexplored();
    int chooseNextCell(int currentDirection);
    bool checkFinished();
};
#endif