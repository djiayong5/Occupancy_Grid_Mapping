/*
 * Copyright (c) 2005, Brad Kratochvil, Toby Collett, Brian Gerkey, Andrew Howard, ...
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *           this list of conditions and the following disclaimer.
 *               * Redistributions in binary form must reproduce the above copyright notice,
 *                     this list of conditions and the following disclaimer in the documentation
 *                           and/or other materials provided with the distribution.
 *                               * Neither the name of the Player Project nor the names of its contributors
 *                                     may be used to endorse or promote products derived from this software
 *                                           without specific prior written permission.
 *
 *                                           THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *                                           ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *                                           WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *                                           DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 *                                           ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *                                           (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *                                           LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *                                           ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *                                           (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *                                           SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *                                           */

#include <iostream>
#include "Occupancy_Grid.h"
#include <libplayerc++/playerc++.h>


#define UP 0
#define DOWN 180
#define LEFT 90
#define RIGHT 360
#define PGAIN 0.6

using namespace PlayerCc;

Occupancy_Grid *oG = new Occupancy_Grid();

void testRun();
bool atTarget(double currentY, double currentX, double targetY, double targetX);
double calculateTurnRate(double currentYaw, double targetYaw);
int evaluateDirection(double currentYaw);
void setFrontSensorDirection(int currentDirection, int frontSensorFacing);
void setRearSensorDirection(int currentDirection, int rearSensorFacing);
void setLeftSensorDirection(int currentDirection, int leftSensorFacing);
void setRightSensorDirection(int currentDirection, int rightSensorFacing);
double nextCell(double currentY, double currentX, double targetY, double targetX);
void reconfigureSensors(int currentDirection, int frontSensorFacing, int rearSensorFacing, int leftSensorFacing, int rightSensorFacing);
//void surveyCycle(RangerProxy sp, int currentDirection, int frontSensorFacing, int rearSensorFacing, int leftSensorFacing, int rightSensorFacing);
void testTwo();

int main(int argc, char *argv[]) {
    testRun();
    return 0;
}


void testRun() {

    PlayerClient robot("localhost");
    RangerProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);

    pp.SetMotorEnable(true);

    for (;;) {
        double turnrate, speed;

        //  read from the proxies
        robot.Read();

        //         print out sonars for fun
        std::cout << sp << std::endl;

        //             do simple collision avoidance
        if ((sp[0] + sp[1]) < (sp[6] + sp[7]))
            turnrate = dtor(-20); // turn 20 degrees per second
        else
            turnrate = dtor(20);

        if (sp[3] < 0.500)
            speed = 0;
        else
            speed = 0.100;

        //                                                           command the motors
        pp.SetSpeed(speed, turnrate);
    }
}

bool atTarget(double currentY, double currentX, double targetY, double targetX) {
    if (currentY <= (targetY + 0.10) && currentY >= (targetY - 0.10)) {
        if (currentX <= (targetX + 0.10) && currentX >= (targetX - 0.10)) return true;
    } else return false;
}

double calculateTurnRate(double currentYaw, double targetYaw) {
    double turnRate = (targetYaw - currentYaw) * PGAIN;
    return dtor(turnRate);
}

int evaluateDirection(double currentYaw) {
    if (currentYaw < 10.00 || currentYaw > 350.00) return UP;
    else if (currentYaw < 280.00 && currentYaw > 260.00) return RIGHT;
    else if (currentYaw < 190 && currentYaw > 170.00) return DOWN;
    else if (currentYaw < 100 & currentYaw > 80.00) return LEFT;
}

/*void surveyCycle(RangerProxy sp, int currentDirection, int frontSensorFacing, int rearSensorFacing, int leftSensorFacing, int rightSensorFacing) {
    oG->mapRobotLocation(currentDirection);
    oG->resizeGrid(currentDirection);
    oG->evaluateSonarReading(((sp[3] + sp[4]) / 2), frontSensorFacing);
    oG->evaluateSonarReading(((sp[12] + sp[11]) / 2), rearSensorFacing);
    oG->evaluateSonarReading(sp[0], leftSensorFacing);
    oG->evaluateSonarReading(sp[7], rightSensorFacing);
}*/

/*
int getFrontSensorFacing() {
    return frontSensorFacing;
}

int getRearSensorFacing() {
    return rearSensorFacing;
}

int getLeftSensorFacing() {
    return leftSensorFacing;
}

int getRightSensorFacing() {
    return rightSensorFacing;
}*/

void setFrontSensorDirection(int currentDirection, int frontSensorFacing) {
    if (currentDirection == UP) frontSensorFacing = UP;
    else if (currentDirection == RIGHT) frontSensorFacing = RIGHT;
    else if (currentDirection == DOWN) frontSensorFacing = DOWN;
    else if (currentDirection == LEFT) frontSensorFacing = LEFT;
}

void setRearSensorDirection(int currentDirection, int rearSensorFacing) {
    if (currentDirection == UP) rearSensorFacing = DOWN;
    else if (currentDirection == RIGHT) rearSensorFacing = LEFT;
    else if (currentDirection == DOWN) rearSensorFacing = UP;
    else if (currentDirection == LEFT) rearSensorFacing = RIGHT;
}

void setLeftSensorDirection(int currentDirection, int leftSensorFacing) {
    if (currentDirection == UP) leftSensorFacing = LEFT;
    else if (currentDirection == RIGHT) leftSensorFacing = UP;
    else if (currentDirection == DOWN) leftSensorFacing = RIGHT;
    else if (currentDirection == LEFT) leftSensorFacing = DOWN;
}

void setRightSensorDirection(int currentDirection, int rightSensorFacing) {
    if (currentDirection == UP) rightSensorFacing = RIGHT;
    else if (currentDirection == RIGHT) rightSensorFacing = DOWN;
    else if (currentDirection == DOWN) rightSensorFacing = LEFT;
    else if (currentDirection == LEFT) rightSensorFacing = UP;
}

double nextCell(double currentY, double currentX, double targetY, double targetX) {
    double yChange = targetY - currentY;
    double xChange = targetX - currentX;

    xChange = sqrt(xChange * xChange);
    yChange = sqrt(yChange * yChange);
    return xChange + yChange * PGAIN;
}

void reconfigureSensors(int currentDirection, int frontSensorFacing, int rearSensorFacing, int leftSensorFacing, int rightSensorFacing) {
    setFrontSensorDirection(currentDirection, frontSensorFacing);
    setRearSensorDirection(currentDirection, rearSensorFacing);
    setLeftSensorDirection(currentDirection, leftSensorFacing);
    setRightSensorDirection(currentDirection, rightSensorFacing);
}

void testTwo() {
    
    PlayerClient robot("localhost");
    RangerProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);
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
    int frontSensorFacing; /* Field that describes the direction the front sensor currently faces relative to its original direction 'UP' */
    int rearSensorFacing; /* Field that describes the direction the rear sensor currently faces relative to its original direction 'DOWN' */
    int leftSensorFacing; /* Field that describes the direction the right sensor currently faces relative to its original direction 'LEFT' */
    int rightSensorFacing;

    pp.SetMotorEnable(true);

    for (;;) {
        currentYaw = pp.GetYaw();
        currentY = pp.GetYPos();
        currentX = pp.GetXPos();



        if (turnRate == 0.000) {
            if (atTarget(currentY, currentX, targetY, targetX) == true) { /* Determine if at target location. */
                pp.SetSpeed(0.000, 0.000);
                robot.Read();
                currentDirection = evaluateDirection(currentYaw);
                reconfigureSensors(currentDirection, frontSensorFacing, rearSensorFacing, leftSensorFacing, rightSensorFacing);
               //surveyCycle(sp, currentDirection, frontSensorFacing, rearSensorFacing, leftSensorFacing, rightSensorFacing);
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
    }
}