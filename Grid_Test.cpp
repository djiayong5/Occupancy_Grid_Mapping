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
#define ZERO 0
#define ONE_EIGHTY 180
#define NIGHTY 90
#define MINUS_NIGHTY 360
#endif

using namespace std;

int main(int argc, char** argv) {
    Occupancy_Grid *oGrid = new Occupancy_Grid();

    cout << ZERO << endl;
    cout << ONE_EIGHTY << endl;
    cout << MINUS_NIGHTY << endl;
    cout << NIGHTY << endl;
    oGrid->printGrid();
    oGrid->gridUpdate(ZERO); /* Increment value of cell 'UP' 1 by 1. */
    oGrid->printGrid();
    oGrid->gridUpdate(MINUS_NIGHTY); /* Increment value of cell 'UP' 1 by 1. */
    oGrid->printGrid();
    oGrid->gridUpdate(ONE_EIGHTY); /* Increment value of cell 'UP' 1 by 1. */
    oGrid->printGrid();
    oGrid->gridUpdate(NIGHTY); /* Increment value of cell 'UP' 1 by 1. */
    oGrid->printGrid();
    cout << "Resizing Up..." << endl;
    oGrid->resizeGrid(ZERO); /* Increase grid height by 1 to 'UP'. */
    oGrid->printGrid();
    cout << "Resizing Right..." << endl;
    oGrid->resizeGrid(MINUS_NIGHTY); /* Increase grid width by 1 to the 'RIGHT'. */
    oGrid->printGrid();
    cout << "Resizing Up..." << endl;
    oGrid->resizeGrid(ZERO); /* Increase grid height by 1 to 'UP'. */
    oGrid->printGrid();
    cout << "Resizing Left..." << endl;
    oGrid->resizeGrid(NIGHTY); /* Increase grid width by 1 to the 'LEFT'. */
    oGrid->printGrid();
    cout << "Resizing Down..." << endl;
    oGrid->resizeGrid(ONE_EIGHTY); /* Increase grid height by 1 to 'DOWN'. */
    oGrid->printGrid();
    return 0;
}
