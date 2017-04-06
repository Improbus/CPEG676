#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <cstdlib>

using namespace std;

//Variables below used for the overall game and can be read in at program start to define how many bombs are in the game and how large the game board is

const int xBoardSize=10; //Read in at program start to determine the size of the x Axis of the board
const int yBoardSize=10; //Read in at program start to determine the size of the y Axis of the board
int numBombs = 5; //Number of Bombs on the gameboard not to exceed the totalGridSize
int totalGridSize = xBoardSize*yBoardSize; //total size of the grid to ensure that numBombs doesn't exceed totalGridSize

bool lossCon; //boolean to determine lose condition (i.e. hitting a bomb)
bool winCon; //boolean to determine win condition (i.e. not hitting a bomb)

//gameboard object

class gameBoard{
  public:
  bool bombHere;
  bool bombMarkedByPlayer;
  bool cellClicked;

  //object structure for blank gameBoard
  gameBoard(void){
    bombHere=false;
    bombMarkedByPlayer=false;
    cellClicked=false;
  }
};


  //method to show the user the gameBoard via command line using recursive loops

void showGameToUser(gameBoard newGame[xBoardSize][yBoardSize]){

  cout << "Game Legend\n";
  cout << "@ = Bomb Here\n";
  cout << "? = Maybe Bomb Here Marked By Player\n";
  cout << "! = Bomb Here Correctly Marked By Player\n";
  cout << "|___| = Blank Space\n";
  cout << "--------------------------------------\n";
  cout << "\n";
  cout << "!------MINESWEEPER COMMAND LINE------!\n";
  cout << "\n";

  for (int row = 0; row<yBoardSize; row++){

    cout << "|";

    for (int column = 0; column<xBoardSize; column++){

      if (newGame[row][column].bombMarkedByPlayer && newGame[row][column].bombHere) {
        cout << "!|";
      } else if (newGame[row][column].bombMarkedByPlayer) {
        cout << "?|";
      } else if (newGame[row][column].bombHere) {
        //  if (winCon==true or lossCon==true){
            cout << "@|";
        //  }
        //  else{
        //    cout << "_|";
        //  }
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

void placeBombsOnBoard(gameBoard newGame[xBoardSize][yBoardSize], int bombs){

    for (int i=0; i<bombs; i++){
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
        }
        else{
          cout << "Mark Adjacent Cells";
          newGame[row][column].cellClicked=true;
        }
};

int main(){
  lossCon = false;
  winCon = false;
  int selection;
  int row;
  int column;

  gameBoard newGame[xBoardSize][yBoardSize];

  placeBombsOnBoard(newGame, numBombs);

  showGameToUser(newGame);

  while(winCon == false){
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
      showGameToUser(newGame);
    }
    else if(selection == 2){
      cout << "Input Row Number." << endl;
      cin >> row;
      cout << "Input Column Number." << endl;
      cin >> column;
      cout << "Coordinates to be Marked in row column order (" << row << "," << column << ")\n";

      setMarker(newGame, row, column);
      showGameToUser(newGame);
    }
    else{
      cout << "Selection out of scope. Try again\n";
    }
  }
};
