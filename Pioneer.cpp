/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.cpp
 * Description: Stores function declarations for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 28/02/2013
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include "Pioneer.h"
#include "Occupancy_Grid.h"

using namespace PlayerCc;
using namespace std;

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

void Pioneer::setRearSensorDirection(int currentDirection) {
    if (currentDirection == UP) rearSensorFacing = DOWN;
    else if (currentDirection == RIGHT) rearSensorFacing = LEFT;
    else if (currentDirection == DOWN) rearSensorFacing = UP;
    else if (currentDirection == LEFT) rearSensorFacing = RIGHT;
}

void Pioneer::setLeftSensorDirection(int currentDirection) {
    if (currentDirection == UP) leftSensorFacing = LEFT;
    else if (currentDirection == RIGHT) leftSensorFacing = UP;
    else if (currentDirection == DOWN) leftSensorFacing = RIGHT;
    else if (currentDirection == LEFT) leftSensorFacing = DOWN;
}

void Pioneer::setRightSensorDirection(int currentDirection) {
    if (currentDirection == UP) rightSensorFacing = RIGHT;
    else if (currentDirection == RIGHT) rightSensorFacing = DOWN;
    else if (currentDirection == DOWN) rightSensorFacing = LEFT;
    else if (currentDirection == LEFT) rightSensorFacing = UP;
}

double Pioneer::nextCell(double currentY, double currentX, double targetY, double targetX) {
    double yChange = targetY - currentY;
    double xChange = targetX - currentX;

    xChange = sqrt(xChange * xChange);
    yChange = sqrt(yChange * yChange);
    return xChange + yChange * PGAIN;
}

void Pioneer::reconfigureSensors(int currentDirection) {
    setFrontSensorDirection(currentDirection);
    setRearSensorDirection(currentDirection);
    setLeftSensorDirection(currentDirection);
    setRightSensorDirection(currentDirection);
}

void Pioneer::surveyCycle(RangerProxy sp, int currentDirection) {
    oG->mapRobotLocation(currentDirection);
    oG->resizeGrid(currentDirection);
    oG->evaluateSonarReading(((sp[3] + sp[4]) / 2), frontSensorFacing);
    oG->evaluateSonarReading(((sp[12] + sp[11]) / 2), rearSensorFacing);
    oG->evaluateSonarReading(sp[0], leftSensorFacing);
    oG->evaluateSonarReading(sp[7], rightSensorFacing);
}

void Pioneer::runPioneer() {
    PlayerClient robot("lisa.islnet");
    RangerProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);
    oG = new Occupancy_Grid();
    double currentY = 0.000;
    double currentX = 0.000;
    double targetY = 0.000;
    double targetX = 0.000;
    double currentYaw = 0.000;
    double targetYaw;
    double turnRate = 0.000;
    double speed;
    int currentDirection;
    int targetDirection;

    pp.SetMotorEnable(true);

    do {
        currentYaw = pp.GetYaw();
        currentY = pp.GetYPos();
        currentX = pp.GetXPos();



        if (turnRate == 0.000) {
            if (atTarget(currentY, currentX, targetY, targetX) == true) { /* Determine if at target location. */
                pp.SetSpeed(0.000, 0.000);
                robot.Read();
                currentDirection = evaluateDirection(currentYaw);
                reconfigureSensors(currentDirection);
                surveyCycle(sp, currentDirection);
                oG->printGrid();

                if (oG->getGrid()[currentY][currentX].isExplored == false) {
                    oG->addCellToPath(currentY, currentX);
                    targetDirection = oG->chooseNextCell();
                } else if (oG->getGrid()[currentY][currentX].neighboursUnexplored == 0) {
                    oG->removeCellFromPath();
                    targetDirection = oG->getPreviousCellDirection(currentY, currentX);
                } else if (oG->getGrid()[currentY][currentX].neighboursUnexplored != 0) {
                    targetDirection = oG->chooseNextCell();
                }

                targetYaw = dtor(targetDirection);
                turnRate = calculateTurnRate(currentYaw, targetYaw);
            } else {
                targetY = oG->getPathStack().back()->yCoord;
                targetX = oG->getPathStack().back()->xCoord;
                speed = nextCell(currentY, currentX, targetY, targetX);
                turnRate = 0.000;
            }
        } else {
            speed = 0.000;
            targetYaw = dtor(targetDirection);
            turnRate = calculateTurnRate(currentYaw, targetYaw); /* Random choice to turn anti-clockwise or clockwise on the spot to avoid collision. */
        }

        //Command the motors
        pp.SetSpeed(speed, turnRate);
    } while (!oG->getPathStack().empty());
}

int main(int argc, char *argv[]) {
    Pioneer *pioneer = new Pioneer();
    pioneer->runPioneer();
    return 0;
}
