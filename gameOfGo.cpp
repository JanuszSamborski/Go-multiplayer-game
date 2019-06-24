#include<iostream>
#include<vector>
#include"menu.h"
//#include"GO_LOGIC_H.h"

using namespace std;

int main()
{
	menu m1, m2, m3, m4;
	int mainMenuOpt = -1, hostMenuOpt = -1, joinMenuOpt = -1, boardSize;
	string s1[] = {"Host", "Join", "Exit"},
	s2[] = {"Accept", "Map size", "Back"},
	s3[] = {"Input server IP", "Back"},
	s4[] = {"Large", "Medium", "Small"};

	m1.setOptions(s1, 3);
	m2.setOptions(s2, 3);
	m3.setOptions(s3, 2);
	m4.setOptions(s4, 3);

	while(mainMenuOpt != 2)
	{
		mainMenuOpt = m1.ask();

		if(mainMenuOpt == 1)
		{
			while(hostMenuOpt != 2)
			{
				hostMenuOpt = m2.ask();

				if(hostMenuOpt == 1)
					boardSize = m4.ask();
			}

			hostMenuOpt = -1;
		}

		if(mainMenuOpt == 0)
		{
			while(joinMenuOpt != 1)
				joinMenuOpt = m3.ask();

			joinMenuOpt = -1;
		}
	}

	return 0;
}
