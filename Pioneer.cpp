/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.cpp
 * Description: Stores function declarations for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 26/02/2013
 */

#include <cstdio>
#include <cstdlib>
#include "Pioneer.h"
#include "Occupancy_Grid.h"

double movePioneer(double distance, double currentX, double currentY, double targetX, double targetY) {
    double xChange = targetX - currentX;
    double yChange = targetY - currentY;

    return (xChange + yChange) * PGAIN;
}

int Pioneer::getFrontSensorFacing() {
    return frontSensorFacing;
}

int Pioneer::getRearSensorFacing() {
    return rearSensorFacing;
}

int Pioneer:: getLeftSensorFacing() {
    return leftSensorFacing;
}

int Pioneer::getRightSensorFacing() {
    return rightSensorFacing;
}

void Pioneer::setFrontSensorDirection(double currentYaw) {
    if (currentYaw < 10.00 || currentYaw > 350.00) frontSensorFacing = UP;
    else if (currentYaw < 280.00 || currentYaw > 260.00) frontSensorFacing = RIGHT;
    else if (currentYaw < 190 || currentYaw > 170.00) frontSensorFacing = DOWN;
    else if (currentYaw < 100 || currentYaw > 80.00) frontSensorFacing = LEFT;
}

void Pioneer::setRearSensorDirection(double currentYaw) {
    if (currentYaw < 10.00 || currentYaw > 350.00) rearSensorFacing = DOWN;
    else if (currentYaw < 280.00 || currentYaw > 260.00) rearSensorFacing = LEFT;
    else if (currentYaw < 190 || currentYaw > 170.00) rearSensorFacing = UP;
    else if (currentYaw < 100 || currentYaw > 80.00) rearSensorFacing = RIGHT;
}

void Pioneer::setLeftSensorDirection(double currentYaw) {
    if (currentYaw < 10.00 || currentYaw > 350.00) leftSensorFacing = LEFT;
    else if (currentYaw < 280.00 || currentYaw > 260.00) leftSensorFacing = UP;
    else if (currentYaw < 190 || currentYaw > 170.00) leftSensorFacing = RIGHT;
    else if (currentYaw < 100 || currentYaw > 80.00) leftSensorFacing = DOWN;
}

void Pioneer::setRightSensorDirection(double currentYaw) {
    if (currentYaw < 10.00 || currentYaw > 350.00) rightSensorFacing = RIGHT;
    else if (currentYaw < 280.00 || currentYaw > 260.00) rightSensorFacing = DOWN;
    else if (currentYaw < 190 || currentYaw > 170.00) frontSensorFacing = LEFT;
    else if (currentYaw < 100 || currentYaw > 80.00) frontSensorFacing = UP;
}
