#include<iostream>
#include<vector>
#include<syslog.h>
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

	setlogmask(LOG_UPTO(LOG_NOTICE));

	openlog("gameOfGo", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);

	syslog(LOG_NOTICE, "Program started by User %d", getuid());

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
					try { Server = new networkingServer(); }
					catch(logic_error& exc) { syslog(LOG_ERR, exc.what()); continue; }

					goGame Game(Server);

					if(boardSize == 0)
						Game.setBoardSize(19);
					else if(boardSize == 1)
						Game.setBoardSize(13);
					else
						Game.setBoardSize(9);
					
					Game.initializeBoard();

					try { Game.run(); }
					catch(logic_error& exc) { syslog(LOG_ERR, exc.what()); }

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
					try { Client = new networkingClient((char*)IP.c_str()); }
					catch(logic_error& exc) { syslog(LOG_ERR, exc.what()); continue; }
					
					goGame Game(Client);

					Game.initializeBoard();

					try { Game.run(); }
					catch(logic_error& exc) { syslog(LOG_ERR, exc.what()); }

					delete Client;
				}
			}

			joinMenuOpt = -1;
		}
	}

	syslog(LOG_NOTICE, "Program ended successfully");
	closelog();

	return 0;
}
