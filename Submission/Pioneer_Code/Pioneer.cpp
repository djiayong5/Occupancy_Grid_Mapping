/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.cpp
 * Description: Stores function declarations for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 17/04/2013
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
#include "Pioneer.h"
#include "Occupancy_Grid.h"
#include <cstdio>
#include <cstdlib>
#include <math.h>

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

	if (targetYaw == TOP) {
	    if (currentYaw >= (360 - ANGLE_ERROR) || currentYaw <= (targetYaw + ANGLE_ERROR)) {
		turnRate = 0.000;
		yawAchieved = true;
	    }
        } else if (currentYaw >= (targetYaw - ANGLE_ERROR) && currentYaw <= (targetYaw + ANGLE_ERROR)) {
            turnRate = 0.000;
            yawAchieved = true;
        } 
        
        if (yawAchieved != true) turnRate = calculateTurnRate(currentYaw, targetYaw);

        pp->SetSpeed(0.000, turnRate);
    }

    cout << "Turn Complete, Now Facing: " << currentYaw << endl;
}

void Pioneer::calculateMoveDistance(PlayerClient *robot, Position2dProxy *pp, int direction, double distanceToMove) {
    if (direction == TOP) moveForward(robot, pp, direction, distanceToMove);
    else if (direction == BOTTOM) moveForward(robot, pp, direction, -distanceToMove);
    else if (direction == LEFT) moveForward(robot, pp, direction, distanceToMove);
    else if (direction == RIGHT) moveForward(robot, pp, direction, -distanceToMove);
}

void Pioneer::moveForward(PlayerClient *robot, Position2dProxy *pp, int direction, double posDifference) {
    bool travelledDistance = false; //Boolean condition to break out of while loop.
    double current;
    double target;
    double speed;
    double distanceLeft;
    robot->Read();

    if (direction == TOP || direction == BOTTOM) {      
        current = pp->GetXPos();
        target = current + posDifference;
    } else if (direction == LEFT || direction == RIGHT) {
        current = pp->GetYPos();
        target = current + posDifference;       
    }

    while (travelledDistance == false) {
        robot->Read();

        if (direction == TOP || direction == BOTTOM) {
            current = pp->GetXPos();
        } else if (direction == LEFT || direction == RIGHT) {
            current = pp->GetYPos();
        }

        distanceLeft = target - current;

        if (distanceLeft <= MOVE_ERROR_BOUND && distanceLeft >= -MOVE_ERROR_BOUND) {
            travelledDistance = true;
            speed = 0.000;
        } else {
            speed = (sqrt(distanceLeft * distanceLeft)) * MOVE_PGAIN;
            speed = sqrt(speed * speed);
        }

        pp->SetSpeed(speed, 0.000);
    }
}

int Pioneer::evaluateDirection(double currentYaw) {
    if (currentYaw <= (RIGHT + ANGLE_ERROR) && currentYaw >= (RIGHT - ANGLE_ERROR)) return RIGHT;
    else if (currentYaw <= (LEFT + ANGLE_ERROR) && currentYaw >= (LEFT - ANGLE_ERROR)) return LEFT;
   // else if ((currentYaw <= -170 && currentYaw >= -179.99) || (currentYaw >= 170.00 && currentYaw <= 180.00)) return BOTTOM;
    else if (currentYaw <= (BOTTOM + ANGLE_ERROR) && currentYaw >= (BOTTOM - ANGLE_ERROR)) return BOTTOM;
   //if ((currentYaw <= 10.00 && currentYaw >= 0.00) || (currentYaw >= -10.00 && currentYaw <= 0.00)) return TOP;
    else if (currentYaw >= (360 - ANGLE_ERROR) || currentYaw <= (TOP + ANGLE_ERROR)) return TOP;
    else cout << "Not able to evalutate direction, current yaw = " << currentYaw << endl;
    return -1;
}

int Pioneer::setSensorDirection(int currentDirection, int angleOffset) {
    int facing = 0;

    if (currentDirection == TOP) facing = (TOP + angleOffset);
    else if (currentDirection == RIGHT) facing = (RIGHT + angleOffset);
    else if (currentDirection == BOTTOM) facing = (BOTTOM + angleOffset);
    else if (currentDirection == LEFT) facing = (LEFT + angleOffset);

    if (facing == 360) facing = TOP;
    else if (facing > 360) facing = facing - 360;
    return facing;
}

void Pioneer::reconfigureSensors(int currentDirection) {
    frontSensorFacing = setSensorDirection(currentDirection, TOP);
    frontRightSensorFacing = setSensorDirection(currentDirection, TOP_RIGHT);
    rightSensorFacing = setSensorDirection(currentDirection, RIGHT);
    rearRightSensorFacing = setSensorDirection(currentDirection, BOTTOM_RIGHT);
    rearSensorFacing = setSensorDirection(currentDirection, BOTTOM);
    rearLeftSensorFacing = setSensorDirection(currentDirection, BOTTOM_LEFT);
    leftSensorFacing = setSensorDirection(currentDirection, LEFT);
    frontLeftSensorFacing = setSensorDirection(currentDirection, TOP_LEFT);
}

void Pioneer::surveyCycle(double readings[], int currentDirection, bool inNextCell, Occupancy_Grid *grid, bool seekMode) {
    if (inNextCell == true) {
        cout << "In next cell." << endl;

        evaluateReadings(readings[3], readings[4], FRONT_REAR_RANGE, frontSensorFacing, grid, seekMode);
        evaluateBlindSpots(readings[2], readings[5], CLOSE_RANGE, frontSensorFacing, grid, seekMode);
        evaluateReadings(readings[12], readings[11], FRONT_REAR_RANGE, rearSensorFacing, grid, seekMode);
        evaluateBlindSpots(readings[10], readings[13], CLOSE_RANGE, rearSensorFacing, grid, seekMode);
        evaluateReadings(readings[0], readings[15], LEFT_RIGHT_RANGE, leftSensorFacing, grid, seekMode);
        evaluateReadings(readings[7], readings[8], LEFT_RIGHT_RANGE, rightSensorFacing, grid, seekMode);

        evaluateCornerReadings(readings[1], CLOSE_RANGE, CORNER_RANGE, frontLeftSensorFacing, grid, seekMode);
        evaluateCornerReadings(readings[6], CLOSE_RANGE, CORNER_RANGE, frontRightSensorFacing, grid, seekMode);
        evaluateCornerReadings(readings[9], CLOSE_RANGE, CORNER_RANGE, rearRightSensorFacing, grid, seekMode);
        evaluateCornerReadings(readings[14], CLOSE_RANGE, CORNER_RANGE, rearLeftSensorFacing, grid, seekMode);
    } else {
        cout << "Half way to next cell." << endl;
        grid->checkResizeNeeded(currentDirection); //Checks if the grid needs expanding.

        evaluateMovingReadings(readings[0], readings[1], LEFT_RIGHT_RANGE, CLOSE_RANGE, leftSensorFacing, grid, seekMode);
        evaluateMovingReadings(readings[7], readings[6], LEFT_RIGHT_RANGE, CLOSE_RANGE, rightSensorFacing, grid, seekMode);
    }

    grid->checkCellNeighbours();
    cout << endl; //Used for formatting output.
}

void Pioneer::evaluateReadings(double reading1, double reading2, double range, int sensorFacing, Occupancy_Grid *grid, bool seekMode) {
    if (reading1 <= range || reading2 <= range) {
        grid->calculateCellToChange(sensorFacing, true, seekMode);
    } else {
        grid->calculateCellToChange(sensorFacing, false, seekMode);
    }
}

void Pioneer::evaluateCornerReadings(double reading, double lowerBound, double upperBound, int sensorFacing, Occupancy_Grid *grid, bool seekMode) {
    if (reading >= lowerBound && reading <= upperBound) {
        grid->calculateCellToChange(sensorFacing, true, seekMode);
    } else {
        grid->calculateCellToChange(sensorFacing, false, seekMode);
    }
}

void Pioneer::evaluateMovingReadings(double reading1, double reading2, double range1, double range2, int sensorFacing, Occupancy_Grid *grid, bool seekMode) {
    if (reading1 <= range1 || reading2 <= range2) grid->calculateCellToChange(sensorFacing, true, seekMode);
}

void Pioneer::evaluateBlindSpots(double reading1, double reading2, double range, int sensorFacing, Occupancy_Grid *grid, bool seekMode) {
    if (reading1 <= range || reading2 <= range) grid->calculateCellToChange(sensorFacing, true, seekMode);
}

void Pioneer::configureCycle(PlayerClient *robot, Position2dProxy *pp, double *currentYaw, int *currentDirection) {
    robot->Read(); //Reads from the proxies.
    *currentYaw = rtod(pp->GetYaw()); //Gets the robot's current yaw and converts it from radians to degrees.
    *currentDirection = evaluateDirection(*currentYaw); //Evaluates the robots direction.
    cout << "Current direction: " << *currentDirection << endl;
    reconfigureSensors(*currentDirection);
}

void Pioneer::map(PlayerClient *robot, SonarProxy *sp, Position2dProxy *pp) {
    oG->mapConfigure();
    double currentYaw = 0.000;
    double targetYaw = 0.000;
    double sonarReadings[16];
    int currentDirection;
    int targetDirection;

    oG->printGrid(); /* Print out initial grid. */
    pp->SetMotorEnable(true); /* Enable motors. */
    robot->Read(); /* Read from proxies. */
    currentYaw = rtod(pp->GetYaw()); /* Retrieve current yaw. */
    cout << "Start Yaw: " << currentYaw << endl;

    turnToNewDirection(targetYaw, pp, robot); //Gets robot to turn to 0 degrees (with error bounds).
    cout << "Start Yaw Corrected to: " << currentYaw << endl;

    do {
		turnToNewDirection(targetYaw, pp, robot); //Rechecks angle within limits and turn.   
        configureCycle(robot, pp, &currentYaw, &currentDirection);
        for (int counter = 0; counter <= 15; counter++) sonarReadings[counter] = (*sp)[counter];
        oG->setIsExploredTrue();
        surveyCycle(sonarReadings, currentDirection, true, oG, false); //Takes the sonar readings and marks cells as appropriate.
        oG->printGrid(); //Prints the occupancy grid.
        cout << "Neighbours unexplored: " << oG->getNeighboursUnexplored() << endl;

        if (oG->getNeighboursUnexplored() != 0) {
            cout << "Picking a neighbour to explore..." << endl;
            targetDirection = oG->chooseNextCell(currentDirection); //Chooses the next unexplored neighbour cell to travel to.
        } else if (oG->getNeighboursUnexplored() == 0) {
            cout << "All neighbours of current cell explored." << endl;

            if (oG->checkFinished()) {
                cout << "Finished Mapping.";
            } else if (!oG->getPathStack().empty()) {
                cout << "Path not empty." << endl;
                targetDirection = oG->getDirectionOfLastCell(); //Gets direction of cell on top of the path stack.
                cout << "New Direction: " << targetDirection << endl;
            }
        }

        if (!oG->getPathStack().empty()) {
            targetYaw = targetDirection;
            turnToNewDirection(targetYaw, pp, robot); //Turns robot to face direction of next cell to travel to.    

            configureCycle(robot, pp, &currentYaw, &currentDirection);
            for (int counter = 0; counter <= 15; counter++) sonarReadings[counter] = (*sp)[counter];
            surveyCycle(sonarReadings, currentDirection, true, oG, false); //Takes the sonar readings and marks cells as appropriate.
            oG->printGrid(); //Prints the occupancy grid.

            oG->moveRobotOnGrid(currentDirection);
            calculateMoveDistance(robot, pp, currentDirection, 0.350);

            robot->Read();
            for (int counter = 0; counter <= 15; counter++) sonarReadings[counter] = (*sp)[counter];
            surveyCycle(sonarReadings, currentDirection, false, oG, false); //Takes the sonar readings and marks cells as appropriate.
            calculateMoveDistance(robot, pp, currentDirection, 0.350);
        } else {
            cout << "Path stack empty, mapping finished." << endl << endl;
        }
    } while (!oG->getPathStack().empty()); //Keeps the loop going while the path stack is not empty.

    pp->SetMotorEnable(false);
}

bool Pioneer::localise(PlayerClient *robot, SonarProxy *sp, Position2dProxy *pp) {
    Occupancy_Grid *temp = new Occupancy_Grid();
    double currentYaw = 0.000;
    double targetYaw;
    double sonarReadings[16];
    int currentDirection;
    int targetDirection;

    temp->printGrid(); /* Print out initial grid. */
    pp->SetMotorEnable(true); /* Enable motors. */
    robot->Read(); /* Read from proxies. */
    currentYaw = rtod(pp->GetYaw()); /* Retrieve current yaw. */
    targetYaw = currentYaw;

    do {
		turnToNewDirection(targetYaw, pp, robot); //Rechecks angle within limits and turn.
        configureCycle(robot, pp, &currentYaw, &currentDirection);
        for (int counter = 0; counter <= 15; counter++) sonarReadings[counter] = (*sp)[counter];
        temp->setIsExploredTrue();
        surveyCycle(sonarReadings, currentDirection, true, temp, false); //Takes the sonar readings and marks cells as appropriate.
        temp->printGrid(); //Prints the occupancy grid.

        if (oG->attemptLocalisation(temp) == 3) { //Localised successfully.
            oG->switchGrid();
            pp->SetMotorEnable(false);
            return true;
        } else if (oG->attemptLocalisation(temp) == 2) { //Multiple possible locations.
            cout << "Neighbours unexplored: " << temp->getNeighboursUnexplored() << endl;

            if (temp->getNeighboursUnexplored() != 0) {
                cout << "Picking a neighbour to explore..." << endl;
                targetDirection = temp->chooseNextCell(currentDirection); //Chooses the next unexplored neighbour cell to travel to.
            } else if (oG->getNeighboursUnexplored() == 0) {
                cout << "All neighbours of current cell explored." << endl;

                if (temp->checkFinished()) {
                    pp->SetMotorEnable(false);
                    return false;
                } else if (!temp->getPathStack().empty()) {
                    cout << "Path not empty." << endl;
                    targetDirection = temp->getDirectionOfLastCell(); //Gets direction of cell on top of the path stack.
                    cout << "New Direction: " << targetDirection << endl;
                }
            }

            if (!temp->getPathStack().empty()) {
                targetYaw = targetDirection;
                turnToNewDirection(targetYaw, pp, robot); //Turns robot to face direction of next cell to travel to.    

                configureCycle(robot, pp, &currentYaw, &currentDirection);
                for (int counter = 0; counter <= 15; counter++) sonarReadings[counter] = (*sp)[counter];
                surveyCycle(sonarReadings, currentDirection, true, temp, false); //Takes the sonar readings and marks cells as appropriate.
                temp->printGrid(); //Prints the occupancy grid.

                temp->moveRobotOnGrid(currentDirection);
                calculateMoveDistance(robot, pp, currentDirection, 0.350);

                robot->Read();
                for (int counter = 0; counter <= 15; counter++) sonarReadings[counter] = (*sp)[counter];
                surveyCycle(sonarReadings, currentDirection, false, temp, false); //Takes the sonar readings and marks cells as appropriate.
                calculateMoveDistance(robot, pp, currentDirection, 0.350);
            }
        } else {
            pp->SetMotorEnable(false);
            return false; //Failed to localise.
        }

    } while (true);
}

void Pioneer::seek(PlayerClient *robot, SonarProxy *sp, Position2dProxy *pp) {
    double currentYaw = 0.000;
    double targetYaw;
    double sonarReadings[16];
    int currentDirection;
    int targetDirection;

    oG->seekConfigure();
    oG->printGrid(); /* Print out initial grid. */
    pp->SetMotorEnable(true); /* Enable motors. */
    robot->Read(); /* Read from proxies. */
    currentYaw = rtod(pp->GetYaw()); /* Retrieve current yaw. */
    targetYaw = currentYaw;

    do {
		turnToNewDirection(targetYaw, pp, robot); //Rechecks angle within limits and turn.
        configureCycle(robot, pp, &currentYaw, &currentDirection);
        for (int counter = 0; counter <= 15; counter++) sonarReadings[counter] = (*sp)[counter];
        oG->setIsExploredTrue();
        surveyCycle(sonarReadings, currentDirection, true, oG, true); //Takes the sonar readings and marks cells as appropriate.

        if (!oG->getAnomalyFound()) {
            oG->printGrid(); //Prints the occupancy grid.
            cout << "Neighbours unexplored: " << oG->getNeighboursUnexplored() << endl;

            if (oG->getNeighboursUnexplored() != 0) {
                cout << "Picking a neighbour to explore..." << endl;
                targetDirection = oG->chooseNextCell(currentDirection); //Chooses the next unexplored neighbour cell to travel to.
            } else if (oG->getNeighboursUnexplored() == 0) {
                cout << "All neighbours of current cell explored." << endl;

                if (oG->checkFinished()) {
                    cout << "Finished Mapping.";
                } else if (!oG->getPathStack().empty()) {
                    cout << "Path not empty." << endl;
                    targetDirection = oG->getDirectionOfLastCell(); //Gets direction of cell on top of the path stack.
                    cout << "New Direction: " << targetDirection << endl;
                }
            }

            if (!oG->getPathStack().empty()) {
                targetYaw = targetDirection;
                turnToNewDirection(targetYaw, pp, robot); //Turns robot to face direction of next cell to travel to.    

                configureCycle(robot, pp, &currentYaw, &currentDirection);
                for (int counter = 0; counter <= 15; counter++) sonarReadings[counter] = (*sp)[counter];
                surveyCycle(sonarReadings, currentDirection, true, oG, true); //Takes the sonar readings and marks cells as appropriate.
                oG->printGrid(); //Prints the occupancy grid.

                oG->moveRobotOnGrid(currentDirection);
                calculateMoveDistance(robot, pp, currentDirection, 0.350);

                robot->Read();
                for (int counter = 0; counter <= 15; counter++) sonarReadings[counter] = (*sp)[counter];
                surveyCycle(sonarReadings, currentDirection, false, oG, true); //Takes the sonar readings and marks cells as appropriate.
                calculateMoveDistance(robot, pp, currentDirection, 0.350);
            } else {
                cout << "Path stack empty, mapping finished." << endl << endl;
            }
        }
    } while (!oG->getPathStack().empty() && oG->getAnomalyFound() == false); //Keeps the loop going while the path stack is not empty.

    pp->SetMotorEnable(false);
    if (oG->getAnomalyFound() == false) cout << "Failed to find any anomalies." << endl;
}

void Pioneer::hide(PlayerClient *robot, SonarProxy *sp, Position2dProxy *pp) {
    double currentYaw = 0.000;
    double targetYaw = 0.000;
    int currentDirection;
    int targetDirection;

    oG->printGrid(); /* Print out initial grid. */
    pp->SetMotorEnable(true); /* Enable motors. */
    oG->getHideLocation();

    do {
		turnToNewDirection(targetYaw, pp, robot); //Rechecks angle within limits and turn.
        configureCycle(robot, pp, &currentYaw, &currentDirection);
        targetDirection = oG->getNextCellDriection();
        targetYaw = targetDirection;

        if (!oG->getPathStack().empty()) {
            targetYaw = targetDirection;
            turnToNewDirection(targetYaw, pp, robot); //Turns robot to face direction of next cell to travel to.    

            oG->moveRobotOnGrid(currentDirection);
            calculateMoveDistance(robot, pp, currentDirection, 0.700);
        } else {
            cout << "No more cells to move, robot should be in hiding location." << endl << endl;
        }
    } while (!oG->getPathStack().empty()); //Keeps the loop going while the hide path stack is not empty.

    pp->SetMotorEnable(false);
}

Pioneer::Pioneer() {
    oG = new Occupancy_Grid(); //Creates new occupancy grid on the heap;
}

Pioneer::~Pioneer() {
    delete(oG); //Ensures the deletion of the occupancy grid.
}

int main(int argc, char *argv[]) {
    Pioneer *pioneer = new Pioneer(); //Creates new pioneer on heap.  
    //PlayerClient robot("localhost");
    PlayerClient robot("lisa.islnet");
    //RangerProxy sp(&robot, 0);
    SonarProxy sp(&robot, 0);
    Position2dProxy pp(&robot, 0);

    pioneer->runProgram(&robot, &sp, &pp);
    delete(pioneer); //Ensures the deletion of pioneer.
    return 0;
}

void Pioneer::runProgram(PlayerClient *robot, SonarProxy *sp, Position2dProxy *pp) {
    int option; //Field to store the user's option input.
    bool mapped = false;

    do {
        cout << "***********************************" << endl;
        cout << "*         Pioneer Program         *" << endl;
        cout << "***********************************" << endl;
        cout << "*         1. Map Area             *" << endl;
        cout << "*         2. Seek                 *" << endl;
        cout << "*         3. Hide                 *" << endl;
        cout << "*         4. Exit                 *" << endl;
        cout << "***********************************" << endl << endl;

        cout << "Please enter in an option from the above an press 'Enter': ";
        cin.clear();
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1:
                map(robot, sp, pp);
                mapped = true;
                break;
            case 2:
                if (mapped == true) {
                    if (localise(robot, sp, pp)) {
                        cout << "Localisation successful, starting to seek..." << endl;
                        seek(robot, sp, pp);
                    } else cout << "Pioneer failed to localise itself, cannot seek." << endl;
                } else cout << "Pioneer has not mapped the area yet, cannot localise for seeking." << endl;
                break;
            case 3:
                if (mapped == true) {
                    if (localise(robot, sp, pp)) {
                        cout << "Localisation successful, starting to hide..." << endl;
                        hide(robot, sp, pp);
                    } else cout << "Pioneer failed to localise itself, cannot hide." << endl;
                } else cout << "Pioneer has not mapped the area yet, cannot localise for hiding." << endl;
                break;
            case 4:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Please enter in a valid option." << endl << endl;
        }
    } while (option != 4);
}
