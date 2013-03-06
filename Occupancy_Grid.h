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
#define RIGHT -90
#endif

#define CELLWIDTH 0.6
#define START_COLUMNS 3
#define START_ROWS 3

using namespace std;

struct Cell {
    double yCoord;
    double xCoord;
    bool isExplored;
    int neighboursUnexplored;
    int obstacleValue;
    int directionToComeFrom;
};

/* Occupancy_Grid class definition */
class Occupancy_Grid {
private:
    vector<Cell*> pathStack;
    vector<vector<Cell> > grid;
    int columnWidth;
    int rowHeight;
    int robotY;
    int robotX;
    void checkOccupancy(int gridY, int gridX);
    void incrementCell(int gridY, int gridX);
    void decrementCell(int gridY, int gridX);

public:
    /* Member Function Prototypes: */
    Occupancy_Grid();
    void initialiseCell(Cell *cell);
    vector<Cell*> getPathStack();
    vector<vector<Cell> > getGrid();
    void resizeGrid(int directionToExpand);
    void shrinkGrid(int directionToShrink);
    void shiftValuesDown();
    void shiftValuesRight();
    void printGrid();
    void mapRobotLocation(int direction);
    void evaluateSonarReading(double sonarReading, int sonarFacing);
    void addCellToPath();
    void removeCellFromPath();
    bool getIsExplored();
    void checkNeighbours();
    int getNeighboursUnexplored();
    int chooseNextCell();
    void setCellDirectionToComeFrom(int direction);
};
#endif