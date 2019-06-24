#ifndef GO_LOGIC_H
#define GO_LOGIC_H
#include <vector>
using namespace std;

class logic
{
public:
  int initializeBoard(vector<vector<int>> &board, int board_size);
  void addStone(vector<vector<int>> &board, int color, int position[2], int &captured_black, int &captured_white);
  void countPoints(vector<vector<int>> &board, int &points_black, int &points_white, int &captured_black, int &captured_white);

  enum fieldValues
  {
    WHITE = 1,
    BLACK = -1,
    EMPTY = 0
  };

private:
  int captureStones(vector<vector<int>> &board, int position[2]);
  bool inVector(vector<pair<int,int>> &tested, pair<int,int> &test_pos);

};

#endif
