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

/* Occupancy_Grid class definition */
class Occupancy_Grid {
private:
    vector<vector<int> > grid;
    int columnWidth;
    int rowHeight;
    int robotX;
    int robotY;
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
    vector<vector<int> > getGrid();
    void resizeGrid(int directionToExpand);
    void shiftValuesDown();
    void shiftValuesRight();
    void printGrid();
    void gridUpdate(int direction);
    void mapRobotLocation(int direction);
    void evaluateSonarReading(double sonarReading, int sonarFacing);
};
#endif