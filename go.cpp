#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<utility>
#include<algorithm>

using namespace std;

#define WHITE 1
#define BLACK -1
#define EMPTY 0

int Initialize_board(vector<vector<int>> &board, int board_size)
{
  for(int y=0; y<board_size; y++)
  {
    board.push_back(vector<int>());
      for(int x=0; x<board_size; x++)
        board[y].push_back(0);
  }
  return 0;
}


int Capture(vector<vector<int>> &board, int position[2])
{
  /*tests if stone group is to be captured, removes captured stones from board
  returns 0 if no stone can be captured else returns number of removed stones*/
  vector<pair<int,int>> tested;
  vector<pair<int,int>> to_test;
  pair<int,int> test_pos;
  test_pos.first = position[0];
  test_pos.second = position[1];
  int color = board[position[0]][position[1]];
  bool is_captured=false;

  while(true)
  {
    if(test_pos.first>=0 && test_pos.first<board.size() &&
      test_pos.second>=0 && test_pos.second<board.size())                       //check if inside playfield
    {
                                                                                //test adjacent fields, if the same color add to to_test
      tested.push_back(test_pos);
      pair<int,int> temp_pos;
      int iterate_array[4][2]={{-1,0},{0,1},{1,0},{0,-1}};
      for(int i=0; i<4; i++)
        {
          temp_pos.first=test_pos.first+iterate_array[i][0];
          temp_pos.second=test_pos.second+iterate_array[i][1];
          if(temp_pos.first>=0 && temp_pos.first<board.size() &&
            temp_pos.second>=0 && temp_pos.second<board.size())
          {
            //printf("%s%d\n", "board value: ", board[temp_pos.first][temp_pos.second]);
            if(board[temp_pos.first][temp_pos.second]==EMPTY)
            {
              return 0;
              break;
            }
            else if(board[temp_pos.first][temp_pos.second]==color)
            {
              bool in_list = false;
              for (size_t i = 0; i<tested.size(); i++)
                if(tested[i].first == temp_pos.first && tested[i].second == temp_pos.second)
                {
                  in_list=true;
                  break;
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
        return -1;

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

int Draw_board(vector<vector<int>> &board)
{
  int board_size = board.size();
  char buffer[board_size+1];

  for(size_t y=0; y<board_size; y++)
  {
    for(size_t x=0; x<board_size; x++)
      switch (board[y][x])
      {
        case EMPTY:
          buffer[x]='+';
        break;
        case WHITE:
          buffer[x]='W';
          break;
        case BLACK:
          buffer[x]='B';
          break;
      }
      buffer[board_size]='\0';
      printf("%s\n", buffer);
    }
  return 0;
}

int Add_stone(vector<vector<int>> &board, int color, int position[2])
{
  board[position[0]][position[1]]=color;
  int captured=0;
  int iterate_array[4][2]={{-1,0},{0,1},{1,0},{0,-1}};
  for(int i=0; i<4; i++)
  {
    int temp_pos[2];
    temp_pos[0]=position[0]+iterate_array[i][0];
    temp_pos[1]=position[1]+iterate_array[i][1];
    if(temp_pos[0]>=0 && temp_pos[1]>=0 &&
      temp_pos[0]<board.size() && temp_pos[1]<board.size())
    {
      if(board[temp_pos[0]][temp_pos[1]] == -color)
        captured+=Capture(board, temp_pos);
    }
  }
  if (captured>0)
    return captured;
  else
    {
      captured-=Capture(board,position);
      return captured;
    }
}

int main()
{
  int board_size=9;
  int captured_white=0;
  int captured_black=0;
  vector<vector<int>> board;
  Initialize_board(board, board_size);
  board[0][1]=WHITE;
  board[2][0]=WHITE;
  //board[1][1]=WHITE;
  board[1][0]=BLACK;
  board[0][0]=BLACK;
  Draw_board(board);
  int pos[2]={1,1};
  captured_black+=Add_stone(board, WHITE, pos);
  printf("%s\n", "---------------------------");
  Draw_board(board);
  printf("%d\n", captured_black);
  return 0;
}
