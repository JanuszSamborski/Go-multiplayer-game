#include<iostream>
#include<vector>
#include<utility>
#include"menu.h"
#include"goGame.h"

using namespace std;

int main()
{
	logic game;
	game.board_size = 9;
	game.initializeBoard(game.board, game.board_size, game.captured_white, game.captured_black);
	int a[]={0,0};
	int b[]={0,1};
	game.addStone(game.board, game.BLACK, a, game.captured_black, game.captured_white);
	game.addStone(game.board, game.WHITE, b, game.captured_black, game.captured_white);
	game.drawBoard(game.board, make_pair(0,0),true);
	while(true)
	{
		game.drawScreen(game.cursor_position, true);
	}
	return 0;
}
