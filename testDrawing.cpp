#include<iostream>
#include<vector>
#include"menu.h"
#include"goGame.h"

using namespace std;

int main()
{
	goGame game;
	game.initializeBoard(game.board, 9, game.captured_white, game.captured_black);
	game.drawBoard(game.board);
	return 0;
}
