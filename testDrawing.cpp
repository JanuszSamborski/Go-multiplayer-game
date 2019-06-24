#include<iostream>
#include<vector>
#include"menu.h"
#include"goGame.h"

using namespace std;

int main()
{
	logic game;
	game.initializeBoard(game.board, 9, game.captured_white, game.captured_black);
	int a[]={0,0};
	int b[]={0,1};
	game.addStone(game.board, game.BLACK, a, game.captured_black, game.captured_white);
	game.addStone(game.board, game.WHITE, b, game.captured_black, game.captured_white);
	game.drawBoard(game.board);
	return 0;
}
