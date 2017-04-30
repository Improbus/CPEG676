#include <iostream>
#include <vector>
#include "cell.h"
/**
 * randomly places the entered number of bombs onto the board
 * takes in a board, passed by reference, the board's width and height, and the desired number of bombs
 * */

void addBomb(std::vector< std::vector<cell> >* board, int width, int height, int numbombs) {
    srand(time(NULL));
    
    // for the amount of given bombs, randomly generate a location to place it
    for (int i = 0; i < numbombs; i++) {
        int bomb_x = 0, bomb_y = 0;
        // will randomly generate a location for the bomb; if the generated location is already
        // occupied by a bomb, it will generate a new location
        do {
            bomb_x = (rand() % width);
            bomb_y = (rand() % height);
        } while (board->at(bomb_x).at(bomb_y).get_is_bomb() == true);
        // once an acceptable location is chosen for the bomb, that cell is set to be a bomb
        board->at(bomb_x).at(bomb_y).set_bomb(true);
    }
}

/**
 * prints board in it's current state
 * takes in a board, passed by reference, and the width and height of the board
 * */
void updateBoard(std::vector< std::vector<cell> >* board, int width, int height){
    // labels the x axis
    std::cout << "  _";
	for (int i = 0; i < width; i++) {
		if (i < 10) std::cout << i << "__";
		else std::cout << i << "_";
	}
	std::cout << std::endl;

	//draws body
	for (int y = 0; y < height ; y++){
	    // labels the y axis
	    if (y < 10) std::cout << y << " |";
		else std::cout << y << "|";
		    
		for (int x = 0; x < width ; x++){
		    // displays a !! if you hit a bomb
		    if(board->at(x).at(y).get_is_bomb() && board->at(x).at(y).get_is_marked()) {
		        std::cout << "!!|";
		    }
		    
		    // could be useful for testing purposes, if using this comment out the above if statement
		    /* 
			if(board->at(x).at(y).get_is_bomb()){
				if(board->at(x).at(y).get_is_marked()){
				    std::cout << "!!|";
				}
				else{
				    std::cout << "bb|";
				}
			}
			*/
			// if the cell is marked...
			else if (board->at(x).at(y).get_is_marked()){
			    // and the cell has a flag of a number higher than 0, display the number
			    if(board->at(x).at(y).get_flag() > '0'){
			        std::cout << board->at(x).at(y).get_flag() << " |";
			    }
			    else{ // otherwise you just display a blank space
				    std::cout << "  |";
			    }
			}
			else{ // otherwise display the default __
				std::cout << "__|";
			}			
		}
		std::cout << std::endl;
	}
}

/**
 * Checks to make sure the given coordinates are within the bounds of the given width and height
 * takes in an x coordinate, a y coordinate, a width, and a height
 * returns true if the x and y coordinate are within the given bounds, false otherwise
 **/
bool is_in_range(int x, int y, int w, int h) {
    return (x >= 0) && (x < w) && (y >= 0) && (y < h);
}

/** 
 * checks to see if the cell at the given x and y location has any adjacent bombs
 * takes in the board, the x position, and the y position
 * returns true if there is even one adjacent bomb, else return false
 **/
bool checkadjbomb(std::vector<std::vector<cell> > board, int x, int y) {
    // loop to check all adjacent cells
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            // first makes sure the cell is valid in the bounds of the board
            // then checks if the cell is a bomb
            if(is_in_range(x, y, board.size(), board.at(0).size()) 
                && board.at(x+i).at(y+i).get_is_bomb()) {
                // if it is a bomb, cool you found an adjacent bomb
                return true;
            }
        }
    }
    return false;
}

/**
 * Determines whether or not the board is in a win state
 * takes in the board
 * returns true if the board is considered a win state, false otherwise
 **/
bool is_win(std::vector< std::vector<cell> > board) {
    // simple enough, has to check every tile to make sure it is either flipped or a bomb
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board.at(0).size(); j++) {
            // if even one tile is not flipped or a bomb, then the board is not a winning one
            if(!board.at(i).at(j).get_is_marked() && !board.at(i).at(j).get_is_bomb()) return false;
        }
    }
    // if you exit the loop, YAY you win
    return true;
}

/**
 * Will set the number for the current cell
 * takes in the board, the x position, the y position, the width, and the height
 * checks all adjacent cells to see if they are bombs, if they are, add 1 to the total count
 * sets the current cell's tag to be the character representing its calculated number
 **/
void set_tile_num(std::vector< std::vector<cell> > *board, int x, int y, int width, int height) {
    int count = 0;
    // loop to check every adjacent cell
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            // first makes sure the tile is within the range of the board, then checks if the tile is a bomb
            if(is_in_range(x+i, y+j, width, height) && board->at(x+i).at(y+j).get_is_bomb()) count++;
        }
    }
    
    // sets the cell's flag to the character '0' plus the number of adjacent bombs
    // since the max number for this is 8 there is no need to worry about if the number was more than 9
    board->at(x).at(y).set_flag('0'+count);
}

/**
 * when you select a cell of the board, this function is called
 * it takes in the board, passed by reference, and the input x and y coordinate
 * if the selected cell is a bomb the function will return true, otherwise it will return false
 * this is where most of the logic behind the actual game play is handled
 **/
bool select_cell(std::vector< std::vector<cell> > *board, int x, int y) {
    // get the width(w) and height(h) of the board
    int w = board->size(), h = board->at(0).size();
    // if x or y are out of bounds, return false; you don't lose for selecting an out of bounds tile
    // it just doesn't acomplish anything
    if(x < 0 || y < 0 || x >= w || y >= h) return false;
    // if the selected tile is already marked as selected you do nothing
    if(board->at(x).at(y).get_is_marked()) return false;
    
    // set the selected cell to be marked
    board->at(x).at(y).set_marked(true);
    
    // if a bomb is selected, return true; this means you lose
    if(board->at(x).at(y).get_is_bomb()) return true; 
    
    // calls the set tile number function, it will set the tiles number of bombs adjacent
    // if this is 0 then it means the tile has no bombs near it
    set_tile_num(board, x, y, w, h);
    
    // if the tile has any adjacent bombs, then it is a dead end and you exit the function
    if(board->at(x).at(y).get_flag() > '0') {
        return false;
    }
    
    // recursively calls itself on all of the cells surrounded the selected tile
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            select_cell(board, x+i, y+j);
        }
    }
    
    // if you made it this far in the function, congradulations, you didn't lose
    return false;
}

/**
 * Takes in 2 int variables, passed by reference
 * has the user input values for the 2 input integers
 * w = width, h = height
 * returns the generated board
 **/
std::vector< std::vector<cell> > make_board(int* w, int* h) {
    int n;
    std::cout << "Enter the width (between 1-20): ";
    std::cin >> *w;
    
    while(*w > 20 || *w < 1) {
        std::cout << "Out of range, try again" << std::endl;
        std::cin >> *w;
    }
    
    std::cout << "Enter the height (between 1-20): ";
    std::cin >> *h;
    
    while(*h > 20 || *h < 1) {
        std::cout << "Out of range, try again" << std::endl;
        std::cin >> *h;
    }
    
    int max = (*h) * (*w) - 1;
    
    std::cout << "Enter the number of bombs (between 1-" << max << "): ";
    std::cin >> n;
    while(n > ((*h) * (*w)) || n < 1) {
        std::cout << "Out of range, try again" << std::endl;
        std::cin >> n;
    } 
    std::vector< std::vector<cell> > board;
    board.resize(*w, std::vector<cell>(*h,cell()));
    addBomb(&board, *w, *h, n);
    
    return board;
}

/**
 * play calls all of the other functions in the correct order to play the game
 * and includes logic to mutate the board as the user inputs values
 * */
void play(std::vector< std::vector<cell> > board, int width, int height) {
    int x, y;
    bool lose = false;
    
    // draw the board
    updateBoard(&board, width, height);
    
    // while you're not a LOSER, play the game
    while(!lose) {
        std::cout << "Test location at: Enter Column (0-height)";
        std::cin >> x;
        while(x > 20 || x < 0) {
            std::cout << "invalid, try again" << std::endl;
            std::cin >> x;
        }
        std::cout << "Test location at: Enter Row (0-width)";
        std::cin >> y;
        while(y > 20 || y < 0) {
            std::cout << "invalid, try again" << std::endl;
            std::cin >> y;
        }
        
        // does all the magic of selecting a location in a seperate function
        lose = select_cell(&board, x, y);
        
        // draw the board again fam
        updateBoard(&board,height,width);
        // if you win, determined by another function, then yay cool
        if(is_win(board)) {
            std::cout << std::endl << 
            "_____.___.________   ____ ___   __      __.___ _______  \n" <<
            "\\__  |   |\\_____  \\ |    |   \\ /  \\    /  \\   |\\      \\  \n" <<
            " /   |   | /   |   \\|    |   / \\   \\/\\/   /   |/   |   \\ \n" <<
            " \\____   |/    |    \\    |  /   \\        /|   /    |    \\ \n" <<
            " / ______|\\_______  /______/     \\__/\\  / |___\\____|__  / \n" <<
            " \\/               \\/                  \\/              \\/ \n"
             << std::endl;
            // deallocate the memory for all of the cells of the board
            for(int i = 0; i<height -1;i++){
                for(int j = 0; j <width -1; j++){
                    board[i][j].~cell();
                }
            }
            return;
        }
    }
    // if you get this far, you're a LOSER, oh no
    std::cout << 
    "_____.___.________   ____ ___  .____    ________    ____________________\n"<<
    "\\__  |   |\\_____  \\ |    |   \\ |    |   \\_____  \\  /   _____/\\_   _____/\n"<<
    " /   |   | /   |   \\|    |   / |    |    /   |   \\ \\_____  \\  |    __)_ \n"<<
    " \\____   |/    |    \\    |  /  |    |___/    |    \\/        \\ |        \\ \n"<<
    " / ______|\\_______  /______/   |_______ \\_______  /_______  //_______  / \n"<<
    " \\/               \\/                   \\/       \\/        \\/         \\/ \n" << std::endl;
    // even though you're a LOSER you still need to deallocate them cells, boi!
    for(int i = 0; i<height -1;i++){
        for(int j = 0; j <width -1; j++){
            board[i][j].~cell();
        }
    }
}