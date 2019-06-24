#include<iostream>
#include<vector>
#include"menu.h"
#include"goGame.h"

using namespace std;

int main()
{
<<<<<<< HEAD
	menu m1, m2, m3, m4;
	int mainMenuOpt = -1, hostMenuOpt = -1, joinMenuOpt = -1, boardSize;
	string IP;
=======
	/*menu m1, m2, m3, m4;
	int x = -1, y = -1, z = -1;
>>>>>>> c088961988f5ba3ca78697d5c5ec331541487f5b
	string s1[] = {"Host", "Join", "Exit"},
	s2[] = {"Accept", "Map size", "Back"},
	s3[] = {"Input server IP", "Back"},
	s4[] = {"Large", "Medium", "Small"};

	m1.setOptions(s1, 3);		// main menu
	m2.setOptions(s2, 3);		// host
	m3.setOptions(s3, 2);		// join
	m4.setOptions(s4, 3);		// map size

	cout<<"\033[2J\033[1;1H";		// initial clearscreen
	cout<<"\e[?25l";				// hide cursor

	while(x != 2)
	{
		x = m1.ask();

<<<<<<< HEAD
		if(mainMenuOpt == 0)
		{
			while(hostMenuOpt != 2)
=======
		if(x == 1)
			while(y != 2)
>>>>>>> c088961988f5ba3ca78697d5c5ec331541487f5b
			{
				y = m2.ask();

				if(y == 1)
					board_size = m4.ask();
			}

<<<<<<< HEAD
			hostMenuOpt = -1;
		}

		if(mainMenuOpt == 1)
		{
			while(joinMenuOpt != 1)
			{
				joinMenuOpt = m3.ask();

				if(joinMenuOpt == 0)
					IP = m3.IPAsk();
			}

			joinMenuOpt = -1;
		}
	}

=======
		if(x == 0)
			while(z != 1)
				z = m3.ask();
	}
*/
>>>>>>> c088961988f5ba3ca78697d5c5ec331541487f5b
	goGame::goGame game;
	game.initializeBoard(game.board, 9, game.captured_white, game.captured_black);
	game.drawBoard(game.board);
	return 0;
}
