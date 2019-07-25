#include "Board.h"
#include <iostream>

using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole): grid0(nullptr), grid1(nullptr)
{
	if (nHoles < 1) //treat negative hole number as 1
	{
		nHoles = 1;
	}
	if (nInitialBeansPerHole < 0) //treat negative beans per hole as 0
	{
		nInitialBeansPerHole = 0;
	}
	m_holes_side = nHoles;
	m_total_holes = m_holes_side * NSIDES;
	num_beans_north = nInitialBeansPerHole * m_holes_side;
	num_beans_south = nInitialBeansPerHole * m_holes_side;
	//////////////////////////////////////////////////////
	//initialize the grid
	//////////////////////////////////////////////////////
	grid0 = new int[m_holes_side + 1]; //account for the pot on each side
	grid1 = new int[m_holes_side + 1]; //account for the pot on each side

	//fill grid with beans
	//////////////////////////////////////
	//North row
	//////////////////////////////////////
	for (int i = 1; i <= m_holes_side; i++)
	{
		grid0[i] = nInitialBeansPerHole; //fill north row with beans
	}
	grid0[0] = 0; //intialise north pot as empty
	//////////////////////////////////////
	//South row
	//////////////////////////////////////
	for (int i = 1; i <= m_holes_side; i++)
	{
		grid1[i] = nInitialBeansPerHole; //fill south row with beans
	}
	grid1[0] = 0; //intialise south pot as empty
}

Board::Board(const Board& copy)
{
	m_holes_side = copy.m_holes_side;
	m_total_holes = copy.m_total_holes;
	num_beans_north = copy.num_beans_north;
	num_beans_south = copy.num_beans_south;
	//////////////////////////////////////////////////////
	//initialize the grid
	//////////////////////////////////////////////////////
	grid0 = new int[m_holes_side + 1]; //account for the pot on each side
	grid1 = new int[m_holes_side + 1]; //account for the pot on each side
	//fill grid with beans
	//////////////////////////////////////
	//North row
	//////////////////////////////////////
	for (int i = 1; i <= m_holes_side; i++)
	{
		grid0[i] = copy.grid0[i]; //fill north row with beans
	}
	grid0[0] = copy.grid0[0]; //intialise north pot as empty
	//////////////////////////////////////
	//South row
	//////////////////////////////////////
	for (int i = 1; i <= m_holes_side; i++)
	{
		grid1[i] = copy.grid1[i]; //fill south row with beans
	}
	grid1[0] = copy.grid1[0]; //intialise south pot as empty
}

//Board& Board::operator=(const Board& rhs) //assignment operator
//{
//	if (this != &rhs)
//	{
//		/////////////////////////////////////////////
//		//Copy all the private members
//		/////////////////////////////////////////////
//		m_holes_side = rhs.m_holes_side;
//		m_total_holes = rhs.m_total_holes;
//		num_beans_north = rhs.num_beans_north;
//		num_beans_south = rhs.num_beans_south;
//
//		//////////////////////////////////////////////////////
//		//initialize the grid
//		//////////////////////////////////////////////////////
//		grid0 = new int[m_holes_side + 1]; //account for the pot on each side
//		grid1 = new int[m_holes_side + 1];
//		////////////////////////////////////////////////////
//		//fill the grid with same number of beans from rhs//
//		////////////////////////////////////////////////////
//		for (int i = 0; i <= rhs.holes(); i++)
//		{
//			grid0[i] = rhs.grid0[i];
//			grid1[i] = rhs.grid1[i];
//		}
//	}
//	return *this;
//}

Board& Board::operator=(const Board& rhs)
{
	if (this != &rhs)
	{
		delete[]grid1;
		delete[]grid0;
		m_holes_side = rhs.m_holes_side;
		m_total_holes = rhs.m_total_holes;
		num_beans_north = rhs.num_beans_north;
		num_beans_south = rhs.num_beans_south;
		////////////////////////////////////
		grid0 = new int[m_holes_side + 1];
		grid1 = new int[m_holes_side + 1];
		////////////////////////////////////
		for (int i = 1; i <= m_holes_side; i++)
		{
			grid0[i] = rhs.grid0[i]; //fill north row with beans
		}
		grid0[0] = rhs.grid0[0]; //intialise north pot as empty
		for (int i = 1; i <= m_holes_side; i++)
		{
			grid1[i] = rhs.grid1[i]; //fill south row with beans
		}
		grid1[0] = rhs.grid1[0]; //intialise south pot as empty
	}
	return (*this);
}

Board::~Board()
{
	delete [] grid0;
	delete [] grid1;

	//for (int i = 0; i < 2; i++)
	//{
	//	delete[] grid[i]; //delete the 2 arrays of integers first
	//}
	//delete []grid; //delete the array of 2 pointers

}



int Board::holes() const
{
	return(m_holes_side);
}

int Board::beans(Side s, int hole) const
{
	if (s == NORTH)
	{
		if (hole >= 0 && hole <= m_holes_side )
		{
			return(grid0[hole]);
		}
		else
		{
			return(-1);
		}
	}
	else
	{
		if (hole >= 0 && hole <= m_holes_side)
		{
			return(grid1[hole]);
		}
		else
		{
			return(-1);
		}
	}
}

int Board::beansInPlay(Side s) const
{
	if (s == NORTH)
	{
		return(num_beans_north - grid0[0]);
	}
	else
	{
		return(num_beans_south - grid1[0]);
	}
}

int Board::totalBeans() const
{
	return(num_beans_north + num_beans_south);
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
	if (!(hole >= 1 && hole <= m_holes_side))
	{
		return(false);
	}
	if (s == SOUTH)
	{
		if (grid1[hole] == 0)
		{
			return(false);
		}
	}
	if (s == NORTH)
	{
		if (grid0[hole] == 0)
		{
			return(false);
		}
	}
	/////////////////////////////////////////////////////////////////
	//hole number enter is valid
	////////////////////////////////////////////////////////////////
	if (s == NORTH)
	{
		int beanstosow = grid0[hole];
		grid0[hole] = 0; //remove all the beans form the particular side and row
		int pos_nextsow = hole;
		Side currentside = NORTH;
		while (beanstosow != 0)
		{
			if (currentside == NORTH && pos_nextsow == 0) //when hit north's pot and next move should be first cell of south
			{
				currentside = SOUTH;
				pos_nextsow = 1;
			}
			else if (currentside == NORTH && pos_nextsow != 0) //anywhere between the last cell and first cell of north, next pos-1
			{
				currentside = NORTH;
				pos_nextsow -= 1;
			}
			else if (currentside == SOUTH && pos_nextsow == m_holes_side) //when at south and hits last cell of south before south's pot, got to last cell of north(anticlockwise)
			{
				currentside = NORTH;
				pos_nextsow = m_holes_side;
			}
			else if (currentside == SOUTH && pos_nextsow != m_holes_side) //when at south but did not hit south's pot, stay at currentside and nextpos+1
			{
				currentside = SOUTH;
				pos_nextsow += 1;
			}
			////////////////////////////////////////////////////////
			if (currentside == NORTH) //when sow in north
			{
				grid0[pos_nextsow] += 1;
				endHole = pos_nextsow;
				beanstosow--;
			}
			else //when sow is in south(different side)
			{
				grid1[pos_nextsow] += 1;
				endHole = pos_nextsow;
				beanstosow--;
				//account for change in the number of north beans and south beans
				num_beans_north--; 
				num_beans_south++; 
			}
		}
		endSide = currentside;
		return(true);
	}
	else
	{
		int beanstosow = grid1[hole];
		grid1[hole] = 0;
		int pos_nextsow = hole;
		Side currentside = SOUTH;
		bool hitSouthPot = false; //boolean to indicate if south pot is hit
		while (beanstosow != 0)
		{
			if (currentside == SOUTH && pos_nextsow == m_holes_side) //when at south and hit the last cell of south
			{
				currentside = SOUTH;
				pos_nextsow = 0; //next pos is south pot
				hitSouthPot = true; //set boolean to true
			}
			else if (currentside == SOUTH && hitSouthPot == true) //when at south and at south's pot
			{
				currentside = NORTH; //change the side
				pos_nextsow = m_holes_side; //set the next to north's last cell
				hitSouthPot = false; //set boolean to false
			}
			else if (currentside == SOUTH && hitSouthPot == false && pos_nextsow!=m_holes_side) //from the first to one before the last south cell
			{
				currentside = SOUTH; //remain same side
				pos_nextsow += 1; //increment position by one
			}
			else if (currentside == NORTH && pos_nextsow == 1) //when at north and positon is 1
			{
				currentside = SOUTH; //change side, skipping the north pot
				pos_nextsow = 1; //go to the first cell of south
			}
			else if (currentside == NORTH && pos_nextsow != 1 && pos_nextsow != 0) //from the last to the first north cell, leaving out the pot
			{
				currentside = NORTH; //remain same side
				pos_nextsow -= 1;
			}
			////////////////////////////////////////////////////////
			if (currentside == SOUTH) //when sow is in south
			{
				grid1[pos_nextsow] += 1;
				endHole = pos_nextsow;
				beanstosow--;
			}
			else //when sow is in north(different side)
			{
				grid0[pos_nextsow] += 1;
				endHole = pos_nextsow;
				num_beans_north++;
				num_beans_south--;
				beanstosow--;
			}
		}
		endSide = currentside;
		return(true);
	}

}


bool Board::moveToPot(Side s, int hole, Side potOwner)
{
	if (hole>=1 && hole <= m_holes_side)
	{
		int temp;
		if (s == NORTH)
		{
			temp = grid0[hole];
			grid0[hole] = 0;
		}
		else
		{
			temp = grid1[hole];
			grid1[hole] = 0;
		}
		if (potOwner == NORTH)
		{
			grid0[0] += temp;
			if (s == SOUTH) //account for changing number of beans (during capture)
			{
				num_beans_north += temp;
				num_beans_south -= temp;
			}
		}
		else
		{
			grid1[0] += temp;
			if (s == NORTH) //account for changing number of beans (during capture)
			{
				num_beans_south += temp;
				num_beans_north -= temp;
			}
		}
		return(true);
	}
	else
	{
		return(false);
	}
}

bool Board::setBeans(Side s, int hole, int beans)
{
	if (beans < 0)
	{
		return(false);
	}
	if (!(hole >= 0 && hole <= m_holes_side))
	{
		return(false);
	}
	////////////////////////////////////////////////
	if (s == NORTH)
	{
		num_beans_north -= grid0[hole];
		grid0[hole] = beans;
		num_beans_north += beans;
	}
	else
	{
		num_beans_south -= grid1[hole];
		grid1[hole] = beans;
		num_beans_south += beans;
	}
	return(true);
}
