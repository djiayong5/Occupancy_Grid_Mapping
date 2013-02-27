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

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
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
};
#endif