/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Grid_Test.cpp
 * Description: Test class to test Occupancy_Grid and Pioneer classes in isolation.
 * First Created: 26/02/2013
 * Last Modified: 27/02/2013
 */

#include "Occupancy_Grid.h"
#include "Pioneer.h"

int main(int argc, char** argv) {
    Occupancy_Grid *oGrid = new Occupancy_Grid();

    oGrid->printGrid();
    oGrid->gridUpdate(UP); /* Increment value of cell 'UP' 1 by 1. */
    oGrid->printGrid();
    oGrid->resizeGrid(RIGHT); /* Increase grid width by 1 to the 'RIGHT'. */
    oGrid->printGrid();
    oGrid->resizeGrid(UP);
    oGrid->printGrid();
    oGrid->resizeGrid(LEFT);
    oGrid->printGrid();
    return 0;
}
