/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.cpp
 * Description: Stores function declarations for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 26/02/2013
 */

#include <iostream>
#include </src/player-3.0.2/client_libs/libplayerc++/playerc++.h>
#include <cstdio>
#include <cstdlib>
#include "Pioneer.h"
#include "Occupancy_Grid.h"

bool Pioneer::atTarget(double currentY, double currentX, double targetY, double targetX) {
    if (currentY <= (targetY + 0.10) && currentY >= (targetY - 0.10)) {
        if (currentX <= (targetX + 0.10) && currentX >= (targetX - 0.10)) return true;
    } else return false;
}

double movePioneer(double distance, double currentX, double currentY, double targetX, double targetY) {
    double xChange = targetX - currentX;
    double yChange = targetY - currentY;

    return (xChange + yChange) * PGAIN;
}

double calculateTurnRate(double targetYaw, double currentYaw) {
    double turnRate = (targetYaw - currentYaw) * PGAIN;
    return turnRate;
}

int Pioneer::evaluateDirection(double currentYaw) {
    if (currentYaw < 10.00 || currentYaw > 350.00) return UP;
    else if (currentYaw < 280.00 && currentYaw > 260.00) return RIGHT;
    else if (currentYaw < 190 && currentYaw > 170.00) return DOWN;
    else if (currentYaw < 100 & currentYaw > 80.00) return LEFT;
}

int Pioneer::getFrontSensorFacing() {
    return frontSensorFacing;
}

int Pioneer::getRearSensorFacing() {
    return rearSensorFacing;
}

int Pioneer::getLeftSensorFacing() {
    return leftSensorFacing;
}

int Pioneer::getRightSensorFacing() {
    return rightSensorFacing;
}

void Pioneer::setFrontSensorDirection(int currentDirection) {
    if (currentDirection == UP) frontSensorFacing = UP;
    else if (currentDirection == RIGHT) frontSensorFacing = RIGHT;
    else if (currentDirection == DOWN) frontSensorFacing = DOWN;
    else if (currentDirection == LEFT) frontSensorFacing = LEFT;
}

void Pioneer::setRearSensorDirection(double currentDirection) {
    if (currentDirection == UP) rearSensorFacing = DOWN;
    else if (currentDirection == RIGHT) rearSensorFacing = LEFT;
    else if (currentDirection == DOWN) rearSensorFacing = UP;
    else if (currentDirection == LEFT) rearSensorFacing = RIGHT;
}

void Pioneer::setLeftSensorDirection(double currentDirection) {
    if (currentDirection == UP) leftSensorFacing = LEFT;
    else if (currentDirection == RIGHT) leftSensorFacing = UP;
    else if (currentDirection == DOWN) leftSensorFacing = RIGHT;
    else if (currentDirection == LEFT) leftSensorFacing = DOWN;
}

void Pioneer::setRightSensorDirection(double currentDirection) {
    if (currentDirection == UP) rightSensorFacing = RIGHT;
    else if (currentDirection == RIGHT) rightSensorFacing = DOWN;
    else if (currentDirection == DOWN) rightSensorFacing = LEFT;
    else if (currentDirection == LEFT) rightSensorFacing = UP;
}

void Pioneer::runPioneer() {
    using namespace PlayerCc;
    using namespace Pioneer;
    using namespace std;

    PlayerClient robot("lisa.islnet");
    SonarProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);
    Occupancy_Grid oG = new Occupancy_Grid();
    double currentY, currentX, targetY, targetX; /* Fields to robot's current and target, x/y coordinates. */

    pp.SetMotorEnable(true);

    for (;;) {
        double turnRate = 0.00;
        double speed;
        int currentDirection;

        /* Read from proxies. */
        robot.Read();

        /* Get robot's current x and y coordinates. */
        currentY = pp.GetYPos();
        currentX = pp.GetXPos();
        currentDirection = Pioneer::evaluateDirection(pp.GetYaw());
        Pioneer::setFrontSensorDirection(currentDirection);
        Pioneer::setRearSensorDirection(currentDirection);
        Pioneer::setLeftSensorDirection(currentDirection);
        Pioneer::setRightSensorDirection(currentDirection);

        /* Determine if at target location. */
        if (Pioneer::atTarget(currentY, currentX, targetY, targetX) == true) {
            oG.mapRobotLocation(currentDirection);
            oG.resizeGrid(currentDirection);
            oG.gridUpdate(currentDirection);
            oG.evaluateSonarReading(((sp[3] + sp[4]) / 2), frontSensorFacing);
            oG.evaluateSonarReading(((sp[12] + sp[11]) / 2), rearSensorFacing);
            oG.evaluateSonarReading(sp[0], leftSensorFacing);
            oG.evaluateSonarReading(sp[7], rightSensorFacing);
            
            //if ()
        }
        //Print out sonar readings
        cout << sp << endl;

        //Do simple collision avoidance
        if ((sp[0] + sp[1]) < (sp[6] + sp[7]))
            turnRate = dtor(-20); //Turn 20 degrees per delta time
        else
            turnRate = dtor(20);

        if (((sp[3] + sp[4]) / 2) < 0.600)
            speed = 0.000;
        else
            speed = 0.050;
        //Command the motors
        pp.SetSpeed(speed, turnRate);
    }
}

int main(int argc, char *argv[]) {
    Pioneer pioneer = new Pioneer();
    pioneer.runPioneer();
    return 0;
}
