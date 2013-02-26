/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.cpp
 * Description: Stores function declarations for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 25/02/2013
 */

#include "Pioneer.h"
#include "Occupancy_Grid.h"

public double movePioneer(double currentX, double currentY, double targetX, double targetY){
	double xChange = targetX - currentX;
	double yChange = targetY - currentY;

	return (xChange + yChange) * PGAIN;
}

public void Pioneer::getFrontSensorFacing() {
	return frontSensorFacing;
}

public void Pioneer::getRearSensorFacing() {
	return rearSensorFacing;
}

public void Pioneer:getLeftSensorFacing() {
	return leftSensorFacing;
}

public void Pioneer::getRightSensorFacing() {
	return rightSensorFacing;
}

public void Pioneer::setFrontSensorDirection(double currentYaw) {
	if(currentYaw < 10.00 || currentYaw > 350.00) frontSensorFacing = UP;
	else if(currentYaw < 280.00 || currentYaw > 260.00) frontSensorFacing = RIGHT;
	else if(currentYaw < 190 || currentYaw > 170.00) frontSensorFacing = DOWN;
	else if(currentYaw < 100 || currentYaw > 80.00) frontSensorFacing = LEFT; 
}

public void Pioneer::setRearSensorDirection(double current_yaw) {
	if(currentYaw < 10.00 || currentYaw > 350.00) rearSensorFacing = DOWN;
	else if(currentYaw < 280.00 || currentYaw > 260.00) rearSensorFacing = LEFT;
	else if(currentYaw < 190 || currentYaw > 170.00) rearSensorFacing = UP;
	else if(currentYaw < 100 || currentYaw > 80.00) rearSensorFacing = RIGHT;
}

public void Pioneer::setLeftSensorDirection(double current_yaw) {
	if(currentYaw < 10.00 || currentYaw > 350.00) leftSensorFacing = LEFT;
	else if(currentYaw < 280.00 || currentYaw > 260.00) leftSensorFacing = UP;
	else if(currentYaw < 190 || currentYaw > 170.00) leftSensorFacing = RIGHT;
	else if(currentYaw < 100 || currentYaw > 80.00) leftSensorFacing = DOWN;
}

public void Pioneer::setRightSensorDirection(double current_yaw) {
	if(currentYaw < 10.00 || currentYaw > 350.00) rightSensorFacing = RIGHT;
	else if(currentYaw < 280.00 || currentYaw > 260.00) rightSensorFacing = DOWN;
	else if(currentYaw < 190 || currentYaw > 170.00) frontSensorFacing = LEFT;
	else if(currentYaw < 100 || currentYaw > 80.00) frontSensorFacing = UP;
} 
