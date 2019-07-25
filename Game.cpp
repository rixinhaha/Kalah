#include "Game.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Game::Game(const Board& b, Player* south, Player* north): m_board(b)
{
	m_south = south;
	m_north = north;
}

void Game::display() const
{
	/////////////////////////////////////////
	//Create and fill in values for northrow
	/////////////////////////////////////////
	vector<int> northrow(m_board.holes());
	for (int i = 0; i < m_board.holes(); i++)
	{
		northrow[i] = m_board.beans(NORTH, i+1);
	}

	///////////////////////////////////
	//create and fill in values for pot
	///////////////////////////////////
	vector<int> potrow(2);
	potrow[0] = m_board.beans(NORTH,0);
	potrow[1] = m_board.beans(SOUTH,0);
	
	/////////////////////////////////////////
	//Create and fill in values for southrow
	/////////////////////////////////////////
	vector<int> southrow(m_board.holes());
	for (int i = 0; i < m_board.holes(); i++)
	{
		southrow[i] = m_board.beans(SOUTH, i+1);
	}

	//////////////////
	//Display of grid 
	//////////////////
	for (int i = 0; i < m_board.holes(); i++)
	{
		cout << " ";
	}
	cout << m_north->name() << endl;
	cout << " ";
	for (size_t i = 0; i < m_board.holes(); i++)
	{
		cout << " ";
		cout << northrow[i];
		cout << " ";
	}
	cout << endl;

	cout << potrow[0];
	cout << "  ";
	for (size_t i = 1; i < m_board.holes(); i++)
	{
		cout << "   ";
	}
	cout << " ";
	cout << potrow[1] << endl;

	cout << " ";
	for (size_t i = 0; i < m_board.holes(); i++)
	{
		cout << " ";
		cout << southrow[i];
		cout << " ";
	}
	cout << endl;
	for (int i = 0; i < m_board.holes(); i++)
	{
		cout << " ";
	}
	cout << m_south->name() << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
	if (m_board.beansInPlay(NORTH) == 0 || m_board.beansInPlay(SOUTH) == 0) //if either side has 0 beans in play
	{
		over = true; //game is over
		//find the total number of beans for each side
		int northpot = m_board.beans(NORTH, 0) + m_board.beansInPlay(NORTH);
		int southpot = m_board.beans(SOUTH, 0) + m_board.beansInPlay(SOUTH);
		if (northpot > southpot) //if north has more beans than south, winner is north
		{
			hasWinner = true;
			winner = NORTH;
		}
		else if (northpot < southpot) //if south has more beans than north, winner is south
		{
			hasWinner = true;
			winner = SOUTH;
		}
		else //if it is a draw
		{
			hasWinner = false; //there is no winner, make no changes to winner
		}
		return;
	}
	else //if there are still beans in play
	{
		over = false; //game not over
		return;
	}
}

bool Game::move()
{
	status(over, hasWinner, winner); //call status function to check whether game is over
	if (over)
	{
		return(false); //if game over, no move possible, return false
	}
	else
	{
		if (m_turn == SOUTH)
		{
			int holetosole = m_south->chooseMove(m_board, SOUTH); //call the player choose move function
			if (!(m_south->isInteractive())) //if not human player
			{
				cout << m_south->name() << " chooses hole " << holetosole << "." << endl; //cout the statement which hole the south player chooses
			}
			m_board.sow(SOUTH, holetosole, endSide, endHole); //sow the board
			if (endSide == SOUTH && endHole == 0 && m_board.beansInPlay(SOUTH) != 0) //if lands in the south's pot(its own pot) and turn still available
			{
				display(); //display the grid 
				cout << m_south->name() << " gets another turn." << endl; 
				return(move()); //recursively call the function again 
			}
			else if (endSide == SOUTH && m_board.beans(SOUTH, endHole) == 1 && m_board.beans(NORTH, endHole) != 0) //if it is a capture move(lands in a hole on the same side initially empty), and hole on opposite side is not empty
			{
				m_board.moveToPot(NORTH, endHole, SOUTH);
				m_board.moveToPot(SOUTH, endHole, SOUTH);
			}
			m_turn = NORTH; //change of turn, after end of each move
		}
		else if (m_turn == NORTH) //if turn is north
		{
			int holetosole = m_north->chooseMove(m_board, NORTH); 
			if (!(m_north->isInteractive())) //if not human player
			{
				cout << m_north->name() << " chooses hole " << holetosole << "." << endl; //cout the statement which hole the north player chooses
			}
			m_board.sow(NORTH, holetosole, endSide, endHole);
			if (endSide == NORTH && endHole == 0 && m_board.beansInPlay(NORTH) != 0) //if repeat move
			{
				display();
				cout << m_north->name() << " gets another turn." << endl;
				return(move());
			}
			else if (endSide == NORTH && m_board.beans(NORTH, endHole) == 1 && m_board.beans(SOUTH, endHole) != 0) //if capture move
			{
				m_board.moveToPot(SOUTH, endHole, NORTH);
				m_board.moveToPot(NORTH, endHole, NORTH);
			}
			m_turn = SOUTH; //change of turn after move
		}
		status(over, hasWinner, winner); //call the status function
		
		if (over == true) //if game over
		{
			display(); //display grid
			if (m_board.beansInPlay(NORTH) == 0) //case of north, south player still has beans
			{
				for (int i = 1; i <= m_board.holes(); i++)
				{
					if (m_board.beans(SOUTH, i) != 0)
					{
						m_board.moveToPot(SOUTH, i, SOUTH); //put remaining beans into south's pot
					}
				}
				cout << "Sweeping remaining beans into " << m_south->name() << "'s pot." << endl;
				display();
			}
			else //case of south, north player still has beans
			{
				for (int i = 1; i <= m_board.holes(); i++)
				{
					if (m_board.beans(NORTH, i) != 0)
					{
						m_board.moveToPot(NORTH, i, NORTH); //put remaining beans into north's pot
					}
				}
				cout << "Sweeping remaining beans into " << m_north->name() << "'s pot." << endl;
				display();
			}
		}
		status(over, hasWinner, winner); //call status function to change the winner 
		return(true);
	}

}

void Game::play()
{
	if (!(m_south->isInteractive()) && !(m_north->isInteractive())) //both players ar bots
	{
		display(); //display grid
		if (move() == false) //if move returns false, means first move cannot be made, south side has no beans in play
		{
			for (int i = 1; i <= m_board.holes(); i++) 
			{
				m_board.moveToPot(NORTH, i, NORTH); //move all existing north beans into pot
			}
			cout << "Sweeping remaining beans into " << m_north->name() << "'s pot." << endl;
			display();
		}
		while (over != true) //while game is not over
		{
			display();
			move();
			cout << "Press ENTER to continue";
			cin.ignore(9600, '\n');

		}
		if (over == true) //if game is over
		{
			string victor;
			//check winner is south or north
			if (winner == SOUTH && hasWinner == true)
			{
				victor = m_south->name(); //assign south winner's name to victor
				cout << "The winner is " << victor << "." << endl;
			}
			else if (winner == NORTH && hasWinner == true)
			{
				victor = m_north->name(); //assign north winner's name to victor
				cout << "The winner is " << victor << "." << endl;
			}
			else
			{
				cout << "It is a draw, there is no winner." << endl;
			}

		}
	}
	else //one human and one computer
	{
		display(); //display grid 
		if (move() == false) //if no move possible
		{
			for (int i = 1; i <= m_board.holes(); i++)
			{
				m_board.moveToPot(NORTH, i, NORTH);
			}
			cout << "Sweeping remaining beans into " << m_north->name() << "'s pot." << endl;
			display();
		}
		while (over != true) //if game not over
		{
			display();
			move();
		}
		if (over == true)
		{
			string victor;
			//check winner is south or north
			if (winner == SOUTH && hasWinner == true)
			{
				victor = m_south->name(); //assign south winner's name to victor
				cout << "The winner is " << victor << "." << endl;
			}
			else if (winner == NORTH && hasWinner == true)
			{
				victor = m_north->name(); //assign north winner's name to victor
				cout << "The winner is " << victor << "." << endl;
			}
			else
			{
				cout << "It is a draw, there is no winner." << endl;
			}

		}
	}
	return;
}

int Game::beans(Side s, int hole) const
{
	if (hole >= 0 && hole <= m_board.holes()) //if the hole entered is valid
	{
		int numbeans = m_board.beans(s, hole);
		return(numbeans); 
	}
	else
	{
		return(-1);
	}
}