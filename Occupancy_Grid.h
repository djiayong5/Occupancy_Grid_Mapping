/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.h
 * Description: Stores member function prototypes and class definition for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 26/02/2013
 */


#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

/* Occupancy_Grid class definition */
public class Occupancy_Grid {
	private:
		int** grid;
		int columnLength;
		int rowHeight;
		int robotX;
		int robotY;
	
	public:
		/* Member Function Prototypes: */
		Occupancy_Grid();
		void resizeGrid(int directionToExpand);
		void shiftValuesDown();
		void shiftValuesRight();
		void printGrid();
		void gridUpdate(int direction);
		void mapRobotLocation(int direction);
		int getRobotY();
		int getRobotX();
}
