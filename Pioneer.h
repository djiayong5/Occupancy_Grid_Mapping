/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.h
 * Description: Stores function prototypes for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 03/03/2013
 */

#ifndef PIONEER_H
#define PIONEER_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Occupancy_Grid.h"

#ifndef DIRECTIONS
#define DIRECTIONS
#define UP 0
#define DOWN 180
#define LEFT 90
#define RIGHT -90
#endif

#define PGAIN 0.5
#define NEXTSQUARE 0.6

class Pioneer {   
private:
    Occupancy_Grid *oG;
    int frontSensorFacing; /* Field that describes the direction the front sensor currently faces relative to its original direction 'UP' */
    int rearSensorFacing; /* Field that describes the direction the rear sensor currently faces relative to its original direction 'DOWN' */
    int leftSensorFacing; /* Field that describes the direction the right sensor currently faces relative to its original direction 'LEFT' */
    int rightSensorFacing; /* Field that describes the direction the left sensor currently faces relative to its original direction 'RIGHT' */
    double calculateTurnRate(double currentYaw, double targetYaw);
    double nextCell(double distanceToGo, double targetDistance);
    double calculateDistanceToGo(int targetDirection, double currentY, double currentX, double targetY, double targetX);
    double getNextYCoordinates(double currentY, int direction);
    double getNextXCoordinates(double currentX, int direction);
    int getFrontSensorFacing();
    int getRearSensorFacing();
    int getLeftSensorFacing();
    int getRightSensorFacing();
    bool atTarget(double currentY, double currentX, double targetY, double targetX);
    int evaluateDirection(double currentYaw);
    void setFrontSensorDirection(int currentDirection);
    void setRearSensorDirection(int currentDirection);
    void setLeftSensorDirection(int currentDirection);
    void setRightSensorDirection(int currentDirection);
    void reconfigureSensors(int currentDirection);
    void surveyCycle(double frontReading, double rearReading, double leftReading, double rightReading, int currentDirection);
    void printNewMoveDetails(double currentY, double currentX, double targetY, double targetX, int currentDirection, int targetDirection);
public:
    void runPioneer();
    ~Pioneer();
};
#endif