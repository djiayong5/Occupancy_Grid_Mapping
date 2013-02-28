/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Grid_Test.cpp
 * Description: Test class to test Occupancy_Grid and Pioneer classes in isolation.
 * First Created: 26/02/2013
 * Last Modified: 27/02/2013
 */

#include "Occupancy_Grid.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

#ifndef DIRECTIONS
#define DIRECTIONS
#define UP 0
#define DOWN 180
#define LEFT 90
#define RIGHT 360
#endif

using namespace std;

int main(int argc, char** argv) {
    Occupancy_Grid *oGrid = new Occupancy_Grid();

    cout << UP << endl;
    cout << DOWN << endl;
    cout << RIGHT << endl;
    cout << LEFT << endl;
    oGrid->printGrid();
    oGrid->gridUpdate(UP); /* Increment value of cell 'UP' 1 by 1. */
    oGrid->printGrid();
    oGrid->gridUpdate(RIGHT); /* Increment value of cell 'UP' 1 by 1. */
    oGrid->printGrid();
    oGrid->gridUpdate(DOWN); /* Increment value of cell 'UP' 1 by 1. */
    oGrid->printGrid();
    oGrid->gridUpdate(LEFT); /* Increment value of cell 'UP' 1 by 1. */
    oGrid->printGrid();
    cout << "Resizing Up..." << endl;
    oGrid->resizeGrid(UP); /* Increase grid height by 1 to 'UP'. */
    oGrid->printGrid();
    cout << "Resizing Right..." << endl;
    oGrid->resizeGrid(RIGHT); /* Increase grid width by 1 to the 'RIGHT'. */
    oGrid->printGrid();
    cout << "Resizing Up..." << endl;
    oGrid->resizeGrid(UP); /* Increase grid height by 1 to 'UP'. */
    oGrid->printGrid();
    cout << "Resizing Left..." << endl;
    oGrid->resizeGrid(LEFT); /* Increase grid width by 1 to the 'LEFT'. */
    oGrid->printGrid();
    cout << "Resizing Down..." << endl;
    oGrid->resizeGrid(DOWN); /* Increase grid height by 1 to 'DOWN'. */
    oGrid->printGrid();
    return 0;
}
