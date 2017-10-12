#ifndef BOARD_H
#define BOARD_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv; // OpenCV API is in the C++ "cv" namespace

class Board
{
public:
	// Initialise the board
	Board(int blockSize, int numberOfSquares, int offset);

	// Deallocates the board
	~Board();

	void deleteBoard();

	// --------------------------------------BOARD VARIABLES----------------------------------- //
	int** othelloBoard;
	Vec3b color;
	Mat clearBoard;
	// ---------------------------------------------------------------------------------------- //


	// --------------------------------------PRINT OPERATIONS-----------------------------------//
	void printBoardValues();
	int printWinner();
	// ---------------------------------------------------------------------------------------- //

	// --------------------------------------BOARD OPERATIONS---------------------------------- //
	// Sets a disc void such that it can't be played on
	void setDiscUnplayable(int row, int column);

	void setDiscEmpty(int row, int column);

	void setInitialSquares();

	Mat drawAvailablePositions(int playerNumber, Mat currentBoard);

	// Draws an empty board
	Mat drawBoard();

	// Draws discs onto the board
	Mat drawDiscs(Mat emptyBoard, int**);

	// Places a disc at the given row / column for player *playerNumber*
	bool placeDisc(int row, int column, int playerNumber);

	// Sets the board values
	void setBoardValues(int** newBoardValues);

	// Transforms pixel co-ordinates into row/column co-ordinates
	int* scalePixelSelection(int* selection);

	// Places a disc and flips the respective discs
	int flipDiscs(int row, int column, int playerNumber);

	// Flips the discs in the respective direction
	int flipDiscsAbove(int row, int column, int playerNumber, bool flipping);
	int flipDiscsBelow(int row, int column, int playerNumber, bool flipping);
	int flipDiscsLeft(int row, int column, int playerNumber, bool flipping);
	int flipDiscsRight(int row, int column, int playerNumber, bool flipping);

	int flipDiscsLeftUp(int row, int column, int playerNumber, bool flipping);
	int flipDiscsLeftDown(int row, int column, int playerNumber, bool flipping);
	int flipDiscsRightUp(int row, int column, int playerNumber, bool flipping);
	int flipDiscsRightDown(int row, int column, int playerNumber, bool flipping);
	// ---------------------------------------------------------------------------------------- //

	// --------------------------------------GET OPERATIONS------------------------------------ //
	// Returns the array of arrays representing disc locations
	int** getBoard();

	// Returns the board values
	int** getBoardValues() { return othelloBoard; }

	// Checks if there are any empty squares in the board - true if no gaps
	bool hasEmpty();

	// Calculates how many moves are available to player *playerNumber*
	int numberOfAvailableMoves(int playerNumber);

	// Returns an array of arrays representing the moves available to player *playerNumber*
	// 2nd parameter = number of moves available - needed to create the correct sized array
	int** availableMoveLocations(int playerNumber, int numberOfMoves);

	// Calculates who won the game
	int calculateWinner();

	// Obvious
	int getBlockSize() { return blockSize; }
	int getBoardSize() { return boardSize; }
	int getOffset() { return offset; }
	// ---------------------------------------------------------------------------------------- //

private:
	// size of each square
	int blockSize;

	// width or height of board (pixels) - both the same as its a square
	int boardSize;

	// the size of the background area around the board
	int offset;

	// size of total image (pixels)
	int imageSize;
};
#endif