/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.h
 * Description: Stores function prototypes for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 06/04/2013
 */

#ifndef PIONEER_H
#define PIONEER_H

#include <iostream>
#include <libplayerc++/playerc++.h>
#include <cstdio>
#include <cstdlib>
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
#define TURN_ERROR_BOUND 0.050
#define FRONT_REAR_RANGE 0.850
#define LEFT_RIGHT_RANGE 0.950
#define CORNER_RANGE 1.20
#define CLOSE_RANGE 0.300

using namespace PlayerCc;

class Pioneer {
private:
    Occupancy_Grid *oG;
    int frontSensorFacing; /* Field that describes the direction the front sensor currently faces. */
    int rearSensorFacing; /* Field that describes the direction the rear sensor currently faces. */
    int leftSensorFacing; /* Field that describes the direction the right sensor currently faces. */
    int rightSensorFacing; /* Field that describes the direction the left sensor currently faces. */
    int frontLeftSensorFacing; /* Field that describes the direction the front left sensor currently faces. */
    int frontRightSensorFacing; /* Field that describes the direction the front right sensor currently faces. */
    int rearLeftSensorFacing; /* Field that describes the direction the rear left sensor currently faces. */
    int rearRightSensorFacing; /* Field that describes the direction the rear right sensor currently faces. */
    void runProgram();
    void localise();
    void hide();
    double calculateTurnRate(double currentYaw, double targetYaw);
    void turnToNewDirection(double targetYaw, Position2dProxy *pp, PlayerClient *robot);
    void calculateMoveDistance(PlayerClient *robot, Position2dProxy *pp, int direction, double distanceToMove);
    void moveForward(PlayerClient *robot, Position2dProxy *pp, int direction, double posDifference);
    int evaluateDirection(double currentYaw);
    void setFrontSensorDirection(int currentDirection);
    void setRearSensorDirection(int currentDirection);
    void setLeftSensorDirection(int currentDirection);
    void setRightSensorDirection(int currentDirection);
    void setFrontLeftSensorDirection(int currentDirection);
    void setFrontRightSensorDirection(int currentDirection);
    void setRearLeftSensorDirection(int currentDirection);
    void setRearRightSensorDirection(int currentDirection);
    void reconfigureSensors(int currentDirection);
    void surveyCycle(double readings[], int currentDirection, bool inNextCell);
    void evaluateReadings(double reading1, double reading2, double range, int sensorFacing, Occupancy_Grid grid);
    void evaluateCornerReadings(double reading, double lowerBound, double upperBound, int sensorFacing, Occupancy_Grid grid);
    void evaluateMovingReadings(double reading1, double reading2, double range1, double range2, int sensorFacing, Occupancy_Grid grid);
    void evaluateBlindSpots(double reading1, double reading2, double range, int sensorFacing, Occupancy_Grid grid);
    void configureCycle(PlayerClient *robot, Position2dProxy *pp, double *currentYaw, int *currentDirection);
    

public:
    void map();
    ~Pioneer();
};
#endif