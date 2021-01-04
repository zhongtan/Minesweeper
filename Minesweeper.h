#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <vector>
#include <cstdlib>
#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include <iostream>
#include <iomanip>

#define INDENT_SPACE_WIDTH 3

#define BEGINNER_ROW_DIM    8
#define BEGINNER_COL_DIM    8
#define BEGINNER_NUM_MINES  10

#define INTERM_ROW_DIM      16
#define INTERM_COL_DIM      16
#define INTERM_NUM_MINES    40

#define EXPERT_ROW_DIM      16
#define EXPERT_COL_DIM      30
#define EXPERT_NUM_MINES    99

class Minesweeper {
  private:
    // board[i][j]:
    // -1: mine
    // nonnegative integer: the number of mines surrounding this cell
    std::vector<std::vector<int>> board;      // actual board

    // userBoard[i][j]:
    // -1: cell is non visible
    // nonnegative integer: cell is visible to the user and denotes number of mines surrounding it
    std::vector<std::vector<int>> userBoard;  // the board that the user sees

    int numRows;
    int numCols;
    int numMines;
    int cellsRevealed;

    void initializeBoard();
    void initializeUserBoard();
    int getNumMines(int row, int col);
    bool isValid(int row, int col);

  public:
    Minesweeper(int difficulty);
    bool selectTile(int row, int col);
    void revealSolution();
    bool userHasWon();
    void printUserBoard();
};

#endif