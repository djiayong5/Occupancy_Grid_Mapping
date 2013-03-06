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
    double turnRate = (targetYaw - currentYaw) * TURN_PGAIN;
    return dtor(turnRate);
}

/*void Pioneer::turnToNewDirection(double targetYaw, Position2dProxy *pp, PlayerClient *robot) {
    bool yawAchieved = false;
    double currentYaw;
    double turnRate = 0.00;

    while (yawAchieved != true) {
 *robot->Read();
        currentYaw = rtod(*pp->GetYaw());

        if (currentYaw >= (targetYaw - TURNERRORBOUND) && currentYaw <= (targetYaw + TURNERRORBOUND)) {
            turnRate = 0.000;
            yawAchieved = true;
        } else turnRate = calculateTurnRate(currentYaw, targetYaw);

 *pp->SetSpeed(0.000, turnRate);
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

        if ((distance <= (CELLWIDTH + MOVEERRORBOUND)) && (distance >= (CELLWIDTH - MOVEERRORBOUND))) {
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
}*/

int Pioneer::evaluateDirection(double currentYaw) {
    if (currentYaw <= -80 && currentYaw >= -100.00) return MINUS_NIGHTY;   
    else if (currentYaw <= 100.00 && currentYaw >= 80.00) return NIGHTY;
    else if ((currentYaw <= -170 && currentYaw >= -179.99) || (currentYaw >= 170.00 && currentYaw <= 180.00)) return ONE_EIGHTY;
    if ((currentYaw <= 10.00 && currentYaw >= 0.00) || (currentYaw >= -10.00 && currentYaw <= 0.00)) return ZERO;
}

void Pioneer::setFrontSensorDirection(int currentDirection) {
    if (currentDirection == ZERO) frontSensorFacing = ZERO;
    else if (currentDirection == MINUS_NIGHTY) frontSensorFacing = MINUS_NIGHTY;
    else if (currentDirection == ONE_EIGHTY) frontSensorFacing = ONE_EIGHTY;
    else if (currentDirection == NIGHTY) frontSensorFacing = NIGHTY;
}

void Pioneer::setRearSensorDirection(int currentDirection) {
    if (currentDirection == ZERO) rearSensorFacing = ONE_EIGHTY;
    else if (currentDirection == MINUS_NIGHTY) rearSensorFacing = NIGHTY;
    else if (currentDirection == ONE_EIGHTY) rearSensorFacing = ZERO;
    else if (currentDirection == NIGHTY) rearSensorFacing = MINUS_NIGHTY;
}

void Pioneer::setLeftSensorDirection(int currentDirection) {
    if (currentDirection == ZERO) leftSensorFacing = NIGHTY;
    else if (currentDirection == MINUS_NIGHTY) leftSensorFacing = ZERO;
    else if (currentDirection == ONE_EIGHTY) leftSensorFacing = MINUS_NIGHTY;
    else if (currentDirection == NIGHTY) leftSensorFacing = ONE_EIGHTY;
}

void Pioneer::setRightSensorDirection(int currentDirection) {
    if (currentDirection == ZERO) rightSensorFacing = MINUS_NIGHTY;
    else if (currentDirection == MINUS_NIGHTY) rightSensorFacing = ONE_EIGHTY;
    else if (currentDirection == ONE_EIGHTY) rightSensorFacing = NIGHTY;
    else if (currentDirection == NIGHTY) rightSensorFacing = ZERO;
}

void Pioneer::reconfigureSensors(int currentDirection) {
    setFrontSensorDirection(currentDirection);
    setRearSensorDirection(currentDirection);
    setLeftSensorDirection(currentDirection);
    setRightSensorDirection(currentDirection);
}

void Pioneer::surveyCycle(double frontReading, double rearReading, double leftReading, double rightReading, int currentDirection) {
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

    //turnToNewDirection(0.000, &pp, &robot);
    targetYaw = 0.000;
    bool yawAchieved = false;
    double turnRate = 0.00;

    while (yawAchieved != true) {
        robot.Read();
        currentYaw = rtod(pp.GetYaw());

        if (currentYaw >= (targetYaw - TURN_ERROR_BOUND) && currentYaw <= (targetYaw + TURN_ERROR_BOUND)) {
            turnRate = 0.000;
            yawAchieved = true;
        } else turnRate = calculateTurnRate(currentYaw, targetYaw);

        pp.SetSpeed(0.000, turnRate);
    }

    cout << "Start Yaw Corrected to: " << currentYaw << endl;
    currentDirection = evaluateDirection(currentYaw);
    targetDirection = currentDirection;
    reconfigureSensors(currentDirection);
    oG->shrinkGrid(currentDirection);

    do {
        robot.Read();
        currentYaw = rtod(pp.GetYaw());

        currentDirection = evaluateDirection(currentYaw);
        cout << "Current Direction: " << currentDirection << endl;
        reconfigureSensors(currentDirection);
        surveyCycle(((sp[3] + sp[4]) / 2), ((sp[12] + sp[11]) / 2), sp[0], sp[7], currentDirection);
        oG->printGrid();

        if (oG->getIsExplored() == false) {
            cout << "Current Cell not Explored, Adding to Path Stack." << endl;
            oG->addCellToPath();
            targetDirection = oG->chooseNextCell();
            oG->setCellDirectionToComeFrom(targetDirection);
        }
        else if (oG->getNeighboursUnexplored() != 0) {
            cout << "Picking a Neighbour to Explore..." << endl;
            targetDirection = oG->chooseNextCell();
            oG->setCellDirectionToComeFrom(targetDirection);
        } else if (oG->getNeighboursUnexplored() == 0) {
            cout << "All Neighbours Explored." << endl;
            targetDirection = oG->getPathStack().back()->cameFrom;
            oG->removeCellFromPath();
            cout << "Removed Current Cell from Path." << endl;  
        }


        targetYaw = targetDirection;

        if (targetDirection != currentDirection) {
            //turnToNewDirection(targetYaw, &pp, &robot);
            cout << "Turning to: " << "..." << targetYaw << endl;
            bool yawAchieved = false;
            double currentYaw;
            double turnRate = 0.00;

            while (yawAchieved != true) {
                robot.Read();
                currentYaw = rtod(pp.GetYaw());

                if (currentYaw >= (targetYaw - TURN_ERROR_BOUND) && currentYaw <= (targetYaw + TURN_ERROR_BOUND)) {
                    turnRate = 0.000;
                    yawAchieved = true;
                } else turnRate = calculateTurnRate(currentYaw, targetYaw);

                pp.SetSpeed(0.000, turnRate);
            }
            cout << "Turn Complete, Now Facing: " << currentYaw << endl;
        }

        //moveToNextCell(&pp);
        cout << "Now Moving to Next Cell..." << endl;
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

            if ((distance <= (CELL_WIDTH + MOVE_ERROR_BOUND)) && (distance >= (CELL_WIDTH - MOVE_ERROR_BOUND))) {
                speed = 0.000;
                lastSpeed = 0.000;
                distance = 0.000;
                travelledDistance = true;
                cout << "Arrived at Next Cell." << endl;
            } else {
                lastSpeed = speed;
                distance += lastSpeed * timeDifference; //Speed in m/sec, time in sec.
                speed = CELL_WIDTH - distance * MOVE_PGAIN;
            }

            pp.SetSpeed(speed, 0.000);
        }
    }    while (!oG->getPathStack().empty());
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