#include<iostream>
#include<vector>
#include"menu.h"
#include"networking.h"
#include"goGame.h"

using namespace std;

int main()
{
	Menu m1, m2, m3, m4;
	int mainMenuOpt = -1, hostMenuOpt = -1, joinMenuOpt = -1, boardSize;
	networkingServer *Server;
	networkingClient *Client;
	string IP;
	string s1[] = {"Host", "Join", "Exit"},
	s2[] = {"Accept", "Map size", "Back"},
	s3[] = {"Input server IP", "Back"},
	s4[] = {"Large", "Medium", "Small"};

	m1.setOptions(s1, 3);		// main menu
	m2.setOptions(s2, 3);		// host
	m3.setOptions(s3, 2);		// join
	m4.setOptions(s4, 3);		// map size

	while(mainMenuOpt != 2)
	{
		mainMenuOpt = m1.ask();

		if(mainMenuOpt == 0)
		{
			while(hostMenuOpt != 2)
			{
				hostMenuOpt = m2.ask();
				
				if(hostMenuOpt == 0)
				{
					Server = new networkingServer();	
					goGame Game(Server);

					if(boardSize == 0)
						Game.setBoardSize(19);
					else if(boardSize == 1)
						Game.setBoardSize(13);
					else
						Game.setBoardSize(9);
					
					Game.initializeBoard();

					Game.run();

					delete Server;
				}
				else if(hostMenuOpt == 1)
					boardSize = m4.ask();
			}

			hostMenuOpt = -1;
		}

		if(mainMenuOpt == 1)
		{
			while(joinMenuOpt != 1)
			{
				joinMenuOpt = m3.ask();

				if(joinMenuOpt == 0)
				{
					m3.IPAsk(IP);
					Client = new networkingClient((char*)IP.c_str());
					
					goGame Game(Client);

					Game.initializeBoard();

					Game.run();

					delete Client;
				}
			}

			joinMenuOpt = -1;
		}
	}

	return 0;
}
