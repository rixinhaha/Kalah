///////////////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to continue . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif
////////////////////////////////////////////////////////////////////////////////////////////
#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

void doBoardTests()
{
	Board b(3, 2);
	assert(b.holes() == 3 && b.totalBeans() == 12 &&
		b.beans(SOUTH, POT) == 0 && b.beansInPlay(SOUTH) == 6);
	b.setBeans(SOUTH, 1, 1);
	b.moveToPot(SOUTH, 2, SOUTH);
	assert(b.totalBeans() == 11 && b.beans(SOUTH, 1) == 1 &&
		b.beans(SOUTH, 2) == 0 && b.beans(SOUTH, POT) == 2 &&
		b.beansInPlay(SOUTH) == 3);
	Side es;
	int eh;
	b.sow(SOUTH, 3, es, eh);
	assert(es == NORTH && eh == 3 && b.beans(SOUTH, 3) == 0 &&
		b.beans(NORTH, 3) == 3 && b.beans(SOUTH, POT) == 3 &&
		b.beansInPlay(SOUTH) == 1 && b.beansInPlay(NORTH) == 7);
}

void extraBoardtests()
{
	Board test(3, 2);

	Side endside;
	int endhole;

	//test for invalid sow
	test.setBeans(NORTH, 1, 0);
	assert(test.sow(NORTH, 1, endside, endhole) == false); //invalid sow

	//test for skip south pot when player is north
	test.setBeans(NORTH, 1, 5);
	assert(test.sow(NORTH, 1, endside, endhole) == true); //invalid sow
	assert(test.beans(SOUTH, 0) == 0 && endside == NORTH && endhole == 3);

	//test for skip north pot when player is south
	test.setBeans(SOUTH, 3, 5);
	test.setBeans(NORTH, 0, 0);
	assert(test.sow(SOUTH, 3, endside, endhole) == true); //invalid sow
	assert(test.beans(NORTH, 0) == 0 && endside == SOUTH && endhole == 1);
	assert(test.beansInPlay(NORTH) == 8 && test.beansInPlay(SOUTH) == 7);
	assert(test.totalBeans() == 16);

	//test for beans in play and total beans
	Board test1(3, 1);
	test1.sow(NORTH, 1, endside, endhole);
	assert(test1.beansInPlay(NORTH) == 2);

	////copy constructor and assignment operator
	Board a(3, 2);
	Board lala(2, 1);
	a = lala;
	assert(a.totalBeans() == 4 && a.beans(NORTH, 1) == 1);
	Board q(a);
	assert(q.totalBeans() == 4 && q.beans(NORTH, 1) == 1);

	cerr << "Passed" << endl;
}

void doGameTests() //this already tests for capture moves and when game is over
{
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Board b(3, 0);
	b.setBeans(SOUTH, 1, 2);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 2);
	Game g(b, &bp1, &bp2);
	bool over;
	bool hasWinner;
	Side winner;
	g.display();
	//    Homer
	//   0  1  2
	// 0         0
	//   2  0  0
	//    Bart
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
		g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

	g.move();
	//   0  1  0
	// 0         3
	//   0  1  0

	//g.display();
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move();
	//   1  0  0
	// 0         3
	//   0  1  0
	//g.display();
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move();
	//   1  0  0
	// 0         3
	//   0  0  1
	g.status(over, hasWinner, winner);
	//g.display();
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

	g.move();
	//   0  0  0
	// 1         4
	//   0  0  0
	g.status(over, hasWinner, winner);
	//g.display();
	assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
	assert(hasWinner && winner == SOUTH);
}

void additionalGameTests() //test for repeated turns for both north and south players
{
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Board b(3, 0);
	b.setBeans(SOUTH, 1, 1);
	b.setBeans(SOUTH, 3, 1);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 2);
	Game g(b, &bp1, &bp2);
	cout << "first test" << endl;
	g.move();
	//check that south player Bart moves first and gets a repeated move
	cout << "first test passed" << endl;
	cout << endl;


	Board b2(3, 0);
	b2.setBeans(SOUTH, 2, 1);
	b2.setBeans(NORTH, 2, 2);
	b2.setBeans(NORTH, 1, 1);
	
	//////////////////////
	//  1 2 0
	//0       0
	//  0 1 0
	//////////////////////
	Game f(b2, &bp1, &bp2);
	cout << "second test" << endl;
	f.move();
	f.move();
	//check that north player Homer moves second and gets a repeated move
	cout << "second test passed" << endl;
}

void doPlayerTests()
{
	HumanPlayer hp("Marge");
	assert(hp.name() == "Marge"  &&  hp.isInteractive());
	BadPlayer bp("Homer");
	assert(bp.name() == "Homer" && !bp.isInteractive());
	SmartPlayer sp("Lisa");
	assert(sp.name() == "Lisa" && !sp.isInteractive());
	Board b(3, 2);
	b.setBeans(SOUTH, 2, 0);
	cout << "=========" << endl;
	int n = hp.chooseMove(b, SOUTH);
	cout << "=========" << endl;
	assert(n == 1 || n == 3);
	n = bp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
	n = sp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
}

//int main()
//{
//	additionalGameTests();
//}


//int main()
//{
//	doGameTests();
//	cout << "Passed all tests" << endl;
//}

int main()
{
	SmartPlayer hp("Marge");
	BadPlayer bp("Homer");
	Board b(6, 3);
	Game g(b, &hp, &bp);
	g.play();
}

//int main()
//{
//	HumanPlayer hp("Bart");
//	BadPlayer bp("Homer");
//	Board b(3, 2);
//	//for (int i = 1; i <= b.holes(); i++)
//	//{
//	//	b.setBeans(SOUTH, i, 0);
//	//}
//	Game g(b, &hp, &bp);
//	g.play();
//}

//int main()
//{
//	BadPlayer hp("Marge");
//	SmartPlayer bp("Homer");
//	Board b(5, 2);
//	//for (int i = 1; i <= b.holes() - 2; i++)
//	//{
//	//	b.setBeans(SOUTH, i, 0);
//	//}
//	//b.setBeans(SOUTH, b.holes()-1, 1);
//	//b.setBeans(SOUTH, b.holes(), 1);
//
//	Game g(b, &hp, &bp);
//	g.play();
//}

//int main()
//{
//	doPlayerTests();
//	cout << "Passed all tests" << endl;
//}


//int main()
//{
//	extraBoardtests();
//}