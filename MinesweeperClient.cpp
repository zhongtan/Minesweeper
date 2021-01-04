#include "Minesweeper.h"

using namespace std;

void printIntroMessage() {
  cout << endl;
  cout << "******************* Minesweeper Game *********************" << endl;
  cout << "1 - Beginner: \t\t" << BEGINNER_ROW_DIM << "x" << BEGINNER_COL_DIM << ", " << BEGINNER_NUM_MINES << " mines" << endl;
  cout << "2 - Intermediate: \t" << INTERM_ROW_DIM << "x" << INTERM_COL_DIM << ", " << INTERM_NUM_MINES << " mines" << endl;
  cout << "3 - Expert: \t\t" << EXPERT_ROW_DIM << "x" << EXPERT_COL_DIM << ", " << EXPERT_NUM_MINES << " mines" << endl;
  cout << "**********************************************************" << endl;
  cout << endl;
}

int main() {
  bool cont = true;

  while (cont) {
    printIntroMessage();
    int difficulty = -1;

    while (difficulty != 1 && difficulty != 2 && difficulty != 3) {
      cout << "Please select your difficulty level (1, 2 or 3) or Ctrl+C to exit the game: ";
      cin >> difficulty;
    }

    Minesweeper m(difficulty);
    bool userLost = false;

    while (!m.userHasWon()) {
      m.printUserBoard();
      int row;
      int col;
      cout << "Select: " << endl;
      cout << "Row: ";
      cin >> row;
      cout << "Col: ";
      cin >> col;

      try {
        // user detonated a mine
        if (!m.selectTile(row, col)) {
          userLost = true;
          string userCont = "";
          while (userCont != "Y" && userCont != "N") {
            cout << "Oops! You have detonated a mine. Revealing solution ... \n\n";
            m.revealSolution();
            cout << "Continue (Y/N)? ";
            cin >> userCont;
          }

          if (userCont == "N") {
            cont = false;
          }
          break;
        }
      } catch (const invalid_argument& e) {
        cout << "You entered an invalid cell position. Please try again ... " << endl;
      }
    }

    if (userLost) {
      if (!cont) {
        break;
      }
    } else {
      cout << "Congratulations, you won!" << endl;

      string userCont = "";
      while (userCont != "Y" && userCont != "N") {
        cout << "Play again (Y/N)? ";
        cin >> userCont;
      }

      if (userCont == "N") {
        break;
      }
    }
  }
}