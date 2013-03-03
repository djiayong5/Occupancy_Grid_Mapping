/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.cpp
 * Description: Stores function declarations for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 03/03/2013
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include "Pioneer.h"
#include "Occupancy_Grid.h"

using namespace PlayerCc;
using namespace std;

bool Pioneer::atTarget(double currentY, double currentX, double targetY, double targetX) {
    if (currentY >= (targetY - 0.10) && currentY <= (targetY + 0.10)) {
        if (currentX >= (targetX - 0.10) && currentX <= (targetX + 0.10)) return true;
        else return false;
    } else return false;
}

double Pioneer::calculateTurnRate(double currentYaw, double targetYaw) {
    double turnRate = (targetYaw - currentYaw) * PGAIN;
    return dtor(turnRate);
}

int Pioneer::evaluateDirection(double currentYaw) {
    if (currentYaw <= 10.00 || currentYaw >= -10.00) return UP;
    else if (currentYaw <= -80 || currentYaw >= -100.00) return RIGHT;
    else if (currentYaw <= -170 || currentYaw >= 170.00) return DOWN;
    else if (currentYaw < 100.00 || currentYaw > 80.00) return LEFT;
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

double Pioneer::nextCell(int direction, double currentY, double currentX, double targetY, double targetX) {
    if (direction == UP || direction == DOWN) return (targetY - currentY) * PGAIN;
    if (direction == LEFT || direction == RIGHT) return (targetX - currentX) * PGAIN;
}

void Pioneer::reconfigureSensors(int currentDirection) {
    setFrontSensorDirection(currentDirection);
    setRearSensorDirection(currentDirection);
    setLeftSensorDirection(currentDirection);
    setRightSensorDirection(currentDirection);
}

void Pioneer::surveyCycle(double frontReading, double rearReading, double leftReading, double rightReading, int currentDirection) {
    oG->mapRobotLocation(currentDirection);
    oG->resizeGrid(currentDirection);
    oG->evaluateSonarReading(frontReading, frontSensorFacing);
    oG->evaluateSonarReading(rearReading, rearSensorFacing);
    oG->evaluateSonarReading(leftReading, leftSensorFacing);
    oG->evaluateSonarReading(rightReading, rightSensorFacing);
}

void Pioneer::runPioneer() {
    PlayerClient robot("localhost");
    //PlayerClient robot("lisa.islnet");
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

    oG->printGrid(); /* Print out initial grid. */
    pp.SetMotorEnable(true); /* Enable motors. */
    robot.Read(); /* Read from proxies. */
    currentYaw = rtod(pp.GetYaw()); /* Retrieve current yaw. */
    currentY = sqrt(pp.GetYPos() * pp.GetYPos()); /* Retrieve current y position. */
    currentX = sqrt(pp.GetXPos() * pp.GetYPos()); /* Retrieve current x position. */
    targetY = currentY; /* Initialise targetY to match currentY. */
    targetX = currentX; /* Initialise targetX to match currentX. */
    currentDirection = evaluateDirection(currentYaw);
    reconfigureSensors(currentDirection);
    oG->shrinkGrid(currentDirection);

    do {
        cout << endl << endl;
        robot.Read();
        currentYaw = rtod(pp.GetYaw());
        cout << "Current Yaw = " << currentYaw << endl;
        currentY = sqrt(pp.GetYPos() * pp.GetYPos()); /* Retrieve current y position. */
        currentX = sqrt(pp.GetXPos() * pp.GetYPos()); /* Retrieve current x position. */
        cout << "Current Y Pos = " << currentY << endl;
        cout << "Target Y Pos = " << targetY << endl;
        cout << "Current X Pos = " << currentX << endl;
        cout << "Target X Pos = " << targetX << endl;

        if (turnRate == dtor(0.000)) {
            if (atTarget(currentY, currentX, targetY, targetX) == true) { /* Determine if at target location. */
                cout << "At Target Cell." << endl;
                speed = 0.000;
                turnRate = dtor(0.000);
                pp.SetSpeed(speed, turnRate);
                currentDirection = evaluateDirection(currentYaw);
                reconfigureSensors(currentDirection);
                surveyCycle(((sp[3] + sp[4]) / 2), ((sp[12] + sp[11]) / 2), sp[0], sp[7], currentDirection);
                oG->printGrid();

                if (oG->getIsExplored() == false) {
                    cout << "Current Cell not Explored, Adding to Path Stack." << endl;
                    oG->addCellToPath(currentY, currentX);
                    targetDirection = oG->chooseNextCell();
                    oG->setCoordinatesOfCell(currentY, currentX, targetDirection, &targetY, &targetX);
                    cout << "Set Coordinates for Next Cell." << endl;
                } else if (oG->getNeighboursUnexplored() == 0) {
                    cout << "No Neighbours Unexplored." << endl;
                    oG->removeCellFromPath();
                    cout << "Removed Current Cell from Path." << endl;
                    targetDirection = oG->getPathStack().back()->directionCameFrom;
                } else if (oG->getNeighboursUnexplored() != 0) {
                    cout << "Not all Neighbours Explored." << endl;
                    targetDirection = oG->chooseNextCell();
                    oG->setCoordinatesOfCell(currentY, currentX, targetDirection, &targetY, &targetX);
                    cout << "Set Coordinates for Next Cell." << endl;
                }

                cout << "New Direction = " << targetDirection << endl;
                targetYaw = targetDirection;
                cout << "Target Yaw = " << targetYaw << endl;
                turnRate = calculateTurnRate(currentYaw, targetYaw);
            } else {
                speed = nextCell(currentDirection, currentY, currentX, targetY, targetX);
                turnRate = dtor(0.000);
            }
        } else {
            speed = 0.000;

            if ((dtor(currentYaw) >= (dtor(targetYaw) - dtor(1.0))) && (dtor(currentYaw) <= (dtor(targetYaw) + dtor(1.0)))) {
                cout << "New Yaw Achieved." << endl;
                turnRate = dtor(0.000);
            } else {
                targetYaw = targetDirection;
                turnRate = calculateTurnRate(currentYaw, targetYaw); /* Random choice to turn anti-clockwise or clockwise on the spot to avoid collision. */
            }
        }

        cout << "Target Yaw = " << targetYaw << endl;
        //Command the motors
        cout << "Speed = " << speed << endl;
        cout << "Turn Rate = " << turnRate << endl;
        pp.SetSpeed(speed, turnRate);
    } while (!oG->getPathStack().empty());
}

Pioneer::~Pioneer() {
    delete(oG);
}

int main(int argc, char *argv[]) {
    Pioneer *pioneer = new Pioneer();
    pioneer->runPioneer();
    delete(pioneer);
    return 0;
}
