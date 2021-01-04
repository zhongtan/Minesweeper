#include "Minesweeper.h"

using namespace std;

// This function counts the number of digits in a given base-10 integer.
// For example, 2 has 1 digit, while 10 has 2 digits.
int countNumberOfDigits(int num) {
  int numDigits = 1;
  while (num >= 10) {
    num /= 10;
    ++numDigits;
  }
  return numDigits;
}

/**
 * Initialize the actual board containing numbers representing the total number of
 * mines surrounding the current cell. This constructor also initializes the user board
 * where the cells are not visible initially until the user has clicked on a cell. The
 * dimensions of the board and number of mines in the board depends on the given difficulty.
 */
Minesweeper::Minesweeper(int difficulty) {
  if (difficulty == 1) {
    // beginner
    numRows = BEGINNER_ROW_DIM;
    numCols = BEGINNER_COL_DIM;
    numMines = BEGINNER_NUM_MINES;
  } else if (difficulty == 2) {
    // intermediate
    numRows = INTERM_ROW_DIM;
    numCols = INTERM_COL_DIM;
    numMines = INTERM_NUM_MINES;
  } else if (difficulty == 3) {
    // expert
    numRows = EXPERT_ROW_DIM;
    numCols = EXPERT_COL_DIM;
    numMines = EXPERT_NUM_MINES;
  }

  initializeBoard();
  initializeUserBoard();

  cellsRevealed = 0;
}

/**
 * Initialize the actual board with no mines initially. Then, place the mines randomly
 * and assign numbers to each cell (that does not contain a mine) the number of mines
 * surrounding it.
 */ 
void Minesweeper::initializeBoard() {
  for (int i = 0; i < numRows; i++) {
    vector<int> nums;
    for (int j = 0; j < numCols; j++) {
      nums.push_back(0);
    }
    board.push_back(nums);
  }

  srand((unsigned) time(NULL));

  // randomly place mines on the actual board
  for (int i = 0; i < numMines; i++) {
    int randNum = rand() % (numRows * numCols);
    int row = randNum / numCols;
    int col = randNum % numCols;

    // keep sampling while there is a mine on the sampled cell
    while (board[row][col] == -1) {
      randNum = rand() % (numRows * numCols);
      row = randNum / numCols;
      col = randNum % numCols;
    }

    board[row][col] = -1;
  }

  // assign the number of mines surrounding each cell
  for (int r = 0; r < numRows; ++r) {
    for (int c = 0; c < numCols; ++c) {
      if (board[r][c] != -1) {
        board[r][c] = getNumMines(r, c);
      }
    }
  }
}

/**
 * Initialize the user board. The cells on the user board will be filled with -1,
 * indicating that they are not visible at the beginning.
 */ 
void Minesweeper::initializeUserBoard() {
  for (int i = 0; i < numRows; i++) {
    vector<int> nums;
    for (int j = 0; j < numCols; j++) {
      nums.push_back(-1);
    }
    userBoard.push_back(nums);
  }
}

/**
 * This helper function returns the number of mines surrounding the cell at the 
 * position specified by the given row and column.
 */ 
int Minesweeper::getNumMines(int row, int col) {
  int dirs[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1}, 
    {0, -1}, {0, 1}, 
    {1, -1}, {1, 0}, {1, 1}
  };

  int numMines = 0;

  for (auto& dir : dirs) {
    int newRow = row + dir[0];
    int newCol = col + dir[1];

    if (isValid(newRow, newCol) && board[newRow][newCol] == -1) {
      ++numMines;
    }
  }
  return numMines;
}

/**
 * This helper function returns True if the given position is within the boundaries
 * of the board and False otherwise.
 */ 
bool Minesweeper::isValid(int row, int col) {
  return row >= 0 && row < numRows && col >= 0 && col < numCols;
}

/**
 * Simulate a user selecting a tile at the position specified by the given row and
 * column. If the cell contains a mine, return False to indicate that the user has lost.
 * If the user clicks on a cell that has no surrounding mines, perform a BFS to reveal the
 * surrounding area.
 */
bool Minesweeper::selectTile(int row, int col) {
  // invalid position
  if (!isValid(row, col)) {
    throw invalid_argument("Invalid cell position.");
  }

  // player has clicked on a mine
  if (board[row][col] == -1) {
    return false;
  } 
  
  if (board[row][col] > 0) {
    userBoard[row][col] = board[row][col];
    ++cellsRevealed;
  } else {
    // reveal the surrounding area from the top and botton
    queue<pair<int, int>> q;
    set<pair<int, int>> visited;
    int dirs[4][2] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

    q.push(make_pair(row, col));
    visited.insert(make_pair(row, col));

    while (!q.empty()) {
      auto coord = q.front(); q.pop();
      int currRow = coord.first;
      int currCol = coord.second;

      userBoard[currRow][currCol] = board[currRow][currCol];
      ++cellsRevealed;

      for (auto& dir : dirs) {
        int newRow = currRow + dir[0];
        int newCol = currCol + dir[1];
        auto newPair = make_pair(newRow, newCol);

        if (isValid(newRow, newCol) && board[currRow][currCol] == 0 && 
            find(visited.begin(), visited.end(), newPair) == visited.end()) {
          visited.insert(make_pair(newRow, newCol));
          q.push(make_pair(newRow, newCol));
        }
      }
    }
  }

  return true;
}

/**
 * Reveal the solution by printing to console the actual board containing the placements
 * of each mine and also the number of mines surrounding each cell (that does not contain a mine).
 * If there are no mines surrounding a specific cell, simply print an empty character.
 */ 
void Minesweeper::revealSolution() {
  int indent = INDENT_SPACE_WIDTH + countNumberOfDigits(numCols);

  for (int i = 0; i < numCols; i++) {
    cout << setw(indent) << i;
  }
  cout << endl;

  for (int r = 0; r < numRows; r++) {
    cout << r;

    if (board[r][0] == -1) {
      cout << setw(indent - countNumberOfDigits(r)) << 'M';
    } else if (board[r][0] == 0) {
      cout << setw(indent - countNumberOfDigits(r)) << ' ';
    } else {
      cout << setw(indent - countNumberOfDigits(r)) << board[r][0];
    }

    for (int c = 1; c < numCols; c++) {
      if (board[r][c] == -1) {
        cout << setw(indent) << 'M';
      } else if (board[r][c] == 0) {
        cout << setw(indent) << ' ';
      } else {
        cout << setw(indent) << board[r][c];
      }
    }
    cout << endl;
  }
}

/**
 * This function prints the user board to the console. It is called after the user
 * selects a cell that does not contain a mine.
 */ 
void Minesweeper::printUserBoard() {
  int indent = INDENT_SPACE_WIDTH + countNumberOfDigits(numCols);

  for (int i = 0; i < numCols; i++) {
    cout << setw(indent) << i;
  }
  cout << endl;

  for (int r = 0; r < numRows; r++) {
    cout << r;

    if (userBoard[r][0] == -1) {
      cout << setw(indent - countNumberOfDigits(r)) << '*';
    } else if (userBoard[r][0] == 0) {
      cout << setw(indent - countNumberOfDigits(r)) << ' ';
    } else {
      cout << setw(indent - countNumberOfDigits(r)) << userBoard[r][0];
    }

    for (int c = 1; c < numCols; c++) {
      if (userBoard[r][c] == -1) {
        cout << setw(indent) << '*';
      } else if (userBoard[r][c] == 0) {
        cout << setw(indent) << ' ';
      } else {
        cout << setw(indent) << userBoard[r][c];
      }
    }
    cout << endl;
  }
}

/**
 * Returns True if the user has won, i.e. the only cells not revealed are the
 * cells containing mines.
 */
bool Minesweeper::userHasWon() {
  return cellsRevealed == numRows * numCols - numMines;
}