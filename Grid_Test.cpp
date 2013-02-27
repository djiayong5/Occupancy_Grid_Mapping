/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.cpp
 * Description: Test class to test Occupancy_Grid and Pioneer classes in isolation.
 * First Created: 26/02/2013
 * Last Modified: 27/02/2013
 */

#include "Occupancy_Grid.h"
#include "Pioneer.h"

int main(int argc, char** argv) {
    Occupancy_Grid *oGrid = new Occupancy_Grid();

    oGrid->printGrid();
    return 0;
}
