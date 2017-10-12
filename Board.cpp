#include "Board.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include <string> 

using namespace cv;
using namespace std;

// @block   : Determines the size of each square
// @squares : Determines what size the square board is
// @offset  : Determines the size of the background area
// Create a new board object
Board::Board(int block, int squares, int off)
{
	// Set the board dimensions
	blockSize = block;
	boardSize = squares;
	offset = off;
	imageSize = block * squares;

	// Initialise the disc arrays
	othelloBoard = new int*[squares];
	for (int i = 0; i < squares; i++) {
		othelloBoard[i] = new int[squares];
	}

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			othelloBoard[i][j] = 0;
		}
	}

	// Initialise the middle discs (middle based off the boardSize)
	othelloBoard[boardSize / 2 - 1][boardSize / 2 - 1] = 1;
	othelloBoard[boardSize / 2][boardSize / 2] = 1;

	othelloBoard[boardSize / 2 - 1][boardSize / 2] = -1;
	othelloBoard[boardSize / 2][boardSize / 2 - 1] = -1;
}


// Dealloacates a board object
Board::~Board()
{
	// all deallocating is done automatically

	//delete othelloBoard;
}

void Board::deleteBoard() {
	for (int i = 0; i < boardSize; i++) {
		delete othelloBoard[i];
	}
	delete othelloBoard;
}

// Returns an array of arrays representing the current board states
int** Board::getBoard() {
	return othelloBoard;
}

// Prints out one row per line in the terminal of the board values
void Board::printBoardValues() {
	for (int x = 0; x < boardSize; x += 1) {
		for (int y = 0; y < boardSize; y += 1) {
			cout << othelloBoard[y][x] << ", ";
		}
		cout << endl;
	}
	cout << endl << endl;
}

// Sets a disc to be an empty square, can't be played on
void Board::setDiscUnplayable(int row, int column) {
	othelloBoard[column][row] = 2;
}

void Board::setDiscEmpty(int row, int column) {
	othelloBoard[column][row] = 0;
}

void Board::setInitialSquares() {
	othelloBoard[boardSize / 2 - 1][boardSize / 2 - 1] = 1;
	othelloBoard[boardSize / 2][boardSize / 2] = 1;

	othelloBoard[boardSize / 2 - 1][boardSize / 2] = -1;
	othelloBoard[boardSize / 2][boardSize / 2 - 1] = -1;
}

// Draws a red dot in places that the current player can play in
Mat Board::drawAvailablePositions(int playerNumber, Mat currentBoard) {

	for (int column = 0; column < boardSize; column++) {
		for (int row = 0; row < boardSize; row++)
		{
			// Draws a red dot in the available positions to play in
			if (othelloBoard[column][row] == 0) {
				int totalNumberFlipped = 0;
				int above = flipDiscsAbove(row, column, playerNumber, false);
				int below = flipDiscsBelow(row, column, playerNumber, false);
				int left = flipDiscsLeft(row, column, playerNumber, false);
				int right = flipDiscsRight(row, column, playerNumber, false);

				int leftUp = flipDiscsLeftUp(row, column, playerNumber, false);
				int leftDown = flipDiscsLeftDown(row, column, playerNumber, false);
				int rightUp = flipDiscsRightUp(row, column, playerNumber, false);
				int rightDown = flipDiscsRightDown(row, column, playerNumber, false);

				totalNumberFlipped = above + below + left + right + leftUp + leftDown + rightUp + rightDown;
				if (totalNumberFlipped) {
					circle(currentBoard, Point(offset / 2 + (2 * column + 1)*blockSize / 2, offset / 2 + (2 * row + 1)*blockSize / 2), 4, Scalar(0, 0, 255), CV_FILLED, 8, 0);
				}
			}
		}
	} 

	return currentBoard;
}



// @emptyBoard : An image of a board with no discs drawn on it
// @boardWithDiscs : An array of arrays, representing the disc locations
// Takes the empty board as a parameter and draws on all of the discs, which are given as the 2nd parameter
Mat Board::drawDiscs(Mat emptyBoard, int** boardWithDiscs) {

	// Sets where the co-ordinate values are placed
	int openingWidth = 800;
	int openingHeight = 500;

	double vertical = 0;

	if (boardSize == 4) {
		vertical = 0.2;
	}
	else if (boardSize == 6) {
		vertical = 0.3;
	}
	else if (boardSize == 8) {
		vertical = 0.3;
	}
	else if (boardSize == 10) {
		vertical = 0.4;
	}

	// Loop through the array of arrays for the discs
	for (int i = 0; i < boardSize; i++)	{
		for (int j = 0; j < boardSize; j++)
		{
			// If white, draw a white circle in the scaled position
			if (othelloBoard[i][j] == 1) {
				circle(emptyBoard, Point(offset / 2 + (2 * i + 1)*blockSize / 2, offset / 2 + (2 * j + 1)*blockSize / 2), 2 * blockSize / 5, Scalar(255, 255, 255), CV_FILLED, 8, 0);
			}
			// If black, draw a black circle in the scaled position
			else if (othelloBoard[i][j] == -1) {
				circle(emptyBoard, Point(offset / 2 + (2 * i + 1)*blockSize / 2, offset / 2 + (2 * j + 1)*blockSize / 2), 2 * blockSize / 5, Scalar(0, 0, 0), CV_FILLED, 8, 0);
			}
			// If unavailable, draw a grey square in the scaled position to match background grey (100,100,100)
			else if (othelloBoard[i][j] == 2) {
				rectangle(emptyBoard, Point(offset / 2 + i*blockSize - 1 + 2, offset / 2 + j*blockSize + 2), Point(offset / 2 + (i + 1)*blockSize - 2, offset / 2 + (j + 1)*blockSize - 2), Scalar(100, 100, 100), CV_FILLED, 8, 0);
			}
			// Otherwise leave green
			// -------------------------------------

			// The co-ordinate
			string textLineOne = to_string(j) + "," + to_string(i);

			// Font, size and thickness settings
			int fontFace = FONT_HERSHEY_COMPLEX;
			double fontScale = 0.4;
			int thickness = 1;

			// Location of where to put the test (done across 2 lines)
			Point textOrgLineOne(offset / 2 + (2 * i)*blockSize / 2 + blockSize / 16, offset / 2 + (2 * j + vertical)*blockSize / 2);

			// Puts the text in the image, asking the question
			putText(emptyBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
			// -------------------------------------
		}
	}
	return emptyBoard;
}

// Draws an empty board, then calls the drawDiscs function to add in the discs
Mat Board::drawBoard() {
	offset = 100;
	Mat clearBoard(imageSize + offset, imageSize + offset, CV_8UC3, Scalar::all(100));
	// Determine the color of the board. Channel 1 = green component
	color.val[0] = 50;
	color.val[1] = 100;
	color.val[2] = 0;

	// Sets the whole board green
	for (int y = 0 + offset / 2; y<imageSize + offset / 2; y++)
	{
		for (int x = 0 + offset / 2; x<imageSize + offset / 2; x++)
		{
			clearBoard.at<Vec3b>(Point(x, y)) = color;
		}
	}

	// Set the color to black for the edge lines
	color.val[0] = color.val[1] = color.val[2] = 0;

	// Draws the horizontal black lines (2 pixels thick)
	for (int y = blockSize + offset / 2; y<imageSize + offset / 2; y = y + blockSize)
	{
		for (int x = 0 + offset / 2; x<imageSize + offset / 2; x++)
		{
			clearBoard.at<Vec3b>(Point(x, y)) = color;
			clearBoard.at<Vec3b>(Point(x, y - 1)) = color;
		}
	}

	// Draw the vertical black lines (2 pixels thick)
	for (int x = blockSize + offset / 2; x<imageSize + offset / 2; x = x + blockSize)
	{
		for (int y = 0 + offset / 2; y<imageSize + offset / 2; y++)
		{
			clearBoard.at<Vec3b>(Point(x, y)) = color;
			clearBoard.at<Vec3b>(Point(x - 1, y)) = color;
		}
	}

	// Draws the edges lines black (top and bottom)
	for (int x = 0 + offset / 2; x < imageSize + offset / 2; x += 1) {
		clearBoard.at<Vec3b>(Point(x, 0 + offset / 2)) = color;
		clearBoard.at<Vec3b>(Point(x, 1 + offset / 2)) = color;

		clearBoard.at<Vec3b>(Point(x, imageSize - 1 + offset / 2)) = color;
		clearBoard.at<Vec3b>(Point(x, imageSize - 2 + offset / 2)) = color;
	}

	// Draws the edge lines black (left and right)
	for (int y = 0 + offset / 2; y < imageSize + offset / 2; y += 1) {
		clearBoard.at<Vec3b>(Point(0 + offset / 2, y)) = color;
		clearBoard.at<Vec3b>(Point(1 + offset / 2, y)) = color;

		clearBoard.at<Vec3b>(Point(imageSize - 1 + offset / 2, y)) = color;
		clearBoard.at<Vec3b>(Point(imageSize - 2 + offset / 2, y)) = color;
	}
	// Function that draws the disc values into the board too
	Mat boardWithDiscs = drawDiscs(clearBoard, othelloBoard);

	//boardWithDiscs = drawAvailablePositions(-1, boardWithDiscs);

	return boardWithDiscs;
}

// @row		     : The selected row (0-7)
// @column		 : The selected column (0-7)
// @playerNumber : Whether it is player 1 or 2
bool Board::placeDisc(int row, int column, int playerNumber) {
	// Returns true if placing a disc there flips any discs
	int totalFlips = flipDiscs(row, column, playerNumber);

	if (totalFlips > 0) {
		if (playerNumber == 1) {
			othelloBoard[column][row] = 1;
		}
		else if (playerNumber == -1) {
			othelloBoard[column][row] = -1;
		}
	}

	if (totalFlips > 0) {
		return true;
	}
	else {
		return false;
	}
}

// Takes an array of arrays representing the tile values
// Sets the board to have those values
void Board::setBoardValues(int ** newBoardValues)
{
	for (int x = 0; x < boardSize; x += 1) {
		for (int y = 0; y < boardSize; y += 1) {
				othelloBoard[x][y] = newBoardValues[x][y];
		}
	}
}

// @selection : A 2 values array, containing the selected pixels
// This converts the selected pixels to the correct row / column location on the board
int * Board::scalePixelSelection(int * selection)
{

	// An array of 2 values to return the correct row / column values
	int* column_row = new int[2];

	// Subtract half the offset, then it divides by the quotient of the block size (i.e. 175 / 50 = 3)
	column_row[1] = (selection[0] - offset / 2) / blockSize;
	column_row[0] = (selection[1] - offset / 2) / blockSize;

	return column_row;
}

int Board::flipDiscs(int row, int column, int playerNumber) {
	// Returns total number of discs flipped by placing a disc at the specified row / column
	int totalNumberFlipped = 0;
	if (othelloBoard[column][row] == 0) {
		int above = flipDiscsAbove(row, column, playerNumber, true);
		int below = flipDiscsBelow(row, column, playerNumber, true);
		int left = flipDiscsLeft(row, column, playerNumber, true);
		int right = flipDiscsRight(row, column, playerNumber, true);

		int leftUp = flipDiscsLeftUp(row, column, playerNumber, true);
		int leftDown = flipDiscsLeftDown(row, column, playerNumber, true);
		int rightUp = flipDiscsRightUp(row, column, playerNumber, true);
		int rightDown = flipDiscsRightDown(row, column, playerNumber, true);

		totalNumberFlipped = above + below + left + right + leftUp + leftDown + rightUp + rightDown;
	}
	else {
		totalNumberFlipped = 0;
	}
	return totalNumberFlipped;
}

// Return 1 if white wins. Returns -1 if black wins. Returns 0 if draw.
int Board::calculateWinner() {

	int playerOneScore = 0;
	int playerMinusOneScore = 0;
	// Loop through the array of arrays for the discs
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {

			if (othelloBoard[j][i] == 1) {
				playerOneScore += 1;
			}
			else if (othelloBoard[j][i] == -1) {
				playerMinusOneScore += 1;
			}
		}
	}
	//	cout << "Player 1 (white) has a score of " << playerOneScore << " , player 2 (black) has a score of " << playerMinusOneScore << endl;

	if (playerOneScore > playerMinusOneScore) {
		//		cout << "Player 1 (white) is the winner!" << endl;
		return 1;
	}
	else if (playerOneScore < playerMinusOneScore) {
		//		cout << "Player 2 (black) is the winner!" << endl;
		return -1;
	}
	else {
		//		cout << "It was a draw!" << endl;
		return 0;
	}
}

int Board::printWinner() {
	int playerOneScore = 0;
	int playerMinusOneScore = 0;
	// Loop through the array of arrays for the discs
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {

			if (othelloBoard[j][i] == 1) {
				playerOneScore += 1;
			}
			else if (othelloBoard[j][i] == -1) {
				playerMinusOneScore += 1;
			}
		}
	}

	cout << "Player 1 (white) has a score of " << playerOneScore << " , player 2 (black) has a score of " << playerMinusOneScore << endl;

	if (playerOneScore > playerMinusOneScore) {
		//cout << "White won : " << playerOneScore << " - " << playerMinusOneScore << endl;
		//cout << "Player 1 (white) is the winner!" << endl;
		return 1;
	}
	else if (playerOneScore < playerMinusOneScore) {
		//cout << "Black won : " << playerMinusOneScore << " - " << playerOneScore << endl;
		//cout << "Player 2 (black) is the winner!" << endl;
		return -1;
	}
	else {
		//cout << "Draw : " << playerOneScore << " - " << playerMinusOneScore << endl;
		//cout << "It was a draw!" << endl;
		return 0;
	}
}

/// How the flip functions work -
/// Find the closest disc of their own colour in each direction
/// Flip everything between them if there are no gaps

// These functions all flip discs in their respective directions
int Board::flipDiscsAbove(int row, int column, int playerNumber, bool flipping)	 {

	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[column][row] == 0) {

		// Calculates the number of discs that get flipped - needs to be > 0 for valid move		

		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

		// Whether a disc of the same colour has been found in the column upwards from the selected square
		bool foundDiscAbove = false;

		// Initialises the row above to equal row, so that it can count from row 4 down to row 0 for example
		int rowAbove = row;

		// To check whether a gap has been found above - if it has, no discs will get flipped
		bool gapAbove = false;

		// Only loops while : No discs have been found above
		//					: The top of the board has not been reached
		//					: There are no gaps above
		while ((!foundDiscAbove) && (rowAbove > 0) && (!gapAbove)) {

			// Counts down the rows. For e.g. - counts from row 6 - row 5 - row 4 ... - row 0
			rowAbove -= 1;

			// If the square belongs to the current player, then it has found a disc of the same colour above
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[column][rowAbove] == playerNumber) {
				foundDiscAbove = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[column][rowAbove]) == 0 || (othelloBoard[column][rowAbove] == 2)) {
				gapAbove = true;
			}
		}

		// If a disc of the same colour was found above, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc above must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is above it
		if (foundDiscAbove) {
			for (int convertingAbove = rowAbove + 1; convertingAbove < row; convertingAbove += 1) {
				if (flipping) {
					othelloBoard[column][convertingAbove] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int Board::flipDiscsBelow(int row, int column, int playerNumber, bool flipping) {

	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[column][row] == 0) {

		// Calculates the number of discs that get flipped - needs to be > 0 for valid move

		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

		// Whether a disc of the same colour has been found in the column downwards from the selected square
		bool foundDiscBelow = false;

		// Initialises the row above to equal row, so that it can count from row 0 up to row 4 for example
		int rowBelow = row;

		// To check whether a gap has been found below - if it has, no discs will get flipped
		bool gapBelow = false;

		// Only loops while : No discs have been found below
		//					: The bottom of the board has not been reached
		//					: There are no gaps below
		while ((!foundDiscBelow) && (rowBelow < boardSize - 1) && (!gapBelow)) {
			// Counts down the rows. For e.g. - counts from row 5 - row 4 - ... row 1
			rowBelow += 1;
			// If the square belongs to the current player, then it has found a disc of the same colour below
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[column][rowBelow] == playerNumber) {
				foundDiscBelow = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[column][rowBelow] == 0) || (othelloBoard[column][rowBelow] == 2)) {
				gapBelow = true;
			}
		}
		// If a disc of the same colour was found below, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc above must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is below it
		if (foundDiscBelow) {
			for (int convertingBelow = row + 1; convertingBelow < rowBelow; convertingBelow += 1) {
				if (flipping) {
					othelloBoard[column][convertingBelow] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int Board::flipDiscsLeft(int row, int column, int playerNumber, bool flipping) {

	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[column][row] == 0) {

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
			if (othelloBoard[columnLeft][row] == playerNumber) {
				foundDiscLeft = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[columnLeft][row]) == 0 || (othelloBoard[columnLeft][row]) == 2) {
				gapLeft = true;
			}
		}

		// If a disc of the same colour was found to the lef, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc to the left must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is left of it
		if (foundDiscLeft) {
			for (int convertingLeft = columnLeft + 1; convertingLeft < column; convertingLeft += 1) {
				if (flipping) {
					othelloBoard[convertingLeft][row] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int Board::flipDiscsRight(int row, int column, int playerNumber, bool flipping) {

	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[column][row] == 0) {

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
		while ((!foundDiscRight) && (columnRight < boardSize - 1) && (!gapRight)) {

			// Counts up the columns. For e.g. - counts from column 0 - column 1 - ... - column 6
			columnRight += 1;

			// If the square belongs to the current player, then it has found a disc of the same colour to the right
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[columnRight][row] == playerNumber) {
				foundDiscRight = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[columnRight][row] == 0) || (othelloBoard[columnRight][row] == 2)) {
				gapRight = true;
			}
		}

		// If a disc of the same colour was found to the right, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc to the right must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is right of it
		if (foundDiscRight) {
			for (int convertingRight = column + 1; convertingRight < columnRight; convertingRight += 1) {
				if (flipping) {
					othelloBoard[convertingRight][row] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int Board::flipDiscsLeftUp(int row, int column, int playerNumber, bool flipping)
{
	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[column][row] == 0) {

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
		while ((!foundDiscLeftUp) && (columnLeft > 0) && (!gapLeftUp) && (rowUp > 0)) {

			// Counts down the column and down the rows
			columnLeft -= 1;
			rowUp -= 1;

			// If the square belongs to the current player, then it has found a disc of the same colour diagonally left and up
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[columnLeft][rowUp] == playerNumber) {
				foundDiscLeftUp = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[columnLeft][rowUp] == 0) || (othelloBoard[columnLeft][rowUp] == 2)) {
				gapLeftUp = true;
			}
		}

		// If a disc of the same colour was found diagonally left and up, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc diagonally left and up must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is diagonally left and up of it
		if (foundDiscLeftUp) {

			while ((columnLeft < column - 1) && (rowUp < row - 1)) {
				columnLeft += 1;
				rowUp += 1;
				if (flipping) {
					othelloBoard[columnLeft][rowUp] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int Board::flipDiscsLeftDown(int row, int column, int playerNumber, bool flipping)
{
	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[column][row] == 0) {

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
		while ((!foundDiscLeftDown) && (columnLeft > 0) && (!gapLeftDown) && (rowDown < boardSize - 1)) {

			// Counts down the column and up the rows
			columnLeft -= 1;
			rowDown += 1;

			// If the square belongs to the current player, then it has found a disc of the same colour diagonally left and down
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[columnLeft][rowDown] == playerNumber) {
				foundDiscLeftDown = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[columnLeft][rowDown] == 0) || (othelloBoard[columnLeft][rowDown] == 2)) {
				gapLeftDown = true;
			}
		}

		// If a disc of the same colour was found diagonally left and down, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc diagonally left and down must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is diagonally left and down of it
		if (foundDiscLeftDown) {

			while ((columnLeft < column - 1) && (rowDown > row + 1)) {
				columnLeft += 1;
				rowDown -= 1;
				if (flipping) {
					othelloBoard[columnLeft][rowDown] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int Board::flipDiscsRightUp(int row, int column, int playerNumber, bool flipping)
{
	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[column][row] == 0) {

		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

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
		while ((!foundDiscRightUp) && (columnRight < boardSize - 1) && (!gapRightUp) && (rowUp > 0)) {

			// Counts up the column and down the rows
			columnRight += 1;
			rowUp -= 1;

			// If the square belongs to the current player, then it has found a disc of the same colour diagonally right and up
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[columnRight][rowUp] == playerNumber) {
				foundDiscRightUp = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[columnRight][rowUp] == 0) || (othelloBoard[columnRight][rowUp] == 2)) {
				gapRightUp = true;
			}
		}

		// If a disc of the same colour was found diagonally right and up, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc diagonally right and up must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is diagonally right and up of it
		if (foundDiscRightUp) {

			while ((columnRight > column + 1) && (rowUp < row - 1)) {
				columnRight -= 1;
				rowUp += 1;
				if (flipping) {
					othelloBoard[columnRight][rowUp] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

int Board::flipDiscsRightDown(int row, int column, int playerNumber, bool flipping)
{
	// Calculates the number of discs that get flipped - needs to be > 0 for valid move
	int numberOfDiscsFlipped = 0;

	// Only checks all of the information if the board location is empty
	if (othelloBoard[column][row] == 0) {

		// Player 1 = white
		// Player -1 = black

		// Player numbers are 1 and -1, so multiplying by -1 gives player number of opponent
		int otherPlayerNumber = playerNumber * -1;

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
		while ((!foundDiscRightDown) && (columnRight < boardSize - 1) && (!gapRightDown) && (rowDown < boardSize - 1)) {

			// Counts up the column and up the rows
			columnRight += 1;
			rowDown += 1;

			// If the square belongs to the current player, then it has found a disc of the same colour diagonally right and down
			// player 1 (white) has its values in the array stored as 1
			// player 2 (black) has its values in the array stored as 2
			if (othelloBoard[columnRight][rowDown] == playerNumber) {
				foundDiscRightDown = true;
			}
			// If the square = 0, then it is a gap
			else if ((othelloBoard[columnRight][rowDown] == 0) || (othelloBoard[columnRight][rowDown] == 2)) {
				gapRightDown = true;
			}
		}

		// If a disc of the same colour was found diagonally right and down, that means there were also no gaps
		// Then all of the discs inbetween the newly placed disc and the closest disc diagonally right and down must all be the other players
		// Flips all of the discs inbetween the newly placed and the closest one of the same colour that is diagonally right and down of it
		if (foundDiscRightDown) {

			while ((columnRight > column + 1) && (rowDown > row + 1)) {
				columnRight -= 1;
				rowDown -= 1;
				if (flipping) {
					othelloBoard[columnRight][rowDown] = playerNumber;
				}
				numberOfDiscsFlipped += 1;
			}
		}
	}
	return numberOfDiscsFlipped;
}

// Checks if the board has empty squares - true if it does
bool Board::hasEmpty() {
	bool hasEmpty = false;
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if (othelloBoard[i][j] == 0) {
				hasEmpty = true;
			}
		}
	}
	return hasEmpty;
}

// Calculates how many available moves there are for the given player number
int Board::numberOfAvailableMoves(int playerNumber) {
	//cout << "Listing row / column for player " << playerNumber << endl << endl;
	int numberOfAvailableMoves = 0;
	bool flipped;

	for (int row = 0; row < boardSize; row++) {
		for (int column = 0; column < boardSize; column++) {			

			// Checked to stop the same move being added as a potential move twice
			flipped = false;

				if (othelloBoard[column][row] == 0) {

					if (flipDiscsAbove(row, column, playerNumber, false) && (!flipped)) {
						numberOfAvailableMoves += 1;
						//cout << "Above - Row = " << row + 1 << " and column = " << column + 1 << endl;
						flipped = true;
					}

					if (flipDiscsBelow(row, column, playerNumber, false) && (!flipped)) {
						numberOfAvailableMoves += 1;
						//cout << "Below - Row = " << row + 1 << " and column = " << column + 1 << endl;
						flipped = true;
					}

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
			}
		}
	}
	//cout << "Number of moves for player " << playerNumber << " = " << numberOfAvailableMoves << endl;
	return numberOfAvailableMoves;
}

// Returns an array of the available moves given the player number (needs to know the number of available moves to generate the array correctly)
int** Board::availableMoveLocations(int playerNumber, int numberOfMoves) {
	//cout << "Listing row / column for player " << playerNumber << endl << endl;
	bool flipped;

	// An array of arrays, storing the available positions you can play in
	int** playableLocations = new int*[numberOfMoves];

	// Initialises all of the small arrays to a length of 2 (row / column)
	for (int i = 0; i < numberOfMoves; i++) {
		playableLocations[i] = new int[2];
	}

	// Decides which place to index in the array storing all of the available moves
	int moveNumber = 0;

	for (int row = 0; row < boardSize; row++) {
		for (int column = 0; column < boardSize; column++) {

			// Checked to stop the same move being added as a potential move twice
			flipped = false;

			if (flipDiscsAbove(row, column, playerNumber, false) && (!flipped)) {
				//cout << "Above - Row = " << row + 1 << " and column = " << column + 1 << endl;
				flipped = true;
				playableLocations[moveNumber][0] = row;
				playableLocations[moveNumber][1] = column;
				moveNumber += 1;
			}

			if (flipDiscsBelow(row, column, playerNumber, false) && (!flipped)) {
				//cout << "Below - Row = " << row + 1 << " and column = " << column + 1 << endl;
				flipped = true;
				playableLocations[moveNumber][0] = row;
				playableLocations[moveNumber][1] = column;
				moveNumber += 1;
			}

			if (flipDiscsLeft(row, column, playerNumber, false) && (!flipped)) {
				//cout << "Left - Row = " << row + 1 << " and column = " << column + 1 << endl;
				flipped = true;
				playableLocations[moveNumber][0] = row;
				playableLocations[moveNumber][1] = column;
				moveNumber += 1;
			}

			if (flipDiscsRight(row, column, playerNumber, false) && (!flipped)) {
				//cout << "Right - Row = " << row + 1 << " and column = " << column + 1 << endl;
				flipped = true;
				playableLocations[moveNumber][0] = row;
				playableLocations[moveNumber][1] = column;
				moveNumber += 1;
			}

			if (flipDiscsLeftUp(row, column, playerNumber, false) && (!flipped)) {
				//cout << "Left up - Row = " << row + 1 << " and column = " << column + 1 << endl;
				flipped = true;
				playableLocations[moveNumber][0] = row;
				playableLocations[moveNumber][1] = column;
				moveNumber += 1;
			}

			if (flipDiscsLeftDown(row, column, playerNumber, false) && (!flipped)) {
				//cout << "Left down - Row = " << row + 1 << " and column = " << column + 1 << endl;
				flipped = true;
				playableLocations[moveNumber][0] = row;
				playableLocations[moveNumber][1] = column;
				moveNumber += 1;
			}

			if (flipDiscsRightUp(row, column, playerNumber, false) && (!flipped)) {
				//cout << "Right up - Row = " << row + 1 << " and column = " << column + 1 << endl;
				flipped = true;
				playableLocations[moveNumber][0] = row;
				playableLocations[moveNumber][1] = column;
				moveNumber += 1;
			}

			if (flipDiscsRightDown(row, column, playerNumber, false) && (!flipped)) {
				//cout << "Right down - Row = " << row + 1 << " and column = " << column + 1 << endl;
				flipped = true;
				playableLocations[moveNumber][0] = row;
				playableLocations[moveNumber][1] = column;
				moveNumber += 1;
			}
		}
	}
	return playableLocations;
}








