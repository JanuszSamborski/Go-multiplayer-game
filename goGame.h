#include<iostream>
#include<vector>
#include<assert.h>
#include<unistd.h>
#include<termios.h>
#include"networking.h"

#pragma once

using namespace std;

class goGame
{

private:

	tlv tlv_cursor, tlv_stone, tlv_pass, tlv_board_size, tlv_quit, buff;
	vector<vector<int> > board;
	networkingBase *Connection;
	int board_size;
	bool pass, player_turn, is_host, game_finished = false;

	void sendCursor(pair<int,int> cursor_position)
	{
		tlv_cursor.value.x=cursor_position.first;
		tlv_cursor.value.y=cursor_position.second;
		Connection->sendMessage(tlv_cursor);
	}

	void sendStone(pair<int,int> stone_position)
	{
		tlv_stone.value.x=stone_position.first;
		tlv_stone.value.y=stone_position.second;
		Connection->sendMessage(tlv_stone);
	}

	void receiveUpdate()
	{
		buff = Connection->receiveMessage();
    	//cout<<"\nMessage: "<<buff.type<<"/"<<buff.length<<"/"<<buff.value.x<<":"<<buff.value.y<<endl;
		switch(buff.type)
		{
			case 0x1:
			cursor_position.first = buff.value.x;
			cursor_position.second = buff.value.y;
			break;
			case 0x2:
			int a[2];
			a[0]=buff.value.y;
			a[1]=buff.value.x;
      if(is_host)
			   addStone(BLACK, a);
      else
         addStone(WHITE, a);
			player_turn = true;
			break;
			case 0x3:
  			if(pass)
  			{
  				if(game_finished == false)
          {
            Connection->sendMessage(tlv_pass);
            game_finished = true;
          }
  			}
  			else
  				player_turn=true;
			  break;
			case 0x4:
			if(buff.value.x==0)
				board_size=SMALL;
			else if(buff.value.x==1)
				board_size=MEDIUM;
			else if(buff.value.x==2)
				board_size=LARGE;
			break;
		}
	}

	bool inVector(vector<pair<int,int> > &tested, pair<int,int> &test_pos)
	{
		for(int i=0; i<tested.size(); i++)
		{
			if(tested[i].first == test_pos.first && tested[i].second == test_pos.second)
			{
				return true;
			}
		}
		return false;
	}

	/*
	*	tests if stone group is to be captured, removes captured stones from board
	*
	*	\return int (0 if no stone can be captured else returns number of removed stones)
	*/
	int captureStones(int position[2])
	{
		vector<pair<int, int> > tested;
		vector<pair<int, int> > to_test;
		pair<int, int> test_pos;
		test_pos.first = position[0];
		test_pos.second = position[1];
		int color = board[position[0]][position[1]];
		bool is_captured = false;

		while(true)
		{
			if(test_pos.first >= 0 && test_pos.first < board.size() && test_pos.second >= 0 && test_pos.second < board.size())                       //check if test_pos inside playfield
			{
				//test adjacent fields, if the same color add to to_test
				tested.push_back(test_pos);                                               //add to already tested
				pair<int,int> temp_pos;
				int iterate_array[4][2]={{-1,0},{0,1},{1,0},{0,-1}};                      //iterate through neighbours

				for(int i=0; i < 4; i++)
				{
					temp_pos.first=test_pos.first+iterate_array[i][0];
					temp_pos.second=test_pos.second+iterate_array[i][1];

					if(temp_pos.first>=0 && temp_pos.first<board.size() && temp_pos.second>=0 && temp_pos.second<board.size())                 //check if inside playfield
					{
						if(board[temp_pos.first][temp_pos.second]==EMPTY)                   //check if field contains stone
						{
							return 0;
							break;
						}
						else if(board[temp_pos.first][temp_pos.second]==color)              //check if field contains stone the same color as selected
						{
							bool in_list = false;

							for (size_t i = 0; i<tested.size(); i++)
							{
								if(tested[i].first == temp_pos.first && tested[i].second == temp_pos.second)
								{
									in_list=true;
									break;
								}
							}

							if(in_list)
							{
								continue;
							}
							else
							{
								to_test.push_back(temp_pos);
								continue;
							}
						}
						else if (board[temp_pos.first][temp_pos.second]==-color)
						{
							continue;
						}
					}
					else
						continue;
				}
			}
			else
				return 0;

			if (to_test.size()!=0)
			{
				test_pos=to_test.back();
				to_test.pop_back();
			}
			else
			{
				is_captured=true;
				break;
			}
		}

		int captured = tested.size();

		for(int i=0; i<captured; i++)
		{
			board[tested.back().first][tested.back().second] = 0;
			tested.pop_back();
		}

		return captured;
	}

	void placeCursor(int x, int y)
	{
		cout<<"\033[" + to_string(y) + ";" + to_string(x) + "H";
	}

	void centerText(string s, int row)
	{
		int columns, where;
		struct winsize size;

		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

		columns = size.ws_col + 1;

		where = columns/2 - s.size()/2;

		placeCursor(where, row);

		cout<<s;
	}

public:

	int captured_white = 0;
	int captured_black = 0;
	int points_black = 0;
	int points_white = 0;
	pair<int, int> cursor_position = make_pair(0,0);
	int local_player_color = WHITE;

	void setBoardSize(int boardSize)
	{
		board_size = boardSize;
	}

	goGame(networkingBase *conn)
	{
		is_host = false;
		player_turn = false;
		Connection = conn;

		tlv_cursor.type	   = 0x1;
		tlv_cursor.length  = 2;
		tlv_cursor.value.x = 0;
		tlv_cursor.value.y = 0;

		tlv_stone.type	  = 0x2;
		tlv_stone.length  = 2;
		tlv_stone.value.x = 0;
		tlv_stone.value.y = 0;

		tlv_pass.type	= 0x3;
		tlv_pass.length = 0;

		tlv_board_size.type	   = 0x4;
		tlv_board_size.length  = 1;
		tlv_board_size.value.x = 0;

		tlv_quit.type	= 0x5;
		tlv_quit.length = 0;

		pass = false;

		if(Connection -> isServer())
			is_host = true;

		syslog(LOG_INFO, "Initialized goGame board");
	}

	enum fieldValues
	{
		WHITE = 1,
		BLACK = -1,
		EMPTY = 0
	};

	enum boardSizeList
	{
		SMALL  = 9,
		MEDIUM = 13,
		LARGE  = 19
	};

	int getch()
	{
		int c = 0;

		struct termios org_opts, new_opts;
		int res = 0;

		res = tcgetattr(STDIN_FILENO, &org_opts);
		assert(res == 0);

		memcpy(&new_opts, &org_opts, sizeof(new_opts));
		new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
		tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
		c = getchar();

		res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
		assert(res == 0);
		return(c);
	}

	int initializeBoard()
	{
		if(is_host)
		{
			local_player_color = WHITE;
			if(board_size==SMALL)
				tlv_board_size.value.x=0;
			else if(board_size==MEDIUM)
				tlv_board_size.value.x=1;
			else if(board_size==LARGE)
				tlv_board_size.value.x=2;
			Connection->sendMessage(tlv_board_size);
			player_turn=true;
		}
		else
		{
			local_player_color = BLACK;
			player_turn=false;
			receiveUpdate();
		}

		board.clear();
		for(int y=0; y<board_size; y++)
		{
			board.push_back(vector<int>());
			for(int x=0; x<board_size; x++)
				board[y].push_back(0);
		}
		captured_black = 0;
		captured_white = 0;
		return 0;
	}

	void countPoints()
	{
		points_black = captured_white;
		points_white = captured_black;
		vector<pair<int,int>> tested;

		for(size_t pos_y=0; pos_y<board.size(); pos_y++)
			for(size_t pos_x=0; pos_x<board.size(); pos_x++)
			{
				/*---if stone on current position add it to final score---*/
				switch (board[pos_y][pos_x])
				{
					case WHITE:
					points_white++;
					break;
					case BLACK:
					points_black++;
					break;
				}

				/*---check if field already tested---*/
				pair<int,int> current_pos = make_pair(pos_y, pos_x);
				if(inVector(tested, current_pos) == true)
					continue;
				else
				{
					if(board[current_pos.first][current_pos.second] == EMPTY)
					{
						vector<pair<int,int>> to_test;
						to_test.push_back(current_pos);
						int empty_count=0;
						int current_color = EMPTY;
						int to_count = true;

						while(to_test.empty()==false)
						{
							pair<int,int> test_pos = to_test.back();
							to_test.pop_back();
							tested.push_back(test_pos);
							empty_count++;

							int iterate_array[4][2]={{-1,0},{0,1},{1,0},{0,-1}};
							for(int i=0; i<4; i++)
							{
								pair<int,int> temp_pos = make_pair(test_pos.first+iterate_array[i][0], test_pos.second+iterate_array[i][1]);
								if(temp_pos.first>=0 && temp_pos.first<board.size() && temp_pos.second>=0 && temp_pos.second<board.size())
								{
									if(board[temp_pos.first][temp_pos.second] == EMPTY)
									{
										if(inVector(tested, temp_pos) == false && inVector(to_test, temp_pos) == false)
											to_test.push_back(temp_pos);
										else
											continue;
									}
									else
									{
										if(current_color == EMPTY)
										{
											current_color = board[temp_pos.first][temp_pos.second];
											if(inVector(tested, temp_pos) == false)
												tested.push_back(temp_pos);
										}
										else
										{
											if(board[temp_pos.first][temp_pos.second] == current_color)
											{
												if(inVector(tested, temp_pos) == false)
													tested.push_back(temp_pos);
											}
											else
											{
												to_count = false;
												if(inVector(tested, temp_pos) == false)
													tested.push_back(temp_pos);
											}
										}
									}
								}
							}
						}
						if(to_count == true)
						{
							switch (current_color)
							{
								case WHITE:
								points_white += empty_count;
								break;
								case BLACK:
								points_black += empty_count;
								break;
							}
						}
					}
					else
						continue;
				}
			}
	}

	int drawBoard()
	{
		system("clear");
		cout<<endl;
		if(game_finished)
		{
			system("clear");
			countPoints();
			centerText("Final score white: " + to_string(points_white) + " points", 3);
			centerText("Final score black: " + to_string(points_black) + " points", 4);
		}
		else
		{
			int board_size = board.size();
			char buffer[board_size*2][board_size];
			for(size_t y=0; y<board_size; y++)
			{
				for(size_t x=0; x<board_size; x++)
				{
					switch (board[y][x])
					{
						case EMPTY:
						{
							if(y==0 && x==0)
								buffer[x*2][y]=(char)(0xDA);
							else if(y==0 && x==board_size-1)
								buffer[x*2][y]=(char)(0xBF);
							else if(y==board_size-1 && x==0)
								buffer[x*2][y]=(char)(0xC0);
							else if(y==board_size-1 && x==board_size-1)
								buffer[x*2][y]=(char)(0xD9);
							else if(y==0)
								buffer[x*2][y]=(char)(0xC2);
							else if(y==board_size-1)
								buffer[x*2][y]=(char)(0xC1);
							else if(x==0)
								buffer[x*2][y]=(char)(0xC3);
							else if(x==board_size-1)
								buffer[x*2][y]=(char)(0xB4);
							else
								buffer[x*2][y]=(char)(0xC5);
							break;
						}
						case WHITE:
						buffer[x*2][y]='O';
						break;
						case BLACK:
						buffer[x*2][y]='0';
						break;
					}
					if(x<board_size-1)
						buffer[x*2+1][y]=(char)(0xC4);
				}
				buffer[board_size*2-1][y]='\n';
			}

			for(int y=0; y<board_size; y++)
				for(int x=0; x<board_size*2; x++)
				{
					cout<<"\e[?25l";
					if(y==cursor_position.second && x==cursor_position.first*2)
					{
						if(player_turn)
						{
							cout<<"\e[107m\e[30m";
							cout<<buffer[x][y];
							cout<<"\e[0m";
						}
						else
						{
							cout<<"\e[41m";
							cout<<buffer[x][y];
							cout<<"\e[0m";
						}
					}
					else
					{
						cout<<"\e[100m";
						if(buffer[x][y] == '0')
						{
							cout<<"\e[30m";
							cout<<buffer[x][y];

						}
						else if(buffer[x][y] == 'O')
						{
							cout<<"\e[97m";
							cout<<buffer[x][y];
						}
						else
						{
							cout<<"\e[37m";
							cout<<buffer[x][y];
						}
						cout<<"\e[0m";
					}
				}
				cout<<"Captured black stones: " + to_string(captured_black) +
				"\nCaptured white stones: " + to_string(captured_white);
				if(player_turn)
					cout<<"\n\nIt's your turn, place your stone\n";
				else
					cout<<"\n\nIt's opponent's turn, wait for your turn\n";

				cout<<"\nInstructions:\nPress \e[31marrow keys\e[0m to select intersection\nPress \e[31mP\e[0m to pass turn\n"
					<<"Press \e[31mENTER\e[0m to select intersection\n";
	    }

		return 0;
	}

	bool addStone(int color, int position[2])
	{
		if(board[position[0]][position[1]]==EMPTY)
		{
			board[position[0]][position[1]]=color;
			bool surrounded=true;
			int captured=0;
			int iterate_array[4][2]={{-1,0},{0,1},{1,0},{0,-1}};
			for(int i=0; i<4; i++)
			{
				int temp_pos[2];
				temp_pos[0]=position[0]+iterate_array[i][0];
				temp_pos[1]=position[1]+iterate_array[i][1];
				if(temp_pos[0]>=0 && temp_pos[1]>=0 && \
					temp_pos[0]<board.size() && temp_pos[1]<board.size())
				{
					if(board[temp_pos[0]][temp_pos[1]] == -color)
						captured+=captureStones(temp_pos);
					else
						surrounded = false;
				}
			}
			if (captured>0)
			{
				if(color == BLACK)
					captured_white += captured;
				else if(color == WHITE)
					captured_black += captured;

			}
			else if(surrounded == true)
			{
				board[position[0]][position[1]]=EMPTY;
			}
			return true;
		}
		else
			return false;
	}

	void getInput()
	{
    	int g;
		if(player_turn)
		{
		g = getch();
  		if(g == 27)
  		{
  			getch();
  			g = getch();
  		}

			if(g==65)
			{
				if(cursor_position.second>0)
				{
					cursor_position.second--;
					sendCursor(cursor_position);
				}
			}
			else if(g==66)
			{
				if(cursor_position.second<board_size-1)
				{
					cursor_position.second++;
					sendCursor(cursor_position);
				}
			}
			else if(g==67)
			{
				if(cursor_position.first<board_size-1)
				{
					cursor_position.first++;
					sendCursor(cursor_position);
				}
			}
			else if(g==68)
			{
				if(cursor_position.first>0)
				{
					cursor_position.first--;
					sendCursor(cursor_position);
				}
			}
			else if(g==10)
			{
				int a[2];
				a[0]=cursor_position.second;
				a[1]=cursor_position.first;

				if(addStone(local_player_color, a))
				{
					sendStone(cursor_position);
					player_turn=false;
				}
			}
			else if(g==112)
			{
				Connection -> sendMessage(tlv_pass);
				pass = true;
				player_turn = false;
			}
		}
		else
		{
			receiveUpdate();
		}
	}

	void run()
	{
		sleep(0.5);

		while(game_finished == false)
		{
			drawBoard();
			getInput();
		}

		drawBoard();
		cout<<"\033[1;32m";
		centerText("[Press any key to continue]", 5);
		cout<<"\033[0m";
		getchar();

		syslog(LOG_INFO, "goGame session ended successfully");
	}
};
