#include<iostream>
#include"networking.h"
#include"goGame.h"

using namespace std;

void sendReceiveTest()
{
	networkingClient Client((char*)"2001:db8:0:f101::1");

	tlv test, test2;
	
	test.type 	 = 0x1;
	test.length  = 2;
	test.value.x = 10;
	test.value.y = 20;

	Client.sendMessage(test);

	test2 = Client.receiveMessage();
	
	cout<<"type: " + to_string(test2.type)<<endl
		<<"length: " + to_string(test2.length)<<endl
		<<"value 1: " + to_string(test2.value.x)<<endl
		<<"value 2: " + to_string(test2.value.y)<<endl;

	test2 = Client.receiveMessage();
	
	cout<<"type: " + to_string(test2.type)<<endl
		<<"length: " + to_string(test2.length)<<endl
		<<"value 1: " + to_string(test2.value.x)<<endl
		<<"value 2: " + to_string(test2.value.y)<<endl;
}

int main()
{
	networkingClient Client((char*)"2001:db8:0:f101::1");
	goGame Game(&Client);

	Game.initializeBoard(Game.board, Game.board_size, Game.captured_white, Game.captured_black, true, Game.player_turn);

	while(true)
	{
		Game.getInput(Game.cursor_position, Game.player_turn, Game.captured_black, Game.captured_white, true);
	}

	return 0;
}
