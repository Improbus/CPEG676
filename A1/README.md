# CPEG676- Assignment 1 - Command Line Minesweeper
Command Line Minesweeper implementation in C++

General Usage.

Edit Lines 12 and 13 to adjust the game board size.

Compile with g++

g++ mineSweeperCMD.cpp -o mineSweeperCMD

run the executable:

./mineSweeperCMD

To Do List:
• General Variable and comment hygiene to make the code more readable and maintainable

• Find a way for the user to select the size of the game without having to edit the code.  I think this could be accomplished by using a two-dimensional vector of objects but I would need to look more into C/C++’s ability to handle this.

• Fix the grid formatting when the row and column number is greater than 9.  The game still works but the presentation to the user could be better.

• I would like the improve game performance.  When selecting a large number of bombs it takes a few seconds for the gameboard to be created.
