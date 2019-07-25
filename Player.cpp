#include<iostream>
#include<string>
#include "Player.h"
#include <vector>
#include <limits.h>

using namespace std;

Player::Player(string name): m_name(name)
{}

string Player::name() const
{
	return(m_name);
}

bool Player::isInteractive() const
{
	return(true);
}

Player::~Player()
{}

//////////////////////////////////////////////////
//Human Player
//////////////////////////////////////////////////
HumanPlayer::HumanPlayer(string name) : Player(name)
{}


bool HumanPlayer::isInteractive() const
{
	return(true);
}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
	bool movepossible = false;
	for (int i = 1; i <= b.holes(); i++) //first check whether move is possible
	{
		if (b.beans(s, i) != 0)
		{
			movepossible = true; //if move possible break
			break;
		}
	}
	if (movepossible)
	{
		int holeselected = -1; //initialize to be -1
		cout << "Select a hole, " + name() + ": "; 
		cin >> holeselected;
		if (!(holeselected >= 1 && holeselected <= b.holes())) //if hole of invalid number
		{
			cout << "The hole number must be from 1 to " << b.holes() << endl;
			return(chooseMove(b, s)); //prompt again recursively
		}
		else if (holeselected >= 1 && holeselected <= b.holes()) //if hole number valid
		{
			if (b.beans(s, holeselected) == 0) //however hole is empty
			{
				cout << "There are no beans in that hole." << endl;
				return(chooseMove(b, s)); //prompt again recursively
			}
			else
			{
				return(holeselected); //if valid hole, return the hole selected
			}
		}
	}
	return(-1);
}

/////////////////////////////////
//Bad Player
/////////////////////////////////

BadPlayer::BadPlayer(string name) : Player(name)
{}

bool BadPlayer::isInteractive() const
{
	return(false);
}

//int BadPlayer::chooseMove(const Board& b, Side s) const //implement such that it always chooses the pot with the largest beans
//{
//	int largestnumbeans = 0;
//	for (int i = 1; i <= b.holes(); i++)
//	{
//		if (b.beans(s, i) > largestnumbeans && b.beans(s,i) != 0)
//		{
//			largestnumbeans = b.beans(s, i);
//		}
//	}
//	if (largestnumbeans == 0)
//	{
//		return(-1);
//	}
//	else
//	{
//		int result = -1;
//		for (int i = 1; i <=  b.holes(); i++)
//		{
//			if (b.beans(s, i) == largestnumbeans)
//			{
//				result = i;
//				break;
//			}
//		}
//		return(result);
//	}
//	
//}


int BadPlayer::chooseMove(const Board &b, Side s) const //for a bad player that chooses the last possible position with beans
{ 
	for (int i = b.holes(); i != 0; i--) 
	{
		if (b.beans(s, i) > 0) {
			return i;
		}
	}
	return -1;
}

///////////////////////////////////
//Smart Player
///////////////////////////////////

SmartPlayer::SmartPlayer(string name) : Player(name)
{
}

bool SmartPlayer::isInteractive() const
{
	return(false);
}

int SmartPlayer::chooseMove(const Board& b, Side s) const //implement such that it always chooses the pot with the largest beans
{
	int besthole = -1; //intialise besthole as -1
	int depth = 7; //set depth to INT_MAX
	int heuristic; //heuristic will be the best possible value that can be achieve
	AlarmClock ac(3900);  // will time out after 4.9 seconds
	smartMove(ac, b, s, besthole, heuristic, depth);
	return(besthole); 
}

void SmartPlayer::smartMove(AlarmClock& ac, const Board& b, Side s, int& besthole, int& heuristic, int depth) const
{
	//////////////////////////////////////////////////////////////////////////////
	//base cases
	/////////////////////////////////////////////////////////////////////////////

	//set south to be maximising player and north to be minimizing player
	if (b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0) //if there are no moves possible, exceute statement
	{
		if ((b.beansInPlay(NORTH) + b.beans(NORTH, 0)) > (b.beansInPlay(SOUTH) + b.beans(SOUTH, 0))) //when minimzing player wins, north greater than south
		{
			heuristic = INT_MIN; //minimizer wins
			depth = 0; //set depth to 0
			
		}
		else if ((b.beansInPlay(SOUTH) + b.beans(SOUTH, 0)) > (b.beansInPlay(NORTH) + b.beans(NORTH, 0))) //when maximizing player wins, south greater than north
		{
			heuristic = INT_MAX; //maximizer wins
			depth = 0; //set depth to 0
		}
		else //when it is a draw
		{
			heuristic = 0; //a case of draw
			depth = 0; //set depth to 0
		}
		besthole = -1; //no move possible set best hole to -1 and return
		return;
	}
	else if (depth == 0) //when reach end of search depth
	{
		heuristic = evaluate(b); //call evaluate function which takes total number of beans of south - total num beans of north
		besthole = -1; 
		return;
	}
	else if (ac.timedOut()) 
	{
		heuristic = evaluate(b); //when alarm clock times out, call eval function
		besthole = -1;
		return;
	}

	////////////////////////////////////////////////////
	//if base cases not executed
	////////////////////////////////////////////////////
	else
	{
		if (s == SOUTH) //if maximizing player
		{
			heuristic = INT_MIN; //set heruistic to the lowest possible value so it can end up getting replaced
			int best_hole = -1; //variable to just be passed into the function, not really used
			int first_possible_hole = -1; //initialise first possible hole to be -1
			int v2; //this is the value to be passed and will be compared to the global variable heuristic
			for (int i = 1; i <= b.holes(); i++) //find the hole of no choice
			{
				if (b.beans(s, i) != 0)
				{
					first_possible_hole = i; // this is neccessary so that in a losing situation, it does not mess up and choose -1 as besthole
					break;
				}
			}
			for (int i = 1; i <= b.holes(); i++)
			{
				if (b.beans(s, i) != 0) //for every possible move
				{
					Board copy(b); //create a board
					Side endside; 
					int endhole;
					copy.sow(s, i, endside, endhole); //sow the board
					if (!(complete_move(s, endside, endhole, copy))) //this check for whether move is complete, at the same time will excute the capture
					{
						smartMove(ac, copy, SOUTH, best_hole, v2, depth); //if not complete use recursion let the player move again
					}
					else
					{
						smartMove(ac, copy, NORTH, best_hole, v2, depth - 1); //now call the function on the opponent, this assumes the oppenent will try to minimize the value
					}
					if (i == first_possible_hole)
					{
						besthole = i;
					}
					if (v2 > heuristic)  //if v2 is greater than heuristic and the player does not end up losing
					{
						heuristic = v2; 
						besthole = i;
					}
					//cout << "Depth: " << INT_MAX - depth << endl;
					//cout << "Besthole: " << besthole << endl;
					//cout << "heuristic: " << v2 << endl;
				}
			}
			return;
		}
		else if (s == NORTH) //if minimizing player	
		{
			heuristic = INT_MAX; //set heruistic to the highest possible value so it can end up getting replaced
			int v2;
			int best_hole = -1; //variable to just be passed into the function, not really used
			int first_possible_hole = -1; //initialise first possible hole to be -1
			for (int i = 1; i <= b.holes(); i++) //find the hole of no choice in this case
			{
				if (b.beans(s, i) != 0)
				{
					first_possible_hole = i; //this is neccessary so that in a losing situation, it does not mess up and choose -1 as besthole
					break;
				}
			}
			for (int i = 1; i <= b.holes(); i++)
			{
				if (b.beans(s, i) != 0) //for every possible move
				{
					Board copy(b); //create a copy board
					Side endside;
					int endhole;
					copy.sow(s, i, endside, endhole); //sow the board
					if (!(complete_move(s, endside, endhole, copy))) //if not complete move
					{
						smartMove(ac, copy, NORTH, best_hole, v2, depth); //make the move again
					}
					else
					{
						smartMove(ac, copy, SOUTH, best_hole, v2, depth - 1); //change of turn	
					}
					if (i == first_possible_hole)
					{
						besthole = i;
					}
					if (v2 < heuristic) //if not gonna lose and value is better than the current value
					{
						heuristic = v2; //replace current value
						besthole = i; //set best hole to current hole in the loop
					}
				}
			}
			return;
		}
	}
}

int SmartPlayer::evaluate(Board b) const
{
	//return(b.beansInPlay(SOUTH) + b.beans(SOUTH, 0)) - (b.beansInPlay(NORTH) + b.beans(NORTH, 0));
	return(b.beans(SOUTH, 0) - b.beans(NORTH, 0) + (1) * (b.beansInPlay(SOUTH) - b.beansInPlay(NORTH)));
}


bool SmartPlayer::complete_move(Side startside, Side endside, int endhole, Board& newboard) const
{
	if (endside == startside && endhole == 0 && newboard.beansInPlay(startside) != 0)
	{
		return(false);
	}
	else if (endside == startside && newboard.beans(endside, endhole) == 1 && newboard.beans(opponent(endside), endhole) != 0)
	{
		newboard.moveToPot(NORTH, endhole, startside);
		newboard.moveToPot(SOUTH, endhole, startside);
		return(true);
	}
	else
	{
		return(true);
	}
}


