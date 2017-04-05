#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

//Variables below used for the overall game and can be read in at program start to define how many bombs are in the game and how large the game board is

const int xBoardSize=10; //Read in at program start to determine the size of the x Axis of the board
const int yBoardSize=10; //Read in at program start to determine the size of the y Axis of the board
int numBombs; //Number of Bombs on the gameboard not to exceed the totalGridSize
int xLocation; //array location for the x coordinate
int yLocation; //array location for the y coordinate
int totalGridSize; //total size of the grid to ensure that numBombs doesn't exceed totalGridSize

bool lossCon; //boolean to determine lose condition (i.e. hitting a bomb)
bool winCon; //boolean to determine win condition (i.e. not hitting a bomb)

//gameboard object

class gameBoard{
  public:
  bool bombHere;
  bool bombMarkedByPlayer;

  //object structure for blank gameBoard
  gameBoard(void){
    bombHere=false;
    bombMarkedByPlayer=false;
  }
};


  //method to show the user the gameBoard via command line using recursive loops

void showGameToUser(gameBoard newGame[xBoardSize][yBoardSize]){

  int xLoc;
  int yLoc;


  cout << "Game Legend\n";
  cout << "@ = Bomb Here\n";
  cout << "? = Maybe Bomb Here Marked By Player\n";
  cout << "! = Bomb Here Correctly Marked By Player\n";
  cout << "|___| = Blank Space\n";
  cout << "--------------------------------------\n";
  cout << "\n";
  cout << "!------MINESWEEPER COMMAND LINE------!\n";
  cout << "\n";

  for (int i = 0; i<yBoardSize; i++){

    cout << "|";
    yLoc = i;
    for (int t = 0; t<xBoardSize; t++){

      if (newGame[yLoc][xLoc].bombHere) {
        if (winCon==true or lossCon==true){
          cout << "@_|";
        }
        else{
          cout << "__|";
        }
      } else if (newGame[yLoc][xLoc].bombMarkedByPlayer) {
        cout << "?_|";
      } else if ((newGame[yLoc][xLoc].bombMarkedByPlayer) && newGame[yLoc][xLoc].bombHere) {
        cout << "!_|";
      }
        else{
          cout << "__|";
        }
        xLoc = t;
      }
      cout << "\n";
    }
  };

void placeBombsOnBoard(gameBoard newGame[xBoardSize][yBoardSize], int bombs){

    for (int i =0; i<bombs; i++){
      int bombXcoord = rand()%10;
      int bombYcoord = rand()%10;

      if (newGame[bombXcoord][bombYcoord].bombHere==false){
        newGame[bombXcoord][bombYcoord].bombHere=true;
      }
      else{
        i--;
      }

    }
}

void setMarker(gameBoard newGame[xBoardSize][yBoardSize], int xLoc, int yLoc){

  newGame[xLoc][yLoc].bombMarkedByPlayer==true;

};

void click

int main(){
  lossCon = false;
  winCon = false;
  numBombs = 5;

  xLocation = 0;
  yLocation = 0;

  gameBoard newGame[xBoardSize][yBoardSize];

  placeBombsOnBoard(newGame, 5);

  showGameToUser(newGame);

  while(winCon == false){
    cout << "Input x coordinate to check." << endl;
    cin >> xLocation;

  }
};
