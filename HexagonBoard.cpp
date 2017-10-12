#include "Board.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "HexagonBoard.h"
#include <math.h>

#define PI 3.14159265

using namespace cv;
using namespace std;

// @block   : Determines the size of each square
// @squares : Determines what size the square board is
// @offset  : Determines the size of the background area
// Create a new board object

HexagonBoard::HexagonBoard(int squares)
{
	// Set the board dimensions

	// Total size of display
	imageSize = 700;

	// Number of hexagons at the edge
	boardSize = squares;
	
	// Size of a each side of the hexagons
	//blockSize = (700 - 3 * squares) / ( (2 * squares - 1)*(2 * cos(30 * PI / 180)));

	blockSize = (700 - 4 * (2 * boardSize - 2)) / ((2 * cos(30 * PI / 180))*(1 + (2 * boardSize - 2)));

	// Initialise the disc arrays - do the math!
	othelloBoard = new int*[2 * boardSize - 1];

	// Creates all of the requires arrays, based on the board size
	for (int i = 0; i < 2 * boardSize - 1; i++) {
		if (i <= (2 * boardSize - 1) / 2) {
			othelloBoard[i] = new int[boardSize + i];
		}
		else {
			othelloBoard[i] = new int[2 * boardSize - 1 - (i - boardSize) - 1];
		}
	}

	// Based on the board size, sets all of the arrays to have every value as 0
	for (int i = 0; i < 2 * boardSize - 1; i++) {

		if (i <= (2 * boardSize - 1) / 2) {
			for (int j = 0; j < boardSize + i; j++) {
				othelloBoard[i][j] = 0;
			}
		}
		else {
			for (int j = 0; j < 2 * boardSize - 1 - (i - boardSize) - 1; j++) {
				othelloBoard[i][j] = 0;
			}
		}
	}
	// Initialise the middle discs (middle based off the boardSize)
	othelloBoard[boardSize - 1][boardSize - 1] = 1;
	othelloBoard[boardSize][boardSize] = 1;

	othelloBoard[boardSize - 1][boardSize] = -1;
	othelloBoard[boardSize][boardSize - 1] = -1;
}

// Sets a disc to be an empty square, can't be played on
void HexagonBoard::setDiscUnplayable(int column, int row) {
	// Column = -1 when not the middle of the haxagon is pressed
	if (column != -1) {
		othelloBoard[column][row] = 2;
	}
}

HexagonBoard::~HexagonBoard()
{
}

// @emptyBoard : An image of a board with no discs drawn on it
// @boardWithDiscs : An array of arrays, representing the disc locations
// Takes the empty board as a parameter and draws on all of the discs, which are given as the 2nd parameter
Mat HexagonBoard::drawDiscs(Mat emptyBoard, int** boardWithDiscs) {

	int verticalSeperator = 4;
	int horizonalSeperator = 4;

	Scalar lineColor;
	lineColor = Scalar(12, 147, 222);
	int lineThickness = 7;


	for (int i = 0; i < 2*boardSize - 1; i++) {
		if (i <= (2 * boardSize - 1) / 2) {
			for (int j = 0; j < boardSize + i; j++) {
				// Co-ordinates for points in upper half of the board

				Point vertex[1][6];
				double x_one = blockSize / 2 + 2 * j*blockSize*cos(30 * PI / 180) + (boardSize - (i+1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_one = blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

				double x_two = blockSize / 2 + blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) + (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_two = blockSize - blockSize*sin(30 * PI / 180) + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i+1) * verticalSeperator;

				double x_three = blockSize / 2 + 2 * blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) + (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_three = blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

				double x_four = blockSize / 2 + 2 * blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) + (boardSize - (i + 1))* blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_four = 2 * blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

				double x_five = blockSize / 2 + blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) + (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_five = 2 * blockSize + blockSize*sin(30 * PI / 180) + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

				double x_six = blockSize / 2 + 2 * j*blockSize*cos(30 * PI / 180) + (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_six = 2 * blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;
				
				vertex[0][0] = Point(x_one, y_one);
				line(emptyBoard, Point(x_one, y_one), Point(x_two, y_two), lineColor, lineThickness, 8);

				vertex[0][1] = Point(x_two, y_two);
				line(emptyBoard, Point(x_two, y_two), Point(x_three, y_three), lineColor, lineThickness, 8);

				vertex[0][2] = Point(x_three, y_three);
				line(emptyBoard, Point(x_three, y_three), Point(x_four, y_four), lineColor, lineThickness, 8);

				vertex[0][3] = Point(x_four, y_four);
				line(emptyBoard, Point(x_four, y_four), Point(x_five, y_five), lineColor, lineThickness, 8);

				vertex[0][4] = Point(x_five, y_five);
				line(emptyBoard, Point(x_five, y_five), Point(x_six, y_six), lineColor, lineThickness, 8);

				vertex[0][5] = Point(x_six, y_six);
				line(emptyBoard, Point(x_six, y_six), Point(x_one, y_one), lineColor, lineThickness, 8);

				const Point* ppt[1] = { vertex[0] };
				int npt[] = { 6 };

				if (othelloBoard[i][j] == 0) {
					fillPoly(emptyBoard, ppt, npt, 1, Scalar(50, 100, 0), 8);
				}
				else if (othelloBoard[i][j] == 1) {
					fillPoly(emptyBoard, ppt, npt, 1, Scalar(255, 255, 255), 8);
				}
				else if (othelloBoard[i][j] == -1) {
					fillPoly(emptyBoard, ppt, npt, 1, Scalar(0, 0, 0), 8);
				}
				else if (othelloBoard[i][j] == 2) {
					fillPoly(emptyBoard, ppt, npt, 1, Scalar(100, 100, 100), 8);
				}			
			}
		}
		else {
			for (int j = 0; j < 2 * boardSize - 1 - (i - boardSize) - 1; j++) {
				// Co-ordinates for points in upper half of the board
				Point vertex[1][6];
				double x_one = blockSize / 2 + 2 * j*blockSize*cos(30 * PI / 180) - (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_one = blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

				double x_two = blockSize / 2 + blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) - (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_two = blockSize - blockSize*sin(30 * PI / 180) + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1) * verticalSeperator;

				double x_three = blockSize / 2 + 2 * blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) - (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_three = blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

				double x_four = blockSize / 2 + 2 * blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) - (boardSize - (i + 1))* blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_four = 2 * blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

				double x_five = blockSize / 2 + blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) - (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_five = 2 * blockSize + blockSize*sin(30 * PI / 180) + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

				double x_six = blockSize / 2 + 2 * j*blockSize*cos(30 * PI / 180) - (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
				double y_six = 2 * blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

				vertex[0][0] = Point(x_one, y_one);
				line(emptyBoard, Point(x_one, y_one), Point(x_two, y_two), lineColor, lineThickness, 8);

				vertex[0][1] = Point(x_two, y_two);
				line(emptyBoard, Point(x_two, y_two), Point(x_three, y_three), lineColor, lineThickness, 8);

				vertex[0][2] = Point(x_three, y_three);
				line(emptyBoard, Point(x_three, y_three), Point(x_four, y_four), lineColor, lineThickness, 8);

				vertex[0][3] = Point(x_four, y_four);
				line(emptyBoard, Point(x_four, y_four), Point(x_five, y_five), lineColor, lineThickness, 8);

				vertex[0][4] = Point(x_five, y_five);
				line(emptyBoard, Point(x_five, y_five), Point(x_six, y_six), lineColor, lineThickness, 8);

				vertex[0][5] = Point(x_six, y_six);
				line(emptyBoard, Point(x_six, y_six), Point(x_one, y_one), lineColor, lineThickness, 8);

				const Point* ppt[1] = { vertex[0] };
				int npt[] = { 6 };

				if (othelloBoard[i][j] == 0) {
					fillPoly(emptyBoard, ppt, npt, 1, Scalar(50, 100, 0), 8);
				}
				else if (othelloBoard[i][j] == 1) {
					fillPoly(emptyBoard, ppt, npt, 1, Scalar(255, 255, 255), 8);
				}
				else if (othelloBoard[i][j] == -1) {
					fillPoly(emptyBoard, ppt, npt, 1, Scalar(0, 0, 0), 8);
				}
				else if (othelloBoard[i][j] == 2) {
					fillPoly(emptyBoard, ppt, npt, 1, Scalar(100, 100, 100), 8);
				}				
			}
		}
	}
	return emptyBoard;
}

// Draws an empty board, then calls the drawDiscs function to add in the discs
Mat HexagonBoard::drawBoard() {
	Mat clearBoard(imageSize, imageSize + 50, CV_8UC3, Scalar::all(100));
	// Determine the color of the board. Channel 1 = green component
	color.val[0] = 100;
	color.val[1] = 100;
	color.val[2] = 100;

	// Sets the whole board gray
	for (double y = 0 + offset / 2; y<imageSize; y++)
	{
		for (double x = 0 + offset / 2; x<imageSize; x++)
		{
			clearBoard.at<Vec3b>(Point(x, y)) = color;
		}
	}
	// Function that draws the disc values into the board too
	Mat boardWithDiscs = drawDiscs(clearBoard, othelloBoard);

	return boardWithDiscs;
}

// @row		     : The selected row (0-7)
// @column		 : The selected column (0-7)
// @playerNumber : Whether it is player 1 or 2
bool HexagonBoard::placeDisc(int row, int column, int playerNumber) {
	// Returns true if placing a disc there flips any discs
	int totalFlips = flipDiscs(row, column, playerNumber);

	if (totalFlips > 0) {
		if (playerNumber == 1) {
			othelloBoard[row][column] = 1;
		}
		else if (playerNumber == -1) {
			othelloBoard[row][column] = -1;
		}
	}
	if (totalFlips > 0) {
		return true;
	}
	else {
		return false;
	}
}

int HexagonBoard::flipDiscs(int row, int column, int playerNumber) {
	// Returns total number of discs flipped by placing a disc at the specified row / column
	int totalNumberFlipped = 0;
	if (othelloBoard[row][column] == 0) {
		int left = flipDiscsLeft(row, column, playerNumber, true);
		int right = flipDiscsRight(row, column, playerNumber, true);

		int leftUp = flipDiscsLeftUp(row, column, playerNumber, true);
		int leftDown = flipDiscsLeftDown(row, column, playerNumber, true);

		int rightUp = flipDiscsRightUp(row, column, playerNumber, true);
		int rightDown = flipDiscsRightDown(row, column, playerNumber, true);

		//totalNumberFlipped = 1;
		totalNumberFlipped = left + right + leftUp + leftDown + rightUp + rightDown;
	}
	else {
		totalNumberFlipped = 0;
	}
	return totalNumberFlipped;
}

// @emptyBoard : An image of a board with no discs drawn on it
// @boardWithDiscs : An array of arrays, representing the disc locations
// Takes the empty board as a parameter and draws on all of the discs, which are given as the 2nd parameter
int* HexagonBoard::scalePixelSelection(int* selection) {

	int* column_row = new int[2];
	column_row[0] = -1;

	int verticalSeperator = 4;
	int horizonalSeperator = 4;

		for (int i = 0; i < 2 * boardSize - 1; i++) {
			if (i <= (2 * boardSize - 1) / 2) {
				for (int j = 0; j < boardSize + i; j++) {
					// Co-ordinates for points in upper half of the board

					Point vertex[1][6];
					double x_one = blockSize / 2 + 2 * j*blockSize*cos(30 * PI / 180) + (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
					double x_three = blockSize / 2 + 2 * blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) + (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;

					double y_one = blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;
					double y_six = 2 * blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

					if ((x_one <= selection[0]) && (selection[0] <= x_three) && (y_one <= selection[1]) && (selection[1] <= y_six)) {
						column_row[0] = i;
						column_row[1] = j;
					}
				}
			}
			else {
				for (int j = 0; j < 2 * boardSize - 1 - (i - boardSize) - 1; j++) {
					// Co-ordinates for points in upper half of the board

					double x_one = blockSize / 2 + 2 * j*blockSize*cos(30 * PI / 180) - (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;
					double x_three = blockSize / 2 + 2 * blockSize*cos(30 * PI / 180) + 2 * j*blockSize*cos(30 * PI / 180) - (boardSize - (i + 1)) * blockSize*cos(30 * PI / 180) + horizonalSeperator * j;

					double y_one = blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;
					double y_six = 2 * blockSize + 300 - (boardSize - i) * blockSize - (boardSize - (i + 1)) * blockSize*sin(30 * PI / 180) + (i + 1)  * verticalSeperator;

					if ((x_one <= selection[0]) && (selection[0] <= x_three) && (y_one <= selection[1]) && (selection[1] <= y_six)) {
						column_row[0] = i;
						column_row[1] = j;
					}
				}
			}
	}
	return column_row;
}

int HexagonBoard::flipDiscsRight(int row, int column, int playerNumber, bool flipping) {
	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;
	int rowLength;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[row][column] == 0) {
		if (row <= (2 * boardSize - 1) / 2) {
			rowLength = row + boardSize;
		}
		else {
			rowLength = 2 * boardSize - 1 - (row - boardSize) - 1;
		}	
		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

		// Whether a disc of the same colour has been found in the row to the right from the selected square
		bool foundDiscRight = false;

		// Initialises the column right to equal column, so that it can count from column 0 up to column 4 for example
		int columnRight = column;

		// To check whether a gap has been found to the right - if it has, no discs will get flipped
		bool gapRight = false;

		// Only loops while : No discs have been found to the right
		//					: The right edge of the board has not been reached
		//					: There are no gaps to the right
		while ((!foundDiscRight) && (columnRight < rowLength - 1) && (!gapRight)) {
			// Counts up the columns. For e.g. - counts from column 0 - column 1 - ... - column 6
			columnRight += 1;



			// If the square belongs to the current player, then it has found a disc of the same colour to the right
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[row][columnRight] == playerNumber) {
				foundDiscRight = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[row][columnRight] == 0) || (othelloBoard[columnRight][row] == 2)) {
				gapRight = true;
			}
		}

		// If a disc of the same colour was found to the right, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc to the right must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is right of it
		if (foundDiscRight) {
			for (int convertingRight = column + 1; convertingRight < columnRight; convertingRight += 1) {
				if (flipping) {
					othelloBoard[row][convertingRight] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	
return	numberOfDiscsFlipped;
}

int HexagonBoard::flipDiscsLeft(int row, int column, int playerNumber, bool flipping) {

	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[row][column] == 0) {

		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

		// Whether a disc of the same colour has been found in the row to the left from the selected square
		bool foundDiscLeft = false;

		// Initialises the column left to equal row, so that it can count from column 4 down to column 0 for example
		int columnLeft = column;

		// To check whether a gap has been found to the left - if it has, no discs will get flipped
		bool gapLeft = false;

		// Only loops while : No discs have been found to the left
		//					: The left edge of the board has not been reached
		//					: There are no gaps to the left
		while ((!foundDiscLeft) && (columnLeft > 0) && (!gapLeft)) {
			// Counts down the column. For e.g. - counts from column 6 - column 5 - ... - column 0
			columnLeft -= 1;
			// If the square belongs to the current player, then it has found a disc of the same colour to the left
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[row][columnLeft] == playerNumber) {
				foundDiscLeft = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[row][columnLeft]) == 0 || (othelloBoard[row][columnLeft]) == 2) {
				gapLeft = true;
			}
		}

		// If a disc of the same colour was found to the lef, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc to the left must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is left of it
		if (foundDiscLeft) {
			for (int convertingLeft = columnLeft + 1; convertingLeft < column; convertingLeft += 1) {
				if (flipping) {
					othelloBoard[row][convertingLeft] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int HexagonBoard::flipDiscsLeftUp(int row, int column, int playerNumber, bool flipping)
{
	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[row][column] == 0) {
		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

		// Whether a disc of the same colour has been found in the diagonally left and up from the selected square
		bool foundDiscLeftUp = false;

		// Initialises the column left to equal column and row up to equal row, so that it can count diagonally left and up
		int columnLeft = column;
		int rowUp = row;

		// To check whether a gap has been found diagonally left and up - if it has, no discs will get flipped
		bool gapLeftUp = false;

		// Only loops while : No discs have been found diagonally left and down
		//					: The left or top edge of the board has not been reached
		//					: There are no gaps diagonally left and up
		while ((!foundDiscLeftUp) && (columnLeft > -1) && (!gapLeftUp) && (rowUp > 0)) {
			// Counts down the column and down the rows

			// When below top half, need to decrease both column and row, otherwise just row
			if (rowUp <= (2 * boardSize - 1) / 2) {
				columnLeft -= 1;
				rowUp -= 1;
			}
			else {
				rowUp -= 1;
			}
			// If the square belongs to the current player, then it has found a disc of the same colour diagonally left and up
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[rowUp][columnLeft] == playerNumber) {
				foundDiscLeftUp = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[rowUp][columnLeft] == 0) || (othelloBoard[rowUp][columnLeft] == 2)) {
				gapLeftUp = true;
			}
		}

		// If a disc of the same colour was found diagonally left and up, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc diagonally left and up must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is diagonally left and up of it
		if (foundDiscLeftUp) {
			while ((columnLeft < column - 1) || (rowUp < row - 1)) {
				if (rowUp < (2 * boardSize - 1) / 2) {
					columnLeft += 1;
					rowUp += 1;
				}
				else {
					rowUp += 1;
				}

				if (flipping) {
					othelloBoard[rowUp][columnLeft] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int HexagonBoard::flipDiscsLeftDown(int row, int column, int playerNumber, bool flipping)
{
	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[row][column] == 0) {

		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

		// Whether a disc of the same colour has been found in the diagonally left and down from the selected square
		bool foundDiscLeftDown = false;

		// Initialises the column left to equal column and row down to equal row, so that it can count diagonally left and down
		int columnLeft = column;
		int rowDown = row;

		// To check whether a gap has been found diagonally left and down - if it has, no discs will get flipped
		bool gapLeftDown = false;

		// Only loops while : No discs have been found diagonally left and down
		//					: The left or bottom edge of the board has not been reached
		//					: There are no gaps diagonally left and down
		while ((!foundDiscLeftDown) && (columnLeft > -1) && (!gapLeftDown) && (rowDown < 2*boardSize - 2)) {
			
			if (rowDown < (2 * boardSize - 1) / 2) {
				rowDown += 1;
			}
			else {
				rowDown += 1;
				columnLeft -= 1;
			}

			// If the square belongs to the current player, then it has found a disc of the same colour diagonally left and down
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[rowDown][columnLeft] == playerNumber) {
				foundDiscLeftDown = true;
			}
			// If the square = 0, then it is a gap
			else if ( (othelloBoard[rowDown][columnLeft] == 0) || (othelloBoard[rowDown][columnLeft] == 2) ) {
				gapLeftDown = true;
				
			}
		}

		// If a disc of the same colour was found diagonally left and down, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc diagonally left and down must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is diagonally left and down of it
		if (foundDiscLeftDown) {
			while ((column < columnLeft - 1) || (row < rowDown - 1)) {

				if (row < (2 * boardSize - 1) / 2) {
					row += 1;
				}
				else {
					row += 1;
					column -= 1;
				}
				if (flipping) {
					othelloBoard[row][column] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int HexagonBoard::flipDiscsRightUp(int row, int column, int playerNumber, bool flipping)
{
	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;
	int rowLength;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[row][column] == 0) {

		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

		if (row < (2 * boardSize - 1) / 2) {
			// Offsets the row length so the rightmost edge are checked for diagonally up, shouldnt have -1 at the end for actual length
			rowLength = row + boardSize - 1;
		}
		else {
			rowLength = 2 * boardSize - 1 - (row - boardSize) - 1;
		}

		// Whether a disc of the same colour has been found in the diagonally right and up from the selected square
		bool foundDiscRightUp = false;

		// Initialises the column right to equal column and row up to equal row, so that it can count diagonally right and up
		int columnRight = column;
		int rowUp = row;

		// To check whether a gap has been found diagonally right and up - if it has, no discs will get flipped
		bool gapRightUp = false;

		// Only loops while : No discs have been found diagonally right and up
		//					: The right or top edge of the board has not been reached
		//					: There are no gaps diagonally right and up
		while ((!foundDiscRightUp) && (columnRight < rowLength) && (!gapRightUp) && (rowUp > 0)) {
			// Counts up the column and down the rows

			if (rowUp <= (2 * boardSize - 1) / 2) {
				rowUp -= 1;
			}
			else {
				rowUp -= 1;
				columnRight += 1;
			}

			if (rowUp <= (2 * boardSize - 1) / 2) {
				rowLength = rowUp + boardSize;
			}
			else {
				rowLength = 2 * boardSize - 1 - (rowUp - boardSize) - 1;
			}
			// If the square belongs to the current player, then it has found a disc of the same colour diagonally right and up
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[rowUp][columnRight] == playerNumber) {
				foundDiscRightUp = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[rowUp][columnRight] == 0) || (othelloBoard[rowUp][columnRight] == 2)) {
				gapRightUp = true;
			}
		}

		// If a disc of the same colour was found diagonally right and up, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc diagonally right and up must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is diagonally right and up of it
		if (foundDiscRightUp) {
			while ((columnRight > column + 1) || (rowUp < row - 1)) {
				if (rowUp < (2 * boardSize - 1) / 2) {
					rowUp += 1;
				}
				else {
					rowUp += 1;
					columnRight -= 1;
				}
				if (flipping) {
					othelloBoard[rowUp][columnRight] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int HexagonBoard::flipDiscsRightDown(int row, int column, int playerNumber, bool flipping)
{
	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;
	int rowLength;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[row][column] == 0) {

		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

		if (row < (2 * boardSize - 1) / 2) {
			rowLength = row + boardSize;
		}
		else {
			// Subtract 2 at the end, usually just 1. This offsets the length for the bottom half so the rightest column dont search more
			rowLength = 2 * boardSize - 1 - (row - boardSize) - 2;
		}

		// Whether a disc of the same colour has been found in the diagonally right and down from the selected square
		bool foundDiscRightDown = false;

		// Initialises the column right to equal column and row down to equal row, so that it can count diagonally right and down
		int columnRight = column;
		int rowDown = row;

		// To check whether a gap has been found diagonally right and down - if it has, no discs will get flipped
		bool gapRightDown = false;

		// Only loops while : No discs have been found diagonally right and down
		//					: The right or bottom edge of the board has not been reached
		//					: There are no gaps diagonally right and down
		while ((!foundDiscRightDown) && (columnRight < rowLength) && (!gapRightDown) && (rowDown < 2 * boardSize - 2)) {
			// Counts up the column and up the rows
			if (rowDown < (2 * boardSize - 1) / 2) {
				rowDown += 1;
				columnRight += 1;
			}
			else {
				rowDown += 1;
			}

			if (rowDown <= (2 * boardSize - 1) / 2) {
				rowLength = rowDown + boardSize;
			}
			else {
				rowLength = 2 * boardSize - 1 - (rowDown - boardSize) - 1;
			}
			// If the square belongs to the current player, then it has found a disc of the same colour diagonally right and down
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[rowDown][columnRight] == playerNumber) {
				foundDiscRightDown = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[rowDown][columnRight] == 0) || (othelloBoard[rowDown][columnRight] == 2)) {
				gapRightDown = true;
			}
		}

		// If a disc of the same colour was found diagonally right and down, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc diagonally right and down must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is diagonally right and down of it
		if (foundDiscRightDown) {

			while ((column < columnRight - 1) || (row < rowDown - 1)) {
				if (row < (2 * boardSize - 1) / 2) {
					row += 1;
					column += 1;
				}
				else {
					row += 1;
				}
				if (flipping) {
					othelloBoard[row][column] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

bool HexagonBoard::hasEmpty() {
	bool hasEmpty = false;
	for (int i = 0; i < 2 * boardSize - 1; i++) {
		if (i <= (2 * boardSize - 1) / 2) {
			for (int j = 0; j < boardSize + i; j++) {
				if (othelloBoard[i][j] == 0) {
					hasEmpty = true;
				}
			}
		}
		else {
			for (int j = 0; j < 2 * boardSize - 1 - (i - boardSize) - 1; j++) {
				if (othelloBoard[i][j] == 0) {
					hasEmpty = true;
				}
			}
		}
	}
	return hasEmpty;
}

int HexagonBoard::numberOfAvailableMoves(int playerNumber) {

	int numberOfAvailableMoves = 0;
	bool flipped;

	for (int row = 0; row < 2 * boardSize - 1; row++) {
		if (row <= (2 * boardSize - 1) / 2) {
			for (int column = 0; column < boardSize + row; column++) {
				flipped = false;
				// ------------------------
				if (flipDiscsLeft(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Left - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsRight(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Right - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsLeftUp(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Left up - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsLeftDown(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Left down - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsRightUp(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Right up - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsRightDown(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Right down - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}
				// ------------------------
			}
		}
		else {
			for (int column = 0; column < 2 * boardSize - 1 - (row - boardSize) - 1; column++) {
				flipped = false;
				// ------------------------
				if (flipDiscsLeft(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Left - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsRight(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Right - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsLeftUp(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Left up - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsLeftDown(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Left down - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsRightUp(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Right up - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}

				if (flipDiscsRightDown(row, column, playerNumber, false) && (!flipped)) {
					numberOfAvailableMoves += 1;
					//cout << "Right down - Row = " << row + 1 << " and column = " << column + 1 << endl;
					flipped = true;
				}
				// ------------------------
			}
		}
	}
	return numberOfAvailableMoves;
}

void HexagonBoard::calculateWinner() {

	int playerOneScore = 0;
	int playerMinusOneScore = 0;
	// Loop through the array of arrays for the discs

	for (int i = 0; i < 2 * boardSize - 1; i++) {
		if (i <= (2 * boardSize - 1) / 2) {
			for (int j = 0; j < boardSize + i; j++) {
				if (othelloBoard[i][j] == 1) {
					playerOneScore += 1;
				}
				else if (othelloBoard[i][j] == -1) {
					playerMinusOneScore += 1;
				}
			}
		}
		else {
			for (int j = 0; j < 2 * boardSize - 1 - (i - boardSize) - 1; j++) {
				if (othelloBoard[i][j] == 1) {
					playerOneScore += 1;
				}
				else if (othelloBoard[i][j] == -1) {
					playerMinusOneScore += 1;
				}
			}
		}
	}
	cout << "Player 1 (white) has a score of " << playerOneScore << " , player 2 (black) has a score of " << playerMinusOneScore << endl;

	if (playerOneScore > playerMinusOneScore) {
		cout << "Player 1 (white) is the winner!" << endl;
	}
	else if (playerOneScore < playerMinusOneScore) {
		cout << "Player 2 (black) is the winner!" << endl;
	}
	else {
		cout << "It was a draw!" << endl;
	}
}