/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Pioneer.cpp
 * Description: Stores function declarations for Pioneer to use.
 * First Created: 25/02/2013
 * Last Modified: 25/02/2013
 */

public double movePioneer(double currentX, double currentY, double targetX, double targetY){
	double xChange = targetX - currentX;
	double yChange = targetY - currentY;

	return (xChange + yChange) * PGAIN;
}
