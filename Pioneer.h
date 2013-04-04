/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.h
 * Description: Stores function prototypes for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 04/04/2013
 */

#ifndef PIONEER_H
#define PIONEER_H

#include <iostream>
#include <libplayerc++/playerc++.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "Occupancy_Grid.h"

#ifndef DIRECTIONS
#define DIRECTIONS
#define ZERO 0
#define ONE_EIGHTY 180
#define NIGHTY 90
#define MINUS_NIGHTY -90
#endif

#define MOVE_PGAIN 0.8
#define TURN_PGAIN 0.8
#define MOVE_ERROR_BOUND 0.025
#define TURN_ERROR_BOUND 0.05
#define SONAR_3_4_12_11_RANGE 0.710
#define SONAR_0_15_7_8_RANGE 0.750
#define SONAR_2_13_5_10_RANGE 0.600
#define SONAR_1_6_9_14_RANGE 0.200

using namespace PlayerCc;

class Pioneer {
private:
    Occupancy_Grid *oG;
    int frontSensorFacing; /* Field that describes the direction the front sensor currently faces relative to its original direction 'UP' */
    int rearSensorFacing; /* Field that describes the direction the rear sensor currently faces relative to its original direction 'DOWN' */
    int leftSensorFacing; /* Field that describes the direction the right sensor currently faces relative to its original direction 'LEFT' */
    int rightSensorFacing; /* Field that describes the direction the left sensor currently faces relative to its original direction 'RIGHT' */
    double calculateTurnRate(double currentYaw, double targetYaw);
    void turnToNewDirection(double targetYaw, Position2dProxy *pp, PlayerClient *robot);
    void moveForward(Position2dProxy *pp);
    int evaluateDirection(double currentYaw);
    void setFrontSensorDirection(int currentDirection);
    void setRearSensorDirection(int currentDirection);
    void setLeftSensorDirection(int currentDirection);
    void setRightSensorDirection(int currentDirection);
    void reconfigureSensors(int currentDirection);
    void surveyCycle(double readings[], int currentDirection, bool inNextCell);
    void configureCycle(PlayerClient *robot, Position2dProxy *pp, double *currentYaw, int *currentDirection);

public:
    void runPioneer();
    ~Pioneer();
};
#endif