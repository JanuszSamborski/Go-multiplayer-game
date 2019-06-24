#ifndef goGame
#define goGame
#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<utility>
#include<algorithm>
#include<stdlib.h>
using namespace std;

class logic
{
public:

  int board_size;
  int captured_white=0;
  int captured_black=0;
  int points_black=0;
  int points_white=0;
  pair<int,int> cursor_position=make_pair(0,0);
  vector<vector<int>> board;

  enum fieldValues
  {
    WHITE = 1,
    BLACK = -1,
    EMPTY = 0
  };

  int initializeBoard(vector<vector<int>> &board, int board_size, int &captured_white, int &captured_black)
  {
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

  void countPoints(vector<vector<int>> &board, int &points_black, int &points_white, int &captured_black, int &captured_white)
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
              printf("add to white %d\n", empty_count);
              points_white += empty_count;
              break;
              case BLACK:
              printf("add to black\n");
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

  int drawBoard(vector<vector<int>> &board, pair<int,int> cursor, bool cursor_player)
  {
  system("clear");
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
          buffer[x*2][y]='W';
          break;
        case BLACK:
          buffer[x*2][y]='B';
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
      cout<<"\e[1m";
      if(y==cursor.second && x==cursor.first*2)
      {
        if(cursor_player)
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
        cout<<buffer[x][y];
        cout<<"\e[0m";
      }
    }
  return 0;
}

  bool addStone(vector<vector<int>> &board, int color, int position[2], int &captured_black, int &captured_white)
  {
  if(board[position[0]][position[1]]==EMPTY)
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
          captured+=captureStones(board, temp_pos);
        }
    }
    if (captured>0)
      if(color == BLACK)
        captured_white += captured;
      else if(color == WHITE)
      {
        captured_black += captured;
      }
      return true;
    }
    else
      return false;
}

  void drawScreen(pair<int,int> &cursor_position, bool cursor_player)
  {
    int g = getch();
		if(g == 27)
		{
			getch();
			g = getch();
		}
    cout<<g<<endl;
    if(g==65)
    {
      if(cursor_position.second>0)
      {
        cursor_position.second--;
        drawBoard(board, cursor_position, cursor_player);
      }
    }
    else if(g==66)
    {
      if(cursor_position.second<board_size-1)
      {
        cursor_position.second++;
        drawBoard(board, cursor_position, cursor_player);
      }
    }
    else if(g==67)
    {
      if(cursor_position.first<board_size-1)
      {
        cursor_position.first++;
        drawBoard(board, cursor_position, cursor_player);
      }
    }
    else if(g==68)
    {
      if(cursor_position.first>0)
      {
        cursor_position.first--;
        drawBoard(board, cursor_position, cursor_player);
      }
    }
    else if(g==10)
    {
      int a[2];
      a[0]=cursor_position.second;
      a[1]=cursor_position.first;
      addStone(board, WHITE, a, captured_black, captured_white);
      drawBoard(board, cursor_position, cursor_player);
    }

  }

private:
  bool inVector(vector<pair<int,int>> &tested, pair<int,int> &test_pos)
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

  int captureStones(vector<vector<int>> &board, int position[2])
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
        test_pos.second>=0 && test_pos.second<board.size())                       //check if test_pos inside playfield
      {
                                                                                  //test adjacent fields, if the same color add to to_test
        tested.push_back(test_pos);                                               //add to already tested
        pair<int,int> temp_pos;
        int iterate_array[4][2]={{-1,0},{0,1},{1,0},{0,-1}};                      //iterate through neighbours
        for(int i=0; i<4; i++)
          {
            temp_pos.first=test_pos.first+iterate_array[i][0];
            temp_pos.second=test_pos.second+iterate_array[i][1];
            if(temp_pos.first>=0 && temp_pos.first<board.size() &&
              temp_pos.second>=0 && temp_pos.second<board.size())                 //check if inside playfield
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

};

#endif
