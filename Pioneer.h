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
#define PGAIN 0.5

/* Function Prototypes: */

class Pioneer {
private:
    int frontSensorFacing; /* Field that describes the direction the front sensor currently faces relative to its original direction 'UP' */
    int rearSensorFacing; /* Field that describes the direction the rear sensor currently faces relative to its original direction 'DOWN' */
    int leftSensorFacing; /* Field that describes the direction the right sensor currently faces relative to its original direction 'LEFT' */
    int rightSensorFacing; /* Field that describes the direction the left sensor currently faces relative to its original direction 'RIGHT' */
public:
    Pioneer();
    int getFrontSensorFacing();
    int getRearSensorFacing();
    int getLeftSensorFacing();
    int getRightSensorFacing();
    void setFrontSensorDirection(double currentYaw);
    void setRearSensorDirection(double currentYaw);
    void setLeftSensorDirection(double currentYaw);
    void setRightSensorDirection(double currentYaw);
    double movePioneer(double distance, double currentX, double currentY, double targetX, double targetY);
};
#endif