#include<iostream>
#include"networking.h"
#include"goGame.h"
using namespace std;

int main()
{
	networkingServer Server;
	logic Game;

	Game.board_size = 19;
	Game.initializeBoard(Game.board, Game.board_size, Game.captured_white, Game.captured_black, true, Game.player_turn);
	while(true)
	{
		Game.getInput(Game.cursor_position, Game.player_turn, Game.captured_black, Game.captured_white, true);
	}
	/*
	tlv test,test2;


	test = Server.receiveMessage();
	cout<<"type: " + to_string(test.type)<<endl
		<<"length: " + to_string(test.length)<<endl
		<<"value 1: " + to_string(test.value.x)<<endl
		<<"value 2: " + to_string(test.value.y)<<endl;


		test2.type=0x1;
		test2.length=2;
		test2.value.x=10;
		test2.value.y=30;

		Server.sendMessage(test2);
		*/

	return 0;
}
