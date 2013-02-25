/*
 * Author: Chris Savill, chs17@aber.ac.uk
 * File Name: Occupancy_Grid.cpp
 * Description: Stores member function declarations.
 * First Created: 25/02/2013
 * Last Modified: 25/02/2013
 */	

public Occupancy_Grid() {
	grid = calloc(grid, sizeof(int*) * 3); /* Allocates memory for columns of grid and sets values to 0. */
	
	for(counter = 0; counter<columnLength; counter++) {
		grid[counter] = calloc(grid, sizeof(int) * 3); /* Allocates memory for rows of grid and sets values to 0. */
	}
}

public void resizeGrid(int directionToExpand) {
	if(directionToExpand == UP || directionToExpand == DOWN) {
		rowHeight++;
		grid = realloc(grid, sizeof(int*) * rowHeight); /* Increases grid's row height by 1, allocating memory for it. */
	
		for (counter = 0; counter < columnLength; counter++) {
			grid[counter] [rowHeight - 1] = 0; /* Set values of new cells to 0. */
		}
		
		if(directionToExpand == DOWN) shiftValuesDown();
	}	
	else if(directionToExpand == LEFT || directionToExpand == RIGHT) {
		columnLength++;
		grid = realloc(grid, sizeof(int*) * columnLength); /* Increases grid's column length by 1, allocating memory for it. */
		
		for(counter = 0; counter < rowHeight; counter++) {
			grid[columnLength - 1] [counter] = 0; /* Set values of new cells to 0. */
		}

		if(directionToExpand == LEFT) shiftValuesRight();
	}		
}

