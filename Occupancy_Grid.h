/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.h
 * Description: Stores member function prototypes and class definition for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 25/02/2013
 */


#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

/* Occupancy_Grid class definition */
public class Occupancy_Grid {
	private int** grid;
	private int columnLength;
	private int rowHeight;

	/* Member Function Prototypes: */
	public Occupancy_Grid();
	public void resizeGrid(int directionToExpand);
	public void shiftValuesDown();
	public void shiftValuesRight();
	public void printGrid();

}
