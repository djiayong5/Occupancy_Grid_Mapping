/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.h
 * Description: Stores member function prototypes and class definition for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 26/02/2013
 */

#ifndef OCCUPANCY_GRID_H
#define OCCUPANCY_GRID_H

#include <cstdio>
#include <cstdlib>
#include <vector>

#ifndef DIRECTIONS
#define DIRECTIONS
#define UP 0
#define DOWN 180
#define LEFT 90
#define RIGHT 360
#endif

#define START_COLUMNS 3
#define START_ROWS 3

using namespace std;

struct Cell {
    double yCoord;
    double xCoord;
    bool isExplored;
    int neighboursUnexplored;
    int obstacleValue;
};

/* Occupancy_Grid class definition */
class Occupancy_Grid {
private:
    vector<Cell*> pathStack;
    vector<vector<Cell> > grid;
    int columnWidth;
    int rowHeight;
    int robotX;
    int robotY;
    void checkOccupancy(int gridY, int gridX);
    void incrementCellUp();
    void incrementCellDown();
    void incrementCellLeft();
    void incrementCellRight();
    void decrementCellUp();
    void decrementCellDown();
    void decrementCellLeft();
    void decrementCellRight();

public:
    /* Member Function Prototypes: */
    Occupancy_Grid();
    void initialiseCell(Cell *cell);
    vector<Cell*> getPathStack();
    vector<vector<Cell> > getGrid();
    void resizeGrid(int directionToExpand);
    void shiftValuesDown();
    void shiftValuesRight();
    void printGrid();
    void mapRobotLocation(int direction);
    void evaluateSonarReading(double sonarReading, int sonarFacing);
    void addCellToPath(double yCoord, double xCoord);
    void removeCellFromPath();
    void checkNeighbours();
    int chooseNextCell();
    int getPreviousCellDirection(double currentY, double currentX);
};
#endif