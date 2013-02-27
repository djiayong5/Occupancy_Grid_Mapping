/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.h
 * Description: Stores function prototypes for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 26/02/2013
 */

#ifndef PIONEER_H
#define PIONEER_H

#include <cstdio>
#include <cstdlib>

#ifndef DIRECTIONS
#define DIRECTIONS
#define UP 0
#define DOWN 180
#define LEFT 90
#define RIGHT 360
#endif

#define PGAIN 0.5
#define NEXTSQUARE 0.6


/* Function Prototypes: */

class Pioneer {
private:
    Occupancy_Grid oG;
    int frontSensorFacing; /* Field that describes the direction the front sensor currently faces relative to its original direction 'UP' */
    int rearSensorFacing; /* Field that describes the direction the rear sensor currently faces relative to its original direction 'DOWN' */
    int leftSensorFacing; /* Field that describes the direction the right sensor currently faces relative to its original direction 'LEFT' */
    int rightSensorFacing; /* Field that describes the direction the left sensor currently faces relative to its original direction 'RIGHT' */
public:
    bool atTarget(double currentY, double currentX, double targetY, double targetX);
    double movePioneer(double distance, double currentX, double currentY, double targetX, double targetY);
    double calculateTurnRate(double targetYaw, double currentYaw);
    int evaluateDirection(double currentYaw);
    int getFrontSensorFacing();
    int getRearSensorFacing();
    int getLeftSensorFacing();
    int getRightSensorFacing();
    void setFrontSensorDirection(double currentDirection);
    void setRearSensorDirection(double currentDirection);
    void setLeftSensorDirection(double currentDirection);
    void setRightSensorDirection(double currentDirection);
    void runPioneer();
};
#endif