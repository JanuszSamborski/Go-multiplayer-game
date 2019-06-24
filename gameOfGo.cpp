#include<iostream>
#include<vector>
#include"menu.h"
#include"GO_LOGIC_H.h"

using namespace std;

int main()
{
	int board_size;
  int captured_white=0;
  int captured_black=0;
  int points_black=0;
  int points_white=0;
  vector<vector<int>> board;


	menu m1, m2, m3, m4;
	int x = -1, y = -1, z = -1;
	string s1[] = {"Host", "Join", "Exit"},
	s2[] = {"Accept", "Map size", "Back"},
	s3[] = {"Input server IP", "Back"},
	s4[] = {"Large", "Medium", "Small"};

	m1.setOptions(s1, 3);
	m2.setOptions(s2, 3);
	m3.setOptions(s3, 2);
	m4.setOptions(s4, 3);

	cout<<"\033[2J\033[1;1H";		// initial clearscreen
	cout<<"\e[?25l";				// hide cursor

	while(x != 2)
	{
		x = m1.ask();

		if(x == 1)
			while(y != 2)
			{
				y = m2.ask();

				if(y == 1)
					board_size = m4.ask();
			}

		if(x == 0)
			while(z != 1)
				z = m3.ask();
	}

	return 0;
}
