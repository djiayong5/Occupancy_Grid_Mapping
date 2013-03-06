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
#include <time.h>
#include "Occupancy_Grid.h"

using namespace PlayerCc;
using namespace std;

double Pioneer::calculateTurnRate(double currentYaw, double targetYaw) {
    double turnRate = (targetYaw - currentYaw) * TURNPGAIN;
    return dtor(turnRate);
}

void Pioneer::turnToNewDirection(double targetYaw, Position2dProxy *pp, PlayerClient *robot) {
    bool yawAchieved = false;
    double currentYaw;
    double turnRate = 0.00;

    while (yawAchieved != true) {
        (void)*robot->Read();
        currentYaw = rtod(*pp->GetYaw());
        
        if (currentYaw >= (targetYaw - ERRORBOUND) && currentYaw <= (targetYaw + ERRORBOUND)) {
            turnRate = 0.000;
            yawAchieved = true;
        }
        else turnRate = calculateTurnRate(currentYaw, targetYaw);

        (void)*pp->SetSpeed(0.000, turnRate);
    }
}

void Pioneer::moveToNextCell(Position2dProxy *pp) {
    time_t currentTime;
    time_t lastTime;
    double timeDifference = 0.000;
    double speed = 0.000;
    double lastSpeed = 0.000;
    double distance = 0.000;
    bool travelledDistance = false;

    while (travelledDistance == false) {
        lastTime = currentTime;
        currentTime = time(NULL);
        timeDifference = difftime(currentTime, lastTime);

        if ((distance <= (CELLWIDTH + ERRORBOUND)) && (distance >= (CELLWIDTH - ERRORBOUND))) {
            speed = 0.000;
            lastSpeed = 0.000;
            distance = 0.000;
            travelledDistance = true;
            cout << "Arrived at next cell." << endl;
        } else {
            lastSpeed = speed;
            distance += lastSpeed * timeDifference; //Speed in m/sec, time in sec.
            speed = CELLWIDTH - distance * MOVEPGAIN;
        }

        *pp->SetSpeed(speed, 0.000);
    }
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
    cout << endl;
}

void Pioneer::runPioneer() {
    PlayerClient robot("localhost");
    //PlayerClient robot("lisa.islnet");
    RangerProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);

    oG = new Occupancy_Grid();
    double currentYaw = 0.000;
    double targetYaw;
    int currentDirection;
    int targetDirection;

    oG->printGrid(); /* Print out initial grid. */
    pp.SetMotorEnable(true); /* Enable motors. */
    robot.Read(); /* Read from proxies. */
    currentYaw = rtod(pp.GetYaw()); /* Retrieve current yaw. */
    cout << "Start Yaw: " << currentYaw << endl;

    turnToNewDirection(0.000, &pp, &robot);

    cout << "Start Yaw Corrected to: " << currentYaw << endl;
    currentDirection = evaluateDirection(currentYaw);
    targetDirection = currentDirection;
    reconfigureSensors(currentDirection);
    oG->shrinkGrid(currentDirection);

    do {
        robot.Read();
        currentYaw = rtod(pp.GetYaw());

        currentDirection = evaluateDirection(currentYaw);
        reconfigureSensors(currentDirection);
        surveyCycle(((sp[3] + sp[4]) / 2), ((sp[12] + sp[11]) / 2), sp[0], sp[7], currentDirection);
        oG->printGrid();

        if (oG->getIsExplored() == false) {
            cout << "Current Cell not Explored, Adding to Path Stack." << endl;
            oG->addCellToPath();
            targetDirection = oG->chooseNextCell();
            oG->setCellDirectionToComeFrom(targetDirection);
            cout << "Set Coordinates for Next Cell." << endl;
        } else if (oG->getNeighboursUnexplored() == 0) {
            cout << "No Neighbours Unexplored." << endl;
            oG->removeCellFromPath();
            cout << "Removed Current Cell from Path." << endl;
            targetDirection = oG->getPathStack().back()->directionToComeFrom;
        } else if (oG->getNeighboursUnexplored() != 0) {
            cout << "Not all Neighbours Explored." << endl;
            targetDirection = oG->chooseNextCell();
            oG->setCellDirectionToComeFrom(targetDirection);
            cout << "Set Coordinates for Next Cell." << endl;
        }

        targetYaw = targetDirection;
        
        if (targetDirection != currentDirection) turnToNewDirection(targetYaw, &pp, &robot);
        moveToNextCell(&pp);
        
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