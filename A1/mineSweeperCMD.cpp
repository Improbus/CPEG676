#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <time.h>

using namespace std;

//Variables below used for the overall game and can be read in at program start to define how many bombs are in the game and how large the game board is

const int xBoardSize=10; //Read in at program start to determine the size of the x Axis of the board
const int yBoardSize=10; //Read in at program start to determine the size of the y Axis of the board
int numBombs; //Number of Bombs on the gameboard not to exceed the totalGridSize
int totalGridSize = xBoardSize*yBoardSize; //total size of the grid to ensure that numBombs doesn't exceed totalGridSize

bool lossCon; //boolean to determine lose condition (i.e. hitting a bomb)
bool winCon; //boolean to determine win condition (i.e. not hitting a bomb)

//gameboard object

class gameBoard{
  public:
  bool bombHere;
  bool bombMarkedByPlayer;
  bool cellClicked;
  int adjacentMines;

  //object structure for blank gameBoard
  gameBoard(void){
    bombHere=false;
    bombMarkedByPlayer=false;
    cellClicked=false;
    adjacentMines=0;
  }
};


  //method to show the user the gameBoard via command line using recursive loops

void showGameToUser(gameBoard newGame[xBoardSize][yBoardSize]){

  for (int row = 0; row<yBoardSize; row++){

    cout << "|";

    for (int column = 0; column<xBoardSize; column++){

      if (newGame[row][column].bombMarkedByPlayer && newGame[row][column].bombHere) {
        cout << "!|";
      } else if (newGame[row][column].bombMarkedByPlayer) {
        cout << "?|";
      } else if (newGame[row][column].bombHere) {
          if (winCon==true or lossCon==true){
            cout << "@|";
          }
          else{
            cout << "_|";
          }
      }
      else if(newGame[row][column].cellClicked && newGame[row][column].adjacentMines>0 ){
        cout << newGame[row][column].adjacentMines << "|";
      }
      else if(newGame[row][column].cellClicked){
        cout << "*|";
      }
      else{
        cout << "_|";
      }
    }
      cout << "\n";
    }
  };

void showGameOverToUser(gameBoard newGame[xBoardSize][yBoardSize]){

  for (int row = 0; row<yBoardSize; row++){

    cout << "|";

    for (int column = 0; column<xBoardSize; column++){

      if (newGame[row][column].bombHere) {
            cout << "@|";
      }
      else{
        cout << "*|";
      }
    }
      cout << "\n";
    }
};

void placeBombsOnBoard(gameBoard newGame[xBoardSize][yBoardSize], int bombs){

    for (int i=0; i<bombs; i++){
      srand(time(NULL));
      int bombRowcoord = rand()%10;
      int bombColumncoord = rand()%10;

      if (newGame[bombRowcoord][bombColumncoord].bombHere==false){
        newGame[bombRowcoord][bombColumncoord].bombHere=true;
      }
      else{
        i--;
      }

    }
}

void setMarker(gameBoard newGame[xBoardSize][yBoardSize], int row, int column){

  newGame[row][column].bombMarkedByPlayer=true;

};

void clickCell(gameBoard newGame[xBoardSize][yBoardSize], int row, int column){

    if (newGame[row][column].bombHere) {
        winCon=false;
        lossCon=true;
        newGame[row][column].cellClicked=true;

        showGameOverToUser(newGame);

        cout << "GAME OVER YOU HIT A BOMB\n";
        cout << "RESTART PROGRAM TO TRY AGAIN\n";
        cout << endl;
        }
    else{
      if(newGame[row][column].bombMarkedByPlayer==true){newGame[row][column].bombMarkedByPlayer=false;};
      newGame[row][column].cellClicked=true;
      cout << "Mark Adjacent Cells\n"; //Need to add cascade logic and marking for cells here
        /*
        Count all the mines in the 8 adjacent cells
        Cell-->Current Cell (row, col)
        N -->  North        (row-1, col)
        S -->  South        (row+1, col)
        E -->  East         (row, col+1)
        W -->  West         (row, col-1)
        N.E--> North-East   (row-1, col+1)
        N.W--> North-West   (row-1, col-1)
        S.E--> South-East   (row+1, col+1)
        S.W--> South-West   (row+1, col-1)
        */

        int countAdjacentMines =0;

        if(row==0 && column==0){
          if (newGame[row+1][column].bombHere==false ){newGame[row+1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row][column+1].bombHere==false ){newGame[row][column+1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row+1][column+1].bombHere==false ){newGame[row+1][column+1].cellClicked=true;}else{countAdjacentMines++;};

          if(countAdjacentMines>0){newGame[row][column].adjacentMines=countAdjacentMines;};
        }
        else if(row==xBoardSize-1 && column==yBoardSize-1){
          if (newGame[row-1][column].bombHere==false ){newGame[row-1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row][column-1].bombHere==false ){newGame[row][column-1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row-1][column-1].bombHere==false ){newGame[row-1][column-1].cellClicked=true;}else{countAdjacentMines++;};

          if(countAdjacentMines>0){newGame[row][column].adjacentMines=countAdjacentMines;};
        }
        else if(row==0 && column==yBoardSize-1){
          if (newGame[row+1][column].bombHere==false ){newGame[row+1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row][column-1].bombHere==false ){newGame[row][column-1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row+1][column-1].bombHere==false ){newGame[row+1][column-1].cellClicked=true;}else{countAdjacentMines++;};

          if(countAdjacentMines>0){newGame[row][column].adjacentMines=countAdjacentMines;};
        }
        else if(column==0 && row==xBoardSize-1){
          if (newGame[row][column+1].bombHere==false ){newGame[row][column+1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row-1][column].bombHere==false ){newGame[row-1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row-1][column+1].bombHere==false ){newGame[row-1][column+1].cellClicked=true;}else{countAdjacentMines++;};

          if(countAdjacentMines>0){newGame[row][column].adjacentMines=countAdjacentMines;};
        }
        else if(row==0 && column!=0){
            if (newGame[row+1][column].bombHere==false ){newGame[row+1][column].cellClicked=true;}else{countAdjacentMines++;};
            if (newGame[row][column+1].bombHere==false ){newGame[row][column+1].cellClicked=true;}else{countAdjacentMines++;};
            if (newGame[row][column-1].bombHere==false ){newGame[row][column-1].cellClicked=true;}else{countAdjacentMines++;};
            if (newGame[row+1][column+1].bombHere==false ){newGame[row+1][column+1].cellClicked=true;}else{countAdjacentMines++;};
            if (newGame[row+1][column-1].bombHere==false ){newGame[row+1][column-1].cellClicked=true;}else{countAdjacentMines++;};

            if(countAdjacentMines>0){newGame[row][column].adjacentMines=countAdjacentMines;};
        }
        else if(column==0 && row!=0){
          if (newGame[row-1][column].bombHere==false ){newGame[row-1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row+1][column].bombHere==false ){newGame[row+1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row][column+1].bombHere==false ){newGame[row][column+1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row-1][column+1].bombHere==false ){newGame[row-1][column+1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row+1][column+1].bombHere==false ){newGame[row+1][column+1].cellClicked=true;}else{countAdjacentMines++;};

          if(countAdjacentMines>0){newGame[row][column].adjacentMines=countAdjacentMines;};
        }
        else if(row==xBoardSize-1 && column!=yBoardSize-1){
          if (newGame[row-1][column].bombHere==false ){newGame[row-1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row][column+1].bombHere==false ){newGame[row][column+1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row][column-1].bombHere==false ){newGame[row][column-1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row-1][column+1].bombHere==false ){newGame[row-1][column+1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row-1][column-1].bombHere==false ){newGame[row-1][column-1].cellClicked=true;}else{countAdjacentMines++;};

          if(countAdjacentMines>0){newGame[row][column].adjacentMines=countAdjacentMines;};
        }
        else if(column==yBoardSize-1 && row!=xBoardSize-1){
          if (newGame[row-1][column].bombHere==false ){newGame[row-1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row+1][column].bombHere==false ){newGame[row+1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row][column-1].bombHere==false ){newGame[row][column-1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row-1][column-1].bombHere==false ){newGame[row-1][column-1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row+1][column-1].bombHere==false ){newGame[row+1][column-1].cellClicked=true;}else{countAdjacentMines++;};

          if(countAdjacentMines>0){newGame[row][column].adjacentMines=countAdjacentMines;};
        }
        else{
          if (newGame[row-1][column].bombHere==false ){newGame[row-1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row+1][column].bombHere==false ){newGame[row+1][column].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row][column+1].bombHere==false ){newGame[row][column+1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row][column-1].bombHere==false ){newGame[row][column-1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row-1][column+1].bombHere==false ){newGame[row-1][column+1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row-1][column-1].bombHere==false ){newGame[row-1][column-1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row+1][column+1].bombHere==false ){newGame[row+1][column+1].cellClicked=true;}else{countAdjacentMines++;};
          if (newGame[row+1][column-1].bombHere==false ){newGame[row+1][column-1].cellClicked=true;}else{countAdjacentMines++;};

          if(countAdjacentMines>0){newGame[row][column].adjacentMines=countAdjacentMines;};
        }

      }
};

void checkWin(gameBoard newGame[xBoardSize][yBoardSize]){
  int winTotal = totalGridSize - numBombs;
  int countToWin;

  for(int y=0; y<yBoardSize; y++){
    for(int x=0; x<xBoardSize; x++){
      if(newGame[xBoardSize][yBoardSize].cellClicked && !newGame[xBoardSize][yBoardSize].bombHere){
        countToWin++;
      }
    }
    if(winTotal==countToWin){
      winCon=true;
    }
  }
};

int main(){
  lossCon = false;
  winCon = false;
  int selection;
  int row;
  int column;

  cout << "!------MINESWEEPER COMMAND LINE------!\n";
  cout << "\n";
  cout << "--------------------------------------\n";
  cout << "Game Legend\n";
  cout << "@ = Bomb Here\n";
  cout << "? = Maybe Bomb Here Marked By Player\n";
  cout << "! = Bomb Here Correctly Marked By Player\n";
  cout << "|___| = Blank Space\n";
  cout << "--------------------------------------\n";
  cout << "\n";

  cout << "Please Enter the Number of Bombs for your game: \n";
  cin >> numBombs;

  gameBoard newGame[xBoardSize][yBoardSize];

  placeBombsOnBoard(newGame, numBombs);



  while(lossCon == false){
    showGameToUser(newGame);
    cout << "--------------------------------------------\n";
    cout << "Select 1 to Reveal the Coordinates" << endl;
    cout << "Select 2 to Mark the Coordinates" << endl;
    cin >> selection;
    cout << "--------------------------------------------\n";

    if(selection == 1){
      cout << "Input Row Number." << endl;
      cin >> row;
      cout << "Input Column Number." << endl;
      cin >> column;
      cout << "Coordinates to be revealed in row column order (" << row << "," << column << ")\n";

      clickCell(newGame, row, column);
      checkWin(newGame);
    }
    else if(selection == 2){
      showGameToUser(newGame);
      cout << "Input Row Number." << endl;
      cin >> row;
      cout << "Input Column Number." << endl;
      cin >> column;
      cout << "Coordinates to be Marked in row column order (" << row << "," << column << ")\n";

      setMarker(newGame, row, column);
      checkWin(newGame);
    }
    else{
      cout << "Selection out of scope. Try again\n";
    }
  }
};
