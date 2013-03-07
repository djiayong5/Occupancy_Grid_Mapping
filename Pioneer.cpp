/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.cpp
 * Description: Stores function declarations for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 03/03/2013
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
#include "Pioneer.h"
#include "Occupancy_Grid.h"
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <time.h>

using namespace PlayerCc;
using namespace std;

double Pioneer::calculateTurnRate(double currentYaw, double targetYaw) {
    double turnRate = (targetYaw - currentYaw) * TURN_PGAIN;
    return dtor(turnRate);
}

void Pioneer::turnToNewDirection(double targetYaw, Position2dProxy *pp, PlayerClient *robot) {
    bool yawAchieved = false;
    double currentYaw;
    double turnRate = 0.00;

    cout << "Turning to: " << targetYaw << "..." << endl;         
            
    while (yawAchieved != true) {
        robot->Read();
        currentYaw = rtod(pp->GetYaw());

        if (currentYaw >= (targetYaw - TURN_ERROR_BOUND) && currentYaw <= (targetYaw + TURN_ERROR_BOUND)) {
            turnRate = 0.000;
            yawAchieved = true;
        } else turnRate = calculateTurnRate(currentYaw, targetYaw);

        pp->SetSpeed(0.000, turnRate);
    }
    
    cout << "Turn Complete, Now Facing: " << currentYaw << endl;
}

void Pioneer::moveToNextCell(Position2dProxy *pp) {
    time_t currentTime = time(NULL); //Stores the current time.
    time_t lastTime = time(NULL); //Stores the previous current time.
    double timeDifference = 0.000; //Stores the time difference between lastTime and currentTime in seconds.
    double speed = 0.000; //Stores the current speed the robot is travelling at.
    double lastSpeed = 0.000; //Stores the speed the robot was last travelling at.
    double distance = 0.000; //Stores the distance the robot has travelled. 
    bool travelledDistance = false; //Boolean condition to break out of while loop.

    while (travelledDistance == false) {
        lastTime = currentTime; //Sets time to the time stored in currentTime before it updates ready for time difference calculation.
        currentTime = time(NULL); //Sets time to the current time ready for time difference calculation, 
        timeDifference = difftime(currentTime, lastTime); //Calculates time difference in seconds.

        if ((distance <= (CELL_WIDTH + MOVE_ERROR_BOUND)) && (distance >= (CELL_WIDTH - MOVE_ERROR_BOUND))) {
            speed = 0.000;
            lastSpeed = 0.000;
            distance = 0.000;
            travelledDistance = true;
            cout << "Arrived at next cell." << endl;
        } else {
            lastSpeed = speed;
            distance += (lastSpeed * timeDifference); //Speed in m/sec, time in sec.
            speed = ((CELL_WIDTH - distance) * MOVE_PGAIN);
        }

        pp->SetSpeed(speed, 0.000);      
    }
}

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
    oG->resizeGrid(currentDirection); //Expands grid in the direction the robot is currently facing by 1.
    oG->evaluateSonarReading(frontReading, frontSensorFacing);
    oG->evaluateSonarReading(rearReading, rearSensorFacing);
    oG->evaluateSonarReading(leftReading, leftSensorFacing);
    oG->evaluateSonarReading(rightReading, rightSensorFacing);
    cout << endl; //Used for formatting output.
}

void Pioneer::runPioneer() {
    PlayerClient robot("localhost");
    //PlayerClient robot("lisa.islnet");
    RangerProxy sp(&robot, 0);
    //SonarProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);

    oG = new Occupancy_Grid(); //Creates new occupancy grid on the heap;
    double currentYaw = 0.000;
    double targetYaw = 0.000;
    int currentDirection;
    int targetDirection;

    oG->printGrid(); /* Print out initial grid. */
    pp.SetMotorEnable(true); /* Enable motors. */
    robot.Read(); /* Read from proxies. */
    currentYaw = rtod(pp.GetYaw()); /* Retrieve current yaw. */
    cout << "Start Yaw: " << currentYaw << endl;

    turnToNewDirection(0.000, &pp, &robot); //Gets robot to turn to 0 degrees (with error bounds).

    cout << "Start Yaw Corrected to: " << currentYaw << endl;
    currentDirection = evaluateDirection(currentYaw);
    targetDirection = currentDirection;
    reconfigureSensors(currentDirection);
    oG->shrinkGrid(currentDirection); //Shrinks grid in the direction the robot faces as the grid will expand in that same direction once it enters the loop ahead.

    do {
        robot.Read(); //Reads from the proxies.
        currentYaw = rtod(pp.GetYaw()); //Gets the robot's current yaw and converts it from radians to degrees.

        currentDirection = evaluateDirection(currentYaw); //Evaluates the robots direction.
        cout << "Current direction: " << currentDirection << endl;
        reconfigureSensors(currentDirection);
        cout << "Reconfigured sensors." << endl;
        surveyCycle(((sp[3] + sp[4]) / 2), ((sp[12] + sp[11]) / 2), sp[0], sp[7], currentDirection); //Takes the sonar readings and marks cells as appropriate.
        oG->printGrid(); //Prints the occupancy grid.

        if (oG->getIsExplored() == false) {
            cout << "Current cell not explored, adding cell to path stack." << endl;
            oG->addCellToPath(); //Adds current cell to the top of the path stack.
            cout << "Picking next cell to travel to..." << endl;
            targetDirection = oG->chooseNextCell(); //Chooses the next unexplored neighbour cell to travel to.
            oG->setLeavingDirection(targetDirection); //Sets the direction in which the robot will leave the current cell.
        } else if (oG->getNeighboursUnexplored() != 0) {
            cout << "Picking a neighbour to explore..." << endl;
            targetDirection = oG->chooseNextCell(); //Chooses the next unexplored neighbour cell to travel to.
            oG->setLeavingDirection(targetDirection); //Sets the direction in which the robot will leave the current cell.
        } else if (oG->getNeighboursUnexplored() == 0) {
            cout << "All neighbours of current cell explored." << endl;
            oG->removeCellFromPath(); //Pops the current cell off the path stack.
            cout << "Removed current cell from path." << endl;
            targetDirection = oG->getDirectionOfLastCell(); //Gets direction of cell on top of the path stack.
        }
        
        targetYaw = targetDirection;

        if (targetDirection != currentDirection) {
            turnToNewDirection(targetYaw, &pp, &robot);  //Turns robot to face direction of next cell to travel to.    
        }

        moveToNextCell(&pp); //Moves robot roughly 0.6m/60cm in the current direction it is facing.
        
    } while (!oG->getPathStack().empty()); //Keeps the loop going while the path stack is not empty.
}

Pioneer::~Pioneer() {
    delete(oG); //Ensures the deletion of the occupancy grid.
}

int main(int argc, char *argv[]) {
    Pioneer *pioneer = new Pioneer(); //Creates new pioneer on heap.
    pioneer->runPioneer();
    delete(pioneer); //Ensures the deletion of pioneer.
    return 0;
}