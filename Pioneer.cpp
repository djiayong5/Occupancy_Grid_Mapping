/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.cpp
 * Description: Stores function declarations for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 26/02/2013
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include "Pioneer.h"
#include "Occupancy_Grid.h"

bool Pioneer::atTarget(double currentY, double currentX, double targetY, double targetX) {
    if (currentY <= (targetY + 0.10) && currentY >= (targetY - 0.10)) {
        if (currentX <= (targetX + 0.10) && currentX >= (targetX - 0.10)) return true;
    } else return false;
}

double calculateTurnRate(double currentYaw, double targetYaw) {
    double turnRate = (targetYaw - currentYaw) * PGAIN;
    return dtor(turnRate);
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

double Pioneer::newDirection(double currentYaw) {
    if ((rand() % (2 - 1) + 1) == 1) return currentYaw + 90;
    else return currentYaw - 90;

}

double Pioneer::getNextYCoordinates(double currentY, int direction) {
    if (direction == UP) return currentY + 0.6;
    else if (direction == DOWN) return currentY - 0.6;
    else return currentY;
}

double Pioneer::getNextXCoordinates(double currentX, int direction) {
    if (direction == RIGHT) return currentX + 0.6;
    else if (direction == LEFT) return currentX - 0.6;
    else return currentX;
}

double Pioneer::nextCell(double currentY, double currentX, double targetY, double targetX) {
    double yChange = targetY - currentY;
    double xChange = targetX - currentX;

    xChange = sqrt(xChange * xChange);
    yChange = sqrt(yChange * yChange);
    return xChange + yChange * PGAIN;
}

void Pioneer::reconfigureSensors(double currentDirection) {
    setFrontSensorDirection(currentDirection);
    setRearSensorDirection(currentDirection);
    setLeftSensorDirection(currentDirection);
    setRightSensorDirection(currentDirection);
}

void Pioneer::surveyCycle(RangerProxy sp, double currentDirection) {
    oG.mapRobotLocation(currentDirection);
    oG.resizeGrid(currentDirection);
    oG.gridUpdate(currentDirection);
    oG.evaluateSonarReading(((sp[3] + sp[4]) / 2), frontSensorFacing);
    oG.evaluateSonarReading(((sp[12] + sp[11]) / 2), rearSensorFacing);
    oG.evaluateSonarReading(sp[0], leftSensorFacing);
    oG.evaluateSonarReading(sp[7], rightSensorFacing);
}

void Pioneer::runPioneer() {
    using namespace PlayerCc;
    using namespace Pioneer;
    using namespace std;

    PlayerClient robot("lisa.islnet");
    RangerProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);
    Occupancy_Grid oG = new Occupancy_Grid();
    double currentY = 0.000;
    double currentX = 0.000;
    double targetY = 0.000;
    double targetX = 0.000;
    double currentYaw = 0.000;
    double targetYaw;
    double turnRate = 0.000;
    double speed;

    pp.SetMotorEnable(true);

    for (;;) {
        currentYaw = pp.GetYaw();
        currentY = pp.GetYPos();
        currentX = pp.GetXPos();

        if (turnRate != 0.000) {
            int currentDirection;

            /* Read from proxies. */
            robot.Read();
            currentDirection = evaluateDirection(currentYaw);
            reconfigureSensors(currentDirection);

            /* Determine if at target location. */
            if (atTarget(currentY, currentX, targetY, targetX) == true) {
                surveyCycle(sp, currentDirection);
            }

            /* Decide new target location based on if there is an obstacle dead in front of the Pioneer. */
            if (((sp[3] + sp[4]) / 2) < 0.300) {
                speed = 0.000;
                targetYaw = newDirection(currentYaw); /* Random choice to turn anti-clockwise or clockwise on the spot to avoid collision. */
                turnRate = calculateTurnRate(currentYaw, targetYaw);
            } else {
                double targetY = getNextYCoordinates(currentY, currentDirection);
                double targetX = getNextXCoordinates(currentX, currentDirection);
                speed = nextCell(currentY, currentX, targetY, targetX);
                turnRate = 0.000;
            }
        } else {
            speed = 0.000;
            turnRate = calculateTurnRate(currentYaw, targetYaw); /* Random choice to turn anti-clockwise or clockwise on the spot to avoid collision. */
        }

        //Command the motors
        pp.SetSpeed(speed, turnRate);
    }
}

int main(int argc, char *argv[]) {
    Pioneer pioneer = new Pioneer();
    pioneer.runPioneer();
    return 0;
}
