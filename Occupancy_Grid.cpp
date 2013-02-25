/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.cpp
 * Description: Stores member function declarations for Occupancy_Grid class.
 * First Created: 25/02/2013
 * Last Modified: 25/02/2013
 */	

public Occupancy_Grid::Occupancy_Grid() {
	grid = calloc(grid, sizeof(int*) * 3); /* Allocates memory for 3 columns of the grid and sets values to 0. */
	
	for(counter = 0; counter<columnLength; counter++) {
		grid[counter] = calloc(grid, sizeof(int) * 3); /* Allocates memory for 3 rows of the grid and sets values to 0. */
	}
}

public void Occupany_Grid::resizeGrid(int directionToExpand) {
	int counter = 0;	

	if(directionToExpand == UP || directionToExpand == DOWN) {
		rowHeight++;
		grid = realloc(grid, sizeof(int*) * rowHeight); /* Increases grid's row height by 1, allocating memory for it. */
	
		for (counter; counter < columnLength; counter++) {
			grid[counter] [rowHeight - 1] = 0; /* Set values of new cells to 0. */
		}
		
		if(directionToExpand == DOWN) shiftValuesDown();
	}	
	else if(directionToExpand == LEFT || directionToExpand == RIGHT) {
		columnLength++;
		grid = realloc(grid, sizeof(int*) * columnLength); /* Increases grid's column length by 1, allocating memory for it. */
		
		for(counter; counter < rowHeight; counter++) {
			grid[columnLength - 1] [counter] = 0; /* Set values of new cells to 0. */
		}

		if(directionToExpand == LEFT) shiftValuesRight();
	}		
}

public void Occupancy_Grid::shiftValuesDown() {
	int columnCounter = 0;
	int rowCounter;

	for(columnCounter; columnCounter < columnLength; columnCounter++) {
		for (rowCounter = 1; rowCounter < rowHeight; rowCounter++) {
			grid[columnCounter][rowCounter - 1] = grid[columnCounter][rowCounter]; /* Copies value to the cell below. */
			grid[columnCounter][rowCounter] = 0; /* Sets value of cell to 0. */
		}
	}
}

public void Occupancy_Grid::shiftValuesRight() {
	int columnCounter;
	int rowCounter = 0;

	for(columnCounter = columnLength - 1; columnCounter >= 0; columnCounter--) {
		for (rowCounter; rowCounter < rowHeight; rowCounter++) {
			grid[columnCounter + 1][rowCounter] = grid[columnCounter][rowCounter]; /* Copies values to the cell to the right. */i
			grid[columnCounter][rowCounter] = 0; /* Sets value of cell to 0. */
		}
	}
}

public void printGrid() {
	int columnCounter = 0;
	int rowCounter = 0;

	for(columnCounter; columnCounter < columnLength; columnCounter++) {
		for(rowCounter; rowCounter < rowHeight; rowCounter++) {
			cout << grid[columnCounter][rowCounter];
		}
	}
}
