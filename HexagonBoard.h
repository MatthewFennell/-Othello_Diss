#ifndef HEXAGONBOARD_H
#define HEXAGONBOARD_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv; // OpenCV API is in the C++ "cv" namespace

class HexagonBoard
{
public:
	// The disc locations and values, as an array of arrays
	int** othelloBoard;

	// Used to color the board
	Vec3b color;

	// Initialise the board
	HexagonBoard(int numberOfSquares);
	void setDiscUnplayable(int column, int row);
	~HexagonBoard();

	Mat drawDiscs(Mat emptyBoard, int ** boardWithDiscs);

	Mat drawBoard();

	bool placeDisc(int row, int column, int playerNumber);

	int flipDiscs(int row, int column, int playerNumber);

	int* scalePixelSelection(int * selection);

	int flipDiscsRight(int row, int column, int playerNumber, bool flipping);
	int flipDiscsLeft(int row, int column, int playerNumber, bool flipping);
	int flipDiscsLeftUp(int row, int column, int playerNumber, bool flipping);
	int flipDiscsLeftDown(int row, int column, int playerNumber, bool flipping);
	int flipDiscsRightUp(int row, int column, int playerNumber, bool flipping);
	int flipDiscsRightDown(int row, int column, int playerNumber, bool flipping);

	bool hasEmpty();

	int numberOfAvailableMoves(int playerNumber);

	void calculateWinner();

private:
	// Board information variables

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
