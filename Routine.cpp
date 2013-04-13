/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Routine.cpp
 * Description: Simple file to build Pioneer's routine.
 * First Created: 28/02/2013
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

void Pioneer::runPioneer() {
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
    int currentDirection;

    pp.SetMotorEnable(true);

    do {
        currentYaw = pp.GetYaw();
        currentY = pp.GetYPos();
        currentX = pp.GetXPos();



        if (turnRate == 0.000) {
            if (atTarget(currentY, currentX, targetY, targetX)) { /* Determine if at target location. */
                robot.Read();
                currentDirection = evaluateDirection(currentYaw);
                reconfigureSensors(currentDirection);
                surveyCycle(sp, currentDirection);
                
                if (oG.getGrid()[currentY][currentX].isExplored == false) {
                    oG.addCellToPath(currentY, currentX);
                }
            }

            /* Decide new target location based on if there is an obstacle dead in front of the Pioneer. */
            if (((sp[3] + sp[4]) / 2) < 0.300) {
                speed = 0.000;
                targetYaw = newDirection(currentYaw); /* Random choice to turn anti-clockwise or clockwise on the spot to avoid collision. */
                turnRate = calculateTurnRate(currentYaw, targetYaw);
            } else {
                double targetY = oG.getPathStack().front().xCoord;
                double targetX = oG.getPathStack().front().yCoord;
                speed = calculateSpeed(currentY, currentX, targetY, targetX);
                turnRate = 0.000;
            }
        } else {
            speed = 0.000;
            turnRate = calculateTurnRate(currentYaw, targetYaw); /* Random choice to turn anti-clockwise or clockwise on the spot to avoid collision. */
        }

        //Command the motors
        pp.SetSpeed(speed, turnRate);
    } while (!oG.getPathStack().empty());
}

int main(int argc, char *argv[]) {
    Pioneer pioneer = new Pioneer();
    pioneer.runPioneer();
    return 0;
}

