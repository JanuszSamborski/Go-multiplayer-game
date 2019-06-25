#include<iostream>
#include<vector>
#include<utility>
#include"menu.h"
#include"goGame.h"

using namespace std;

int main()
{
	logic game;
	game.board_size = 19;
	game.local_player_color = game.BLACK;
	game.initializeBoard(game.board, game.board_size, game.captured_white, game.captured_black);
	int b[8][2]={{1,1},{1,2},{1,3},{2,1},{2,3},{3,1},{3,2},{3,3}};
	for(int i=0; i<8; i++)
		game.addStone(game.board, game.WHITE, b[i], game.captured_black, game.captured_white);
	game.drawBoard(game.board, make_pair(0,0), game.cursor_player, game.captured_black, game.captured_white);
	while(true)
	{
		game.cursor_player = true;
		game.getInput(game.cursor_position, game.cursor_player, game.captured_black, game.captured_white);
	}
	return 0;
}
