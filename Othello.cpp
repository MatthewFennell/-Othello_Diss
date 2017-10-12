#include "Board.h"
#include "HexagonBoard.h"
#include "Player.h"
#include "HexagonPlayer.h"
#include "HexagonBoard.h"
#include "MonteCarlo.h"
#include "MiniMax.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include <time.h> 
#include "Genetic.h"
#include <ctime>

using namespace cv;
using namespace std;

// Handles a mouse button being pressed, returning what area was pressed
void mouseHandler(int event, int x, int y, int flags, void * param)
{
	if (event == CV_EVENT_LBUTTONDOWN) {
		Point* ptPtr = (Point*)param;
		ptPtr->x = x;
		ptPtr->y = y;
	}
}

// Hexagons or Squares (1 = Hexagon)
int chooseGameMode() {
	// Width and height of window to start a game
	int openingWidth = 800;
	int openingHeight = 500;

	// Creates starting screen image, colored dark green
	Mat gameStarter = Mat(openingHeight, openingWidth, CV_8UC3, Scalar(50, 100, 0));

	// Text to be displayed
	string textLineOne = "What game mode would";
	string textLineTwo = "you like to play?";

	// Font, size and thickness settings
	int fontFace = FONT_HERSHEY_COMPLEX;
	double fontScale = 1.5;
	int thickness = 4;

	// Location of where to put the test (done across 2 lines)
	Point textOrgLineOne(openingWidth / 9, openingHeight / 6);
	Point textOrgLineTwo(1.9* openingWidth / 9, 1.75 * openingHeight / 6);

	// Puts the text in the image, asking the question
	putText(gameStarter, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
	putText(gameStarter, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

	// Displays the various board sizes available
	putText(gameStarter, "Square", Point(150, 275), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);
	putText(gameStarter, "Hexagonal", Point(450, 275), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);


	// Initial co-ordinates that are out of range of the image
	Point2i pt(-1, -1);

	imshow("Starting game...", gameStarter);

	setMouseCallback("Starting game...", mouseHandler, (void*)&pt);

	// To loop until board is selected
	bool boardSizeSelected = false;

	// Variable for which board size is used
	int boardSelected = 0;

	// Loops until a size is selected
	while (!boardSizeSelected) {
		// Selecting board sizes from 4x4, 6x6, 8x8, 10x10, 12x12 and 14x14
		if ((140 < pt.x) && (pt.x <  350) && (235 < pt.y) && (pt.y < 290)) {
			boardSelected = 0;
			boardSizeSelected = true;
		}
		else if ((425 < pt.x) && (pt.x < 700) && (235 < pt.y) && (pt.y < 290)) {
			boardSelected = 1;
			boardSizeSelected = true;
		}
		waitKey(50);
	}
	destroyWindow("Starting game...");
	return boardSelected;
}

// Shows an image asking the user to select 1 of several board sizes and returns that value
int chooseGameBoardSizeSquares() {
	// Width and height of window to start a game
	int openingWidth = 800;
	int openingHeight = 500;

	// Creates starting screen image, colored dark green
	Mat gameStarter = Mat(openingHeight, openingWidth, CV_8UC3, Scalar(50, 100, 0));

	// Text to be displayed
	string textLineOne = "What sized board would";
	string textLineTwo = "you like to play on?";

	// Font, size and thickness settings
	int fontFace = FONT_HERSHEY_COMPLEX;
	double fontScale = 1.5;
	int thickness = 4;

	// Location of where to put the test (done across 2 lines)
	Point textOrgLineOne(openingWidth / 9, openingHeight / 6);
	Point textOrgLineTwo(1.5* openingWidth / 9, 1.75 * openingHeight / 6);

	// Puts the text in the image, asking the question
	putText(gameStarter, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
	putText(gameStarter, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

	// Displays the various board sizes available
	putText(gameStarter, "4x4", Point(150, 275), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);
	putText(gameStarter, "6x6", Point(350, 275), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);
	putText(gameStarter, "8x8", Point(550, 275), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);

	putText(gameStarter, "10x10", Point(125, 400), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);
	putText(gameStarter, "12x12", Point(325, 400), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);
	putText(gameStarter, "14x14", Point(525, 400), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);

	// Initial co-ordinates that are out of range of the image
	Point2i pt(-1, -1);

	imshow("Starting game...", gameStarter);

	setMouseCallback("Starting game...", mouseHandler, (void*)&pt);

	// To loop until board is selected
	bool boardSizeSelected = false;

	// Variable for which board size is used
	int boardSelected = 0;

	// Loops until a size is selected
	while (!boardSizeSelected) {
		// Selecting board sizes from 4x4, 6x6, 8x8, 10x10, 12x12 and 14x14
		if ((140 < pt.x) && (pt.x <  260) && (235 < pt.y) && (pt.y < 290)) {
			boardSelected = 4;
			boardSizeSelected = true;
		}
		else if ((340 < pt.x) && (pt.x <  460) && (235 < pt.y) && (pt.y < 290)) {
			boardSelected = 6;
			boardSizeSelected = true;
		}
		else if ((540 < pt.x) && (pt.x <  660) && (235 < pt.y) && (pt.y < 290)) {
			boardSelected = 8;
			boardSizeSelected = true;
		}
		else if ((110 < pt.x) && (pt.x <  280) && (360 < pt.y) && (pt.y < 410)) {
			boardSelected = 10;
			boardSizeSelected = true;
		}
		else if ((320 < pt.x) && (pt.x <  490) && (360 < pt.y) && (pt.y < 410)) {
			boardSelected = 12;
			boardSizeSelected = true;
		}
		else if ((515 < pt.x) && (pt.x <  685) && (360 < pt.y) && (pt.y < 410)) {
			boardSelected = 14;
			boardSizeSelected = true;
		}
		waitKey(50);
	}
	destroyWindow("Starting game...");
	return boardSelected;
}

// Determines the difficulty level
// 2 = Monte Carlo
// 3 = MiniMax
int chooseAIDifficulty(int whichAI) {
	// Width and height of window to start a game
	int openingWidth = 800;
	int openingHeight = 500;

	// Creates starting screen image, colored dark green
	Mat gameStarter = Mat(openingHeight, openingWidth, CV_8UC3, Scalar(50, 100, 0));

	// Text to be displayed
	string textLineOne = "Choose the AI difficulty";

	// Font, size and thickness settings
	int fontFace = FONT_HERSHEY_COMPLEX;
	double fontScale = 1.0;
	int thickness = 4;

	// Location of where to put the test (done across 2 lines)
	Point textOrgLineOne(openingWidth / 7, openingHeight / 8);
	//Point textOrgLineTwo(1.5* openingWidth / 9, 1.75 * openingHeight / 6);

	// Puts the text in the image, asking the question
	putText(gameStarter, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
	//putText(gameStarter, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

	// Displays the various board sizes available
	putText(gameStarter, "Easy", Point(250, 150), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);
	putText(gameStarter, "Medium", Point(250, 250), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);

	putText(gameStarter, "Hard", Point(250, 350), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);
	putText(gameStarter, "Very Hard (Slower)", Point(250, 450), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);

	// Initial co-ordinates that are out of range of the image
	Point2i pt(-1, -1);

	imshow("Starting game...", gameStarter);

	setMouseCallback("Starting game...", mouseHandler, (void*)&pt);

	// To loop until board is selected
	bool difficultySelected = false;

	// Variable for which board size is used
	int difficulty = 0;

	// Loops until a size is selected
	while (!difficultySelected) {
		// Selecting board sizes from 4x4, 6x6, 8x8, 10x10, 12x12 and 14x14
		if ((240 < pt.x) && (pt.x <  530) && (120 < pt.y) && (pt.y < 165)) {
			if (whichAI == 2) {
				difficulty = 100;
			}
			else if (whichAI == 3) {
				difficulty = 2;
			}
			difficultySelected = true;
		}
		else if ((240 < pt.x) && (pt.x <  575) && (215 < pt.y) && (pt.y < 275)) {
			if (whichAI == 2) {
				difficulty = 1000;
			}
			else if (whichAI == 3) {
				difficulty = 3;
			}
			difficultySelected = true;
		}
		else if ((240 < pt.x) && (pt.x <  575) && (320 < pt.y) && (pt.y < 365)) {
			if (whichAI == 2) {
				difficulty = 2000;
			}
			else if (whichAI == 3) {
				difficulty = 4;
			}
			difficultySelected = true;
		}
		else if ((240 < pt.x) && (pt.x <  635) && (415 < pt.y) && (pt.y < 465)) {
			if (whichAI == 2) {
				difficulty = 30000;
			}
			else if (whichAI == 3){
				difficulty = 5;
			}
			difficultySelected = true;
		}
		waitKey(50);
	}
	destroyWindow("Starting game...");
	return difficulty;
}

// Select which players are humans and which are AI
int choosePlayerTypes(String textLineOne, int color) {
	// Width and height of window to start a game
	int openingWidth = 800;
	int openingHeight = 500;

	// Creates starting screen image, colored dark green
	Mat gameStarter = Mat(openingHeight, openingWidth, CV_8UC3, Scalar(50, 100, 0));

	// Text to be displayed
	//string textLineOne = "What player types would you like?";

	// Font, size and thickness settings
	int fontFace = FONT_HERSHEY_COMPLEX;
	double fontScale = 1.0;
	int thickness = 4;

	// Location of where to put the test (done across 2 lines)
	Point textOrgLineOne(openingWidth / 7, openingHeight / 8);
	//Point textOrgLineTwo(1.5* openingWidth / 9, 1.75 * openingHeight / 6);

	// Puts the text in the image, asking the question
	putText(gameStarter, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(color, color, color), thickness, 8);
	//putText(gameStarter, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

	// Displays the various board sizes available
	putText(gameStarter, "Human", Point(250, 150), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(255-color, 255-color, 255-color), thickness, 8);
	putText(gameStarter, "Computer - Monte Carlo", Point(250, 250), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(255 - color, 255 - color, 255 - color), thickness, 8);
	putText(gameStarter, "Computer - MiniMax", Point(250, 350), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(255 - color, 255 - color, 255 - color), thickness, 8);
	//putText(gameStarter, "Computer vs Computer", Point(250, 450), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);

	// Initial co-ordinates that are out of range of the image
	Point2i pt(-1, -1);

	imshow("Starting game...", gameStarter);

	setMouseCallback("Starting game...", mouseHandler, (void*)&pt);

	// To loop until board is selected
	bool playerTypeSelected = false;

	// Variable for which board size is used
	int playerType = 0;

	// Loops until a size is selected
	while (!playerTypeSelected) {
		// Selecting board sizes from 4x4, 6x6, 8x8, 10x10, 12x12 and 14x14
		if ((240 < pt.x) && (pt.x <  530) && (120 < pt.y) && (pt.y < 165)) {
			playerType = 1;
			playerTypeSelected = true;
		}
		else if ((240 < pt.x) && (pt.x < 775) && (215 < pt.y) && (pt.y < 275)) {
			playerType = 2;
			playerTypeSelected = true;
		}
		else if ((240 < pt.x) && (pt.x <  575) && (320 < pt.y) && (pt.y < 365)) {
			playerType = 3;
			playerTypeSelected = true;
		}
		//else if ((240 < pt.x) && (pt.x <  635) && (415 < pt.y) && (pt.y < 465)) {
		//	playerType = 4;
		//	playerTypeSelected = true;
		//}
		waitKey(50);
	}
	destroyWindow("Starting game...");
	return playerType;
}

// Shows an image asking the user to select 1 of several board sizes and returns that value
int chooseGameBoardSizeHexagons() {
	// Width and height of window to start a game
	int openingWidth = 800;
	int openingHeight = 500;

	// Creates starting screen image, colored dark green
	Mat gameStarter = Mat(openingHeight, openingWidth, CV_8UC3, Scalar(50, 100, 0));

	// Text to be displayed
	string textLineOne = "What sized board would";
	string textLineTwo = "you like to play on?";

	// Font, size and thickness settings
	int fontFace = FONT_HERSHEY_COMPLEX;
	double fontScale = 1.5;
	int thickness = 4;

	// Location of where to put the test (done across 2 lines)
	Point textOrgLineOne(openingWidth / 9, openingHeight / 6);
	Point textOrgLineTwo(1.5* openingWidth / 9, 1.75 * openingHeight / 6);

	// Puts the text in the image, asking the question
	putText(gameStarter, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
	putText(gameStarter, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

	// Displays the various board sizes available
	putText(gameStarter, "4", Point(300, 275), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);
	putText(gameStarter, "5", Point(450, 275), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);

	putText(gameStarter, "6", Point(300, 400), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);
	putText(gameStarter, "7", Point(450, 400), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 0), thickness, 8);

	// Initial co-ordinates that are out of range of the image
	Point2i pt(-1, -1);

	imshow("Starting game...", gameStarter);

	setMouseCallback("Starting game...", mouseHandler, (void*)&pt);

	// To loop until board is selected
	bool boardSizeSelected = false;

	// Variable for which board size is used
	int boardSelected = 0;

	// Loops until a size is selected
	while (!boardSizeSelected) {
		// Selecting board sizes from 4x4, 6x6, 8x8, 10x10, 12x12 and 14x14
		if ((280 < pt.x) && (pt.x <  340) && (235 < pt.y) && (pt.y < 290)) {
			boardSelected = 4;
			boardSizeSelected = true;
		}
		else if ((430 < pt.x) && (pt.x <  490) && (235 < pt.y) && (pt.y < 290)) {
			boardSelected = 5;
			boardSizeSelected = true;
		}
		else if ((280 < pt.x) && (pt.x <  340) && (360 < pt.y) && (pt.y < 410)) {
			boardSelected = 6;
			boardSizeSelected = true;
		}
		else if ((430 < pt.x) && (pt.x <  490) && (360 < pt.y) && (pt.y < 410)) {
			boardSelected = 7;
			boardSizeSelected = true;
		}
		waitKey(50);
	}
	destroyWindow("Starting game...");
	return boardSelected;
}

// Plays out a game in hexagons
// Not implemented much as of yet
void playHexagonGame( int boardSize) {

	HexagonBoard *board = new HexagonBoard(boardSize);

	HexagonPlayer *p1 = new HexagonPlayer();
	HexagonPlayer *p2 = new HexagonPlayer();

	Mat hexagon = board->drawBoard();

	/* ---------------------------------------------------------------------------------------------------------------------------------*/
	// This section creates the unplayable squares
	while (true) {

		int openingWidth = 800;
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 20);

		string textLineOne = "Finish selecting unplayables";
		putText(hexagon, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		int* unplayabalePixelSelection = p1->getMove(hexagon, *board);

		if (unplayabalePixelSelection[0] > 125 && unplayabalePixelSelection[0] < 630 && unplayabalePixelSelection[1] > 5 && unplayabalePixelSelection[1] < 28) {
			hexagon = board->drawBoard();
			break;
		}

		int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
		board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
		hexagon = board->drawBoard();

	}

	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	int numberOfNonPlayableConsecutiveTurnsHexagons = 0;

	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started

		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);

		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			//cout << "Blacks turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!firstHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* pixelSelection = p1->getMove(hexagon, *board);

				// Converts the pixels into the board location
				int* moves = board->scalePixelSelection(pixelSelection);
				while (moves[0] == -1) {
					cout << "Please press in the center of the tile you wish to play " << endl;
					pixelSelection = p1->getMove(hexagon, *board);
					moves = board->scalePixelSelection(pixelSelection);

				}

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(moves[0], moves[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurnsHexagons = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurnsHexagons += 1;
			if (numberOfNonPlayableConsecutiveTurnsHexagons == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		hexagon = board->drawBoard();

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);

		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
			//cout << "Whites turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!secondHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* pixelSelection = p2->getMove(hexagon, *board);


				// Converts the pixels into the board location
				int* moves = board->scalePixelSelection(pixelSelection);
				while (moves[0] == -1) {
					cout << "Please press in the center of the tile you wish to play " << endl;
					pixelSelection = p2->getMove(hexagon, *board);
					moves = board->scalePixelSelection(pixelSelection);

				}

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(moves[0], moves[1], 1)) {
					secondHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurnsHexagons = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurnsHexagons += 1;
			if (numberOfNonPlayableConsecutiveTurnsHexagons == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		hexagon = board->drawBoard();
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	hexagon = board->drawBoard();
	imshow("Board with discs", hexagon);

	// Works out who won
	board->calculateWinner();
	cout << "Game finished " << endl;
	waitKey(0);
}

// NOTE TO SELF - USE INHERITANCE NEXT TIME

// Plays a normal game between 2 human players
int playGame(Player *p1, Player *p2, int boardSize) {
	// Gets the user to select a board size (4x4 .... 14x14)

	// Sets the block size based upon the boardSize and the size of this screen
	int blockSize = 672 / boardSize;

	// Outside edge of board set to 100
	int offset = 100;

	// Creates a board object
	Board *board = new Board(blockSize, boardSize, offset);

	// This variable represents the image of the board, but doesn't contain information about the board
	Mat pictureOfBoard = board->drawBoard();
	//pictureOfBoard  = board->drawAvailablePositions(-1, pictureOfBoard);

	/* ---------------------------------------------------------------------------------------------------------------------------------*/
	// This section creates the unplayable squares

	Player *p3 = new Player();

	bool placingVoids = true;

	int previousLocation = 0;
	double newLocation = 0;

	while (placingVoids) {

		int openingWidth = 800;
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
		Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

		string textLineOne = "Finish selecting unplayables";
		string textLineTwo = "Generate random seed";

		putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
		putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		for (int x = 0; x < 10; x++) {
			//cout << "new location = " << newLocation << endl;
			if (x == newLocation) {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
			}
			else {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
			}
		}

		int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);



		for (double x = 0; x < 10; x++) {
			if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
				//cout << "x = " << x << endl;
				newLocation = x;
			}
		}


		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 0 && unplayabalePixelSelection[1] < 41) {
			pictureOfBoard = board->drawBoard();
			placingVoids = false;
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
			srand(time(NULL));
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {

					// Sets the disc back to 0
					board->setDiscEmpty(i, j);
					//cout << "new location / 5 = " << double(newLocation / 5.0) << endl;
					// Gives the square a random chance of becoming unplayable
					if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
						board->setDiscUnplayable(i, j);
					}
				}
			}

			// Set the center co-ordinates back to the start
			board->setInitialSquares();
			pictureOfBoard = board->drawBoard();
		}

		if (placingVoids) {
			if (unplayabalePixelSelection[0] > offset) {
				int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
				board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
				pictureOfBoard = board->drawBoard();
			}
		}

	}
	delete p3;
	pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	// If this ever reaches 2, end the game
	int numberOfNonPlayableConsecutiveTurns = 0;

	// Loops while there are still empty discs on the board
	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started  (Player 1 is black, has a code value of -1)

		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);
		//board->availableMoveLocations(-1, playerOneAvailableMoves);
		//cout << "Black has " << playerOneAvailableMoves << " available moves" << endl;
		//MiniMaxNode *root = new MiniMaxNode(NULL, board->getBoardValues(), -1, boardSize, NULL, -1, 1500, 1200, 500, 10, 312, 6, 50, 2);
		//delete root;

		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			////cout << "Blacks turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!firstHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* pixelSelection = p1->getMove(pictureOfBoard, *board);

				// Converts the pixels into the board location
				int* moves = board->scalePixelSelection(pixelSelection);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(moves[0], moves[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(1, pictureOfBoard);

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started (Player 2 is white, has a code value of 1)

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);
		//board->availableMoveLocations(1, playerTwoAvailableMoves);
		//cout << "White has " << playerTwoAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
		//	//cout << "Whites turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!secondHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* pixelSelection = p2->getMove(pictureOfBoard, *board);

				// Converts the pixels into the board location
				int* moves = board->scalePixelSelection(pixelSelection);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(moves[0], moves[1], 1)) {
					secondHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	pictureOfBoard = board->drawBoard();
	imshow("Board with discs", pictureOfBoard);

	waitKey(50);

	// Works out who won
	int winner = board->printWinner();
	cout << "Game finished " << endl;

	waitKey(0);

	return winner;
}

// Plays against the computer, with the human player going first
int playGame(Player *p1, MonteCarlo *p2, int boardSize) {

	// Gets the user to select a board size (4x4 .... 14x14)
	///int boardSize = chooseGameBoardSizeSquares();

	// Sets the block size based upon the boardSize and the size of this screen
	int blockSize = 672 / boardSize;

	// Outside edge of board set to 100
	int offset = 100;

	// Creates a board object
	Board *board = new Board(blockSize, boardSize, offset);

	// This variable represents the image of the board, but doesn't contain information about the board
	Mat pictureOfBoard = board->drawBoard();

	// Makes 2 player objects, but they don't do anything differently currently
	///Player *p1 = new Player();
	///MonteCarlo *p2 = new MonteCarlo(4000);

	/* ---------------------------------------------------------------------------------------------------------------------------------*/
	// This section creates the unplayable squares

	Player *p3 = new Player();

	bool placingVoids = true;

	int previousLocation = 0;
	double newLocation = 0;

	while (placingVoids) {

		int openingWidth = 800;
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
		Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

		string textLineOne = "Finish selecting unplayables";
		string textLineTwo = "Generate random seed";



		putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
		putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		for (int x = 0; x < 10; x++) {
			//cout << "new location = " << newLocation << endl;
			if (x == newLocation) {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
			}
			else {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
			}
		}

		int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);



		for (double x = 0; x < 10; x++) {
			if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
				//cout << "x = " << x << endl;
				newLocation = x;
			}
		}


		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 0 && unplayabalePixelSelection[1] < 41) {
			pictureOfBoard = board->drawBoard();
			placingVoids = false;
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
			srand(time(NULL));
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {

					// Sets the disc back to 0
					board->setDiscEmpty(i, j);
					//cout << "new location / 5 = " << double(newLocation / 5.0) << endl;
					// Gives the square a random chance of becoming unplayable
					if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
						board->setDiscUnplayable(i, j);
					}
				}
			}

			// Set the center co-ordinates back to the start
			board->setInitialSquares();
			pictureOfBoard = board->drawBoard();
		}

		if (placingVoids) {
			if (unplayabalePixelSelection[0] > offset) {
				int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
				board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
				pictureOfBoard = board->drawBoard();
			}
		}

	}
	delete p3;
	pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	// If this ever reaches 2, end the game
	int numberOfNonPlayableConsecutiveTurns = 0;

	// Loops while there are still empty discs on the board
	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started  (Player 1 is black, has a code value of -1)

		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);
		board->availableMoveLocations(-1, playerOneAvailableMoves);
		//cout << "Black has " << playerOneAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			//cout << "Blacks turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!firstHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* pixelSelection = p1->getMove(pictureOfBoard, *board);

				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// Converts the pixels into the board location
				int* moves = board->scalePixelSelection(pixelSelection);
				//waitKey(1000);

				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(moves[0], moves[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started (Player 2 is white, has a code value of 1)

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);

		//cout << "White has " << playerTwoAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
			//cout << "Whites turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!secondHasPlayed) {
				waitKey(100);
				int* computerMove = p2->getMoveMonteCarlo(board, 1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], 1)) {
					secondHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	pictureOfBoard = board->drawBoard();
	imshow("Board with discs", pictureOfBoard);

	waitKey(50);

	// Works out who won
	int winner = board->printWinner();

	//cout << "Game finished with player " << winner << " as the winner" << endl;

	waitKey(0);

	return winner;

}

// Plays against the computer, with the computer player going first
int playGame(MonteCarlo *p1, Player *p2, int boardSize) {
	// Gets the user to select a board size (4x4 .... 14x14)
	///int boardSize = chooseGameBoardSizeSquares();

	// Sets the block size based upon the boardSize and the size of this screen
	int blockSize = 672 / boardSize;

	// Outside edge of board set to 100
	int offset = 100;

	// Creates a board object
	Board *board = new Board(blockSize, boardSize, offset);

	// This variable represents the image of the board, but doesn't contain information about the board
	Mat pictureOfBoard = board->drawBoard();

	// Makes 2 player objects, but they don't do anything differently currently
	///Player *p1 = new Player();
	///MonteCarlo *p2 = new MonteCarlo(4000);

	/* ---------------------------------------------------------------------------------------------------------------------------------*/
	// This section creates the unplayable squares

	Player *p3 = new Player();

	bool placingVoids = true;

	int previousLocation = 0;
	double newLocation = 0;

	while (placingVoids) {

		int openingWidth = 800;
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
		Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

		string textLineOne = "Finish selecting unplayables";
		string textLineTwo = "Generate random seed";



		putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
		putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		for (int x = 0; x < 10; x++) {
			//cout << "new location = " << newLocation << endl;
			if (x == newLocation) {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
			}
			else {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
			}
		}

		int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);



		for (double x = 0; x < 10; x++) {
			if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
				//cout << "x = " << x << endl;
				newLocation = x;
			}
		}


		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 0 && unplayabalePixelSelection[1] < 41) {
			pictureOfBoard = board->drawBoard();
			placingVoids = false;
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
			srand(time(NULL));
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {

					// Sets the disc back to 0
					board->setDiscEmpty(i, j);
					//cout << "new location / 5 = " << double(newLocation / 5.0) << endl;
					// Gives the square a random chance of becoming unplayable
					if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
						board->setDiscUnplayable(i, j);
					}
				}
			}

			// Set the center co-ordinates back to the start
			board->setInitialSquares();
			pictureOfBoard = board->drawBoard();
		}

		if (placingVoids) {
			if (unplayabalePixelSelection[0] > offset) {
				int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
				board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
				pictureOfBoard = board->drawBoard();
			}
		}

	}
	delete p3;
	pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	// If this ever reaches 2, end the game
	int numberOfNonPlayableConsecutiveTurns = 0;

	// Loops while there are still empty discs on the board
	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started  (Player 1 is black, has a code value of -1)

		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);

		// Checks to see if the number of available moves is > 1
		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			cout << "Blacks turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!firstHasPlayed) {
				waitKey(100);
				int* computerMove = p1->getMoveMonteCarlo(board, -1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started (Player 2 is white, has a code value of 1)

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);

		//cout << "White has " << playerTwoAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
			cout << "Whites turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!secondHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* pixelSelection = p2->getMove(pictureOfBoard, *board);

				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// Converts the pixels into the board location
				int* moves = board->scalePixelSelection(pixelSelection);
				//waitKey(1000);

				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(moves[0], moves[1], 1)) {
					secondHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	pictureOfBoard = board->drawBoard();
	imshow("Board with discs", pictureOfBoard);

	waitKey(50);

	// Works out who won
	int winner = board->printWinner();

	//cout << "Game finished with player " << winner << " as the winner" << endl;

	waitKey(0);

	return winner;
}

// Plays the computer against itself
int playGame(MonteCarlo *p1, MonteCarlo *p2, int boardSize) {
	// Gets the user to select a board size (4x4 .... 14x14)

	// Sets the block size based upon the boardSize and the size of this screen
	int blockSize = 672 / boardSize;

	// Outside edge of board set to 100
	int offset = 100;

	// Creates a board object
	Board *board = new Board(blockSize, boardSize, offset);

	// This variable represents the image of the board, but doesn't contain information about the board
	Mat pictureOfBoard = board->drawBoard();

	// Makes 2 player objects, but they don't do anything differently currently

	/* ---------------------------------------------------------------------------------------------------------------------------------*/
	// This section creates the unplayable squares

	Player *p3 = new Player();

	bool placingVoids = true;

	int previousLocation = 0;
	double newLocation = 0;

	while (placingVoids) {

		int openingWidth = 800;	
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
		Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

		string textLineOne = "Finish selecting unplayables";
		string textLineTwo = "Generate random seed";

		putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
		putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		for (int x = 0; x < 10; x++) {
			if (x == newLocation) {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
			}
			else {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
			}
		}

		int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);

		for (double x = 0; x < 10; x++) {
			if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
				newLocation = x;
			}
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 0 && unplayabalePixelSelection[1] < 41) {
			pictureOfBoard = board->drawBoard();
			placingVoids = false;
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
			srand(time(NULL));
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {

					// Sets the disc back to 0
					board->setDiscEmpty(i, j);
					// Gives the square a random chance of becoming unplayable
					if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
						board->setDiscUnplayable(i, j);
					}
				}
			}

			// Set the center co-ordinates back to the start
			board->setInitialSquares();
			pictureOfBoard = board->drawBoard();
		}

		if (placingVoids) {
			if (unplayabalePixelSelection[0] > offset) {
				int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
				board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
				pictureOfBoard = board->drawBoard();
			}
		}

	}
	delete p3;
	pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	// If this ever reaches 2, end the game
	int numberOfNonPlayableConsecutiveTurns = 0;

	// Loops while there are still empty discs on the board
	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started  (Player 1 is black, has a code value of -1)

		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);
		board->availableMoveLocations(-1, playerOneAvailableMoves);
		/////cout << "Black has " << playerOneAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			/////cout << "Blacks turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!firstHasPlayed) {
				waitKey(5);
				// Gets the pixels selected on the board by that player
				int* computerMove = p1->getMoveMonteCarlo(board, -1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			///cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
			///	cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started (Player 2 is white, has a code value of 1)

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);

		/////cout << "White has " << playerTwoAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
		///	//cout << "Whites turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!secondHasPlayed) {
				waitKey(5);
				int* computerMove = p2->getMoveMonteCarlo(board, 1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], 1)) {
					secondHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
		///	cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
			///	cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	pictureOfBoard = board->drawBoard();
	imshow("Board with discs", pictureOfBoard);

	// Works out who won
	int winner = board->printWinner();

	cout << "Game finished " << endl;

	waitKey(0);

	//waitKey(500);
	return winner;
}

// Plays against the computer, with the human player going first
int playGame(Player *p1, MiniMax *p2, int boardSize) {

	p2->setPlayerNumber(1);

	// Gets the user to select a board size (4x4 .... 14x14)
	///int boardSize = chooseGameBoardSizeSquares();

	// Sets the block size based upon the boardSize and the size of this screen
	int blockSize = 672 / boardSize;

	// Outside edge of board set to 100
	int offset = 100;

	// Creates a board object
	Board *board = new Board(blockSize, boardSize, offset);

	// This variable represents the image of the board, but doesn't contain information about the board
	Mat pictureOfBoard = board->drawBoard();

	// Makes 2 player objects, but they don't do anything differently currently
	///Player *p1 = new Player();
	///MonteCarlo *p2 = new MonteCarlo(4000);

	/* ---------------------------------------------------------------------------------------------------------------------------------*/
	// This section creates the unplayable squares

	Player *p3 = new Player();

	bool placingVoids = true;

	int previousLocation = 0;
	double newLocation = 0;

	while (placingVoids) {

		int openingWidth = 800;
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
		Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

		string textLineOne = "Finish selecting unplayables";
		string textLineTwo = "Generate random seed";

		putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
		putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		for (int x = 0; x < 10; x++) {
			//cout << "new location = " << newLocation << endl;
			if (x == newLocation) {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
			}
			else {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
			}
		}

		int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);



		for (double x = 0; x < 10; x++) {
			if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
				//cout << "x = " << x << endl;
				newLocation = x;
			}
		}


		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 0 && unplayabalePixelSelection[1] < 41) {
			pictureOfBoard = board->drawBoard();
			placingVoids = false;
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
			srand(time(NULL));
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {

					// Sets the disc back to 0
					board->setDiscEmpty(i, j);
					//cout << "new location / 5 = " << double(newLocation / 5.0) << endl;
					// Gives the square a random chance of becoming unplayable
					if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
						board->setDiscUnplayable(i, j);
					}
				}
			}

			// Set the center co-ordinates back to the start
			board->setInitialSquares();
			pictureOfBoard = board->drawBoard();
		}

		if (placingVoids) {
			if (unplayabalePixelSelection[0] > offset) {
				int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
				board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
				pictureOfBoard = board->drawBoard();
			}
		}

	}
	delete p3;
	pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	// If this ever reaches 2, end the game
	int numberOfNonPlayableConsecutiveTurns = 0;

	// Loops while there are still empty discs on the board
	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started  (Player 1 is black, has a code value of -1)

		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);
		MiniMaxNode *root = new MiniMaxNode(NULL, board->getBoardValues(), -1, boardSize, NULL, -1,0,0,0,0,0,0,0,0);
		root->evaluatePosition();
		delete root;
		board->availableMoveLocations(-1, playerOneAvailableMoves);
		//cout << "Black has " << playerOneAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			//cout << "Blacks turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!firstHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* pixelSelection = p1->getMove(pictureOfBoard, *board);

				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// Converts the pixels into the board location
				int* moves = board->scalePixelSelection(pixelSelection);
				//waitKey(1000);

				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(moves[0], moves[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started (Player 2 is white, has a code value of 1)

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);
		MiniMaxNode *roott = new MiniMaxNode(NULL, board->getBoardValues(), 1, boardSize, NULL, 1,0,0,0,0,0,0,0,0);
		//root->findStables();
		delete roott;
		//cout << "White has " << playerTwoAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
			//cout << "Whites turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!secondHasPlayed) {
				waitKey(100);
				int* computerMove = p2->getMoveMiniMax(board, 1);
				//cout << "Computer move = " << computerMove[0] << ", " << computerMove[1] << endl;

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], 1)) {
					secondHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	pictureOfBoard = board->drawBoard();
	imshow("Board with discs", pictureOfBoard);

	waitKey(50);

	// Works out who won
	int winner = board->printWinner();

	//cout << "Game finished with player " << winner << " as the winner" << endl;

	waitKey(0);
	return 0;
}

// Plays against the computer, with the human player going second
int playGame(MiniMax *p1, Player *p2, int boardSize) {

	p1->setPlayerNumber(-1);

	// Gets the user to select a board size (4x4 .... 14x14)
	///int boardSize = chooseGameBoardSizeSquares();

	// Sets the block size based upon the boardSize and the size of this screen
	int blockSize = 672 / boardSize;

	// Outside edge of board set to 100
	int offset = 100;

	// Creates a board object
	Board *board = new Board(blockSize, boardSize, offset);

	// This variable represents the image of the board, but doesn't contain information about the board
	Mat pictureOfBoard = board->drawBoard();

	// Makes 2 player objects, but they don't do anything differently currently
	///Player *p1 = new Player();
	///MonteCarlo *p2 = new MonteCarlo(4000);

	/* ---------------------------------------------------------------------------------------------------------------------------------*/
	// This section creates the unplayable squares

	Player *p3 = new Player();

	bool placingVoids = true;

	int previousLocation = 0;
	double newLocation = 0;

	while (placingVoids) {

		int openingWidth = 800;
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
		Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

		string textLineOne = "Finish selecting unplayables";
		string textLineTwo = "Generate random seed";



		putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
		putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		for (int x = 0; x < 10; x++) {
			//cout << "new location = " << newLocation << endl;
			if (x == newLocation) {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
			}
			else {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
			}
		}

		int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);



		for (double x = 0; x < 10; x++) {
			if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
				//cout << "x = " << x << endl;
				newLocation = x;
			}
		}


		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 0 && unplayabalePixelSelection[1] < 41) {
			pictureOfBoard = board->drawBoard();
			placingVoids = false;
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
			srand(time(NULL));
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {

					// Sets the disc back to 0
					board->setDiscEmpty(i, j);
					//cout << "new location / 5 = " << double(newLocation / 5.0) << endl;
					// Gives the square a random chance of becoming unplayable
					if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
						board->setDiscUnplayable(i, j);
					}
				}
			}

			// Set the center co-ordinates back to the start
			board->setInitialSquares();
			pictureOfBoard = board->drawBoard();
		}

		if (placingVoids) {
			if (unplayabalePixelSelection[0] > offset) {
				int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
				board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
				pictureOfBoard = board->drawBoard();
			}
		}

	}
	delete p3;
	pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	// If this ever reaches 2, end the game
	int numberOfNonPlayableConsecutiveTurns = 0;

	// Loops while there are still empty discs on the board
	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started  (Player 1 is black, has a code value of -1)

		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);
		MiniMaxNode *root = new MiniMaxNode(NULL, board->getBoardValues(), -1, boardSize, NULL, -1, 0, 0, 0, 0, 0, 0, 0, 0);
		root->evaluatePosition();
		delete root;
		board->availableMoveLocations(-1, playerOneAvailableMoves);
		//cout << "Black has " << playerOneAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			//cout << "Blacks turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!firstHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* computerMove = p1->getMoveMiniMax(board, -1);
				//cout << "Computer move = " << computerMove[0] << ", " << computerMove[1] << endl;
				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		waitKey(50);

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started (Player 2 is white, has a code value of 1)

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);
		MiniMaxNode *roott = new MiniMaxNode(NULL, board->getBoardValues(), 1, boardSize, NULL, 1, 0, 0, 0, 0, 0, 0, 0, 0);
		//root->findStables();
		delete roott;
		//cout << "White has " << playerTwoAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
			//cout << "Whites turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!secondHasPlayed) {
				waitKey(100);
				int* pixelSelection = p2->getMove(pictureOfBoard, *board);

				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// Converts the pixels into the board location
				int* moves = board->scalePixelSelection(pixelSelection);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(moves[0], moves[1], 1)) {
					secondHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		waitKey(500);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	pictureOfBoard = board->drawBoard();
	imshow("Board with discs", pictureOfBoard);

	waitKey(50);

	// Works out who won
	int winner = board->printWinner();

	//cout << "Game finished with player " << winner << " as the winner" << endl;

	waitKey(0);
	return 0;
}

// Plays against the computer, with the human player going first
int playGame(MiniMax *p1, MiniMax *p2, int boardSize) {

	p1->setPlayerNumber(-1);
	p2->setPlayerNumber(1);
	
	// Gets the user to select a board size (4x4 .... 14x14)
	///int boardSize = chooseGameBoardSizeSquares();

	// Sets the block size based upon the boardSize and the size of this screen
	int blockSize = 672 / boardSize;

	// Outside edge of board set to 100
	int offset = 100;

	// Creates a board object
	Board *board = new Board(blockSize, boardSize, offset);

	// This variable represents the image of the board, but doesn't contain information about the board
	Mat pictureOfBoard = board->drawBoard();

	Player *p3 = new Player();

	bool placingVoids = true;

	int previousLocation = 0;
	double newLocation = 0;

	while (placingVoids) {

		int openingWidth = 800;
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
		Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

		string textLineOne = "Finish selecting unplayables";
		string textLineTwo = "Generate random seed";

		putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
		putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		for (int x = 0; x < 10; x++) {
			//cout << "new location = " << newLocation << endl;
			if (x == newLocation) {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
			}
			else {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
			}
		}

		int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);



		for (double x = 0; x < 10; x++) {
			if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
				//cout << "x = " << x << endl;
				newLocation = x;
			}
		}


		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 0 && unplayabalePixelSelection[1] < 41) {
			pictureOfBoard = board->drawBoard();
			placingVoids = false;
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
			srand(time(NULL));
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {

					// Sets the disc back to 0
					board->setDiscEmpty(i, j);
					//cout << "new location / 5 = " << double(newLocation / 5.0) << endl;
					// Gives the square a random chance of becoming unplayable
					if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
						board->setDiscUnplayable(i, j);
					}
				}
			}

			// Set the center co-ordinates back to the start
			board->setInitialSquares();
			pictureOfBoard = board->drawBoard();
		}

		if (placingVoids) {
			if (unplayabalePixelSelection[0] > offset) {
				int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
				board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
				pictureOfBoard = board->drawBoard();
			}
		}

	}
	delete p3;
	pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	// If this ever reaches 2, end the game
	int numberOfNonPlayableConsecutiveTurns = 0;

	// Loops while there are still empty discs on the board
	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started  (Player 1 is black, has a code value of -1)

		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);
		board->availableMoveLocations(-1, playerOneAvailableMoves);
		//cout << "Black has " << playerOneAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			//cout << "Blacks turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!firstHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* computerMoveOne = p1->getMoveMiniMax(board, -1);
				waitKey(10);
				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMoveOne[0], computerMoveOne[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			//cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				//cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		waitKey(50);

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started (Player 2 is white, has a code value of 1)

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);
		//cout << "White has " << playerTwoAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
			//cout << "Whites turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!secondHasPlayed) {
				waitKey(10);
				int* computerMove = p2->getMoveMiniMax(board, 1);
				//cout << "Computer move = " << computerMove[0] << ", " << computerMove[1] << endl;

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], 1)) {
					secondHasPlayed = true;
						
					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			//cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				//cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		waitKey(50);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	pictureOfBoard = board->drawBoard();
	imshow("Board with discs", pictureOfBoard);

	waitKey(50);

	// Works out who won
	int winner = board->printWinner();

	//cout << "Game finished " << endl;

	waitKey(0);

	return winner;

}

// Plays against the computer, with the human player going first
int playGame(MonteCarlo *p1, MiniMax *p2, int boardSize) {

	p2->setPlayerNumber(1);

	// Gets the user to select a board size (4x4 .... 14x14)
	///int boardSize = chooseGameBoardSizeSquares();

	// Sets the block size based upon the boardSize and the size of this screen
	int blockSize = 672 / boardSize;

	// Outside edge of board set to 100
	int offset = 100;

	// Creates a board object
	Board *board = new Board(blockSize, boardSize, offset);

	// This variable represents the image of the board, but doesn't contain information about the board
	Mat pictureOfBoard = board->drawBoard();

	// Makes 2 player objects, but they don't do anything differently currently
	///Player *p1 = new Player();
	///MonteCarlo *p2 = new MonteCarlo(4000);

	/* ---------------------------------------------------------------------------------------------------------------------------------*/
	// This section creates the unplayable squares
	Player *p3 = new Player();

	bool placingVoids = true;

	int previousLocation = 0;
	double newLocation = 0;

	while (placingVoids) {

		int openingWidth = 800;
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
		Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

		string textLineOne = "Finish selecting unplayables";
		string textLineTwo = "Generate random seed";

		

		putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
		putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		for (int x = 0; x < 10; x++) {
			//cout << "new location = " << newLocation << endl;
			if (x == newLocation) {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
			}
			else {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
			}
		}

		int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);



		for (double x = 0; x < 10; x++) {
			if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
				//cout << "x = " << x << endl;
				newLocation = x;
			}
		}


		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 0 && unplayabalePixelSelection[1] < 41) {
			pictureOfBoard = board->drawBoard();
			placingVoids = false;
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
			srand(time(NULL));
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {

					// Sets the disc back to 0
					board->setDiscEmpty(i, j);
					//cout << "new location / 5 = " << double(newLocation / 5.0) << endl;
					// Gives the square a random chance of becoming unplayable
					if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
						board->setDiscUnplayable(i, j);
					}
				}
			}

			// Set the center co-ordinates back to the start
			board->setInitialSquares();
			pictureOfBoard = board->drawBoard();
		}

		if (placingVoids) {
			if (unplayabalePixelSelection[0] > offset) {
				int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
				board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
				pictureOfBoard = board->drawBoard();
			}
		}

	}
	delete p3;
	pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	// If this ever reaches 2, end the game
	int numberOfNonPlayableConsecutiveTurns = 0;

	// Loops while there are still empty discs on the board
	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started  (Player 1 is black, has a code value of -1)

		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);
		//MiniMaxNode *root = new MiniMaxNode(NULL, board->getBoardValues(), -1, boardSize, NULL, -1);
		//root->evaluatePosition();
		//delete root;
		board->availableMoveLocations(-1, playerOneAvailableMoves);
		//cout << "Black has " << playerOneAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			//cout << "Blacks turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!firstHasPlayed) {
				// Gets the pixels selected on the board by that player
				int* computerMove = p1->getMoveMonteCarlo(board, -1);

				waitKey(100);

				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started (Player 2 is white, has a code value of 1)

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);
		//cout << "White has " << playerTwoAvailableMoves << " available moves" << endl;

		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
			//cout << "Whites turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!secondHasPlayed) {
				waitKey(100);
				int* computerMove = p2->getMoveMiniMax(board, 1);
				//cout << "Computer move = " << computerMove[0] << ", " << computerMove[1] << endl;

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], 1)) {
					secondHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		waitKey(5);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	pictureOfBoard = board->drawBoard();
	imshow("Board with discs", pictureOfBoard);

	waitKey(50);

	// Works out who won
	int winner = board->printWinner();

	waitKey(0);
	return winner;

}

// Plays against the computer, with the human player going first
int playGame(MiniMax *p1, MonteCarlo *p2, int boardSize) {

	clock_t begin = clock();
	double time_minimax = 0.0;
	double time_monte = 0.0;
	double time_taken_minimax = 0.0;
	double time_taken_monte = 0.0;

	p1->setPlayerNumber(-1);

	// Gets the user to select a board size (4x4 .... 14x14)
	///int boardSize = chooseGameBoardSizeSquares();

	// Sets the block size based upon the boardSize and the size of this screen
	int blockSize = 672 / boardSize;

	// Outside edge of board set to 100
	int offset = 100;

	double miniMaxMoves = 0;
	double monteCarloMoves = 0;
	double totalNumberOfMoves = 0;

	// Creates a board object
	Board *board = new Board(blockSize, boardSize, offset);

	// This variable represents the image of the board, but doesn't contain information about the board
	Mat pictureOfBoard = board->drawBoard();

	// Makes 2 player objects, but they don't do anything differently currently
	///Player *p1 = new Player();
	///MonteCarlo *p2 = new MonteCarlo(4000);

	/* ---------------------------------------------------------------------------------------------------------------------------------*/
	// This section creates the unplayable squares
	Player *p3 = new Player();

	bool placingVoids = true;

	int previousLocation = 0;
	double newLocation = 0;

	while (placingVoids) {

		int openingWidth = 800;
		int openingHeight = 500;

		// Font, size and thickness settings
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 1.0;
		int thickness = 2;

		// Location of where to put the test (done across 2 lines)
		Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
		Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

		string textLineOne = "Finish selecting unplayables";
		string textLineTwo = "Generate random seed";



		putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
		putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

		for (int x = 0; x < 10; x++) {
			//cout << "new location = " << newLocation << endl;
			if (x == newLocation) {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
			}
			else {
				rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
			}
		}

		int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);



		for (double x = 0; x < 10; x++) {
			if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
				//cout << "x = " << x << endl;
				newLocation = x;
			}
		}


		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 0 && unplayabalePixelSelection[1] < 41) {
			pictureOfBoard = board->drawBoard();
			placingVoids = false;
		}

		if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
			srand(time(NULL));
			for (int i = 0; i < boardSize; i++) {
				for (int j = 0; j < boardSize; j++) {

					// Sets the disc back to 0
					board->setDiscEmpty(i, j);
					//cout << "% chance = " << (newLocation + 1) * 2 << endl;
					// Gives the square a random chance of becoming unplayable
					if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
						board->setDiscUnplayable(i, j);
					}
				}
			}

			// Set the center co-ordinates back to the start
			board->setInitialSquares();
			pictureOfBoard = board->drawBoard();
		}

		if (placingVoids) {
			if (unplayabalePixelSelection[0] > offset) {
				int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
				board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
				pictureOfBoard = board->drawBoard();
			}
		}

	}
	delete p3;
	pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
	/* ---------------------------------------------------------------------------------------------------------------------------------*/

	// If this ever reaches 2, end the game
	int numberOfNonPlayableConsecutiveTurns = 0;

	// Loops while there are still empty discs on the board
	while (board->hasEmpty()) {

		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 1 turn started  (Player 1 is black, has a code value of -1)
		totalNumberOfMoves += 1.0;
		// Check if player 1 has had their go
		bool firstHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerOneAvailableMoves = board->numberOfAvailableMoves(-1);
		//MiniMaxNode *root = new MiniMaxNode(NULL, board->getBoardValues(), -1, boardSize, NULL, -1);
		//root->evaluatePosition();
		//delete root;
		board->availableMoveLocations(-1, playerOneAvailableMoves);
		//cout << "Black has " << playerOneAvailableMoves << " available moves" << endl;
		miniMaxMoves += playerOneAvailableMoves;
		// Checks to see if the number of available moves is > 1
		if (playerOneAvailableMoves) {
			//cout << "Blacks turn " << endl << endl;
			// Loops until player 1 has had their go
			while (!firstHasPlayed) {
				// Gets the pixels selected on the board by that player
				clock_t begin_minimax = clock();
				int* computerMove = p1->getMoveMiniMax(board, -1);
				clock_t end_minimax = clock();
				time_taken_minimax = double(end_minimax - begin_minimax) / CLOCKS_PER_SEC;
				time_minimax += time_taken_minimax;

				waitKey(100);

				//int* moves = p2->getMoveMonteCarlo(board, -1);

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], -1)) {
					firstHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			//cout << "No available move for black - turn ended " << endl;
			// If no available move, pass turn to player 2 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				//cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);

		// Player 1 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
		// Player 2 turn started (Player 2 is white, has a code value of 1)

		// Check if player 2 has had their go
		bool secondHasPlayed = false;

		// Checks the number of available moves for player 1
		int playerTwoAvailableMoves = board->numberOfAvailableMoves(1);
		//cout << "White has " << playerTwoAvailableMoves << " available moves" << endl;
		monteCarloMoves += playerTwoAvailableMoves;
		// Checks to see if the number of available moves is > 1
		if (playerTwoAvailableMoves) {
			//cout << "Whites turn" << endl << endl;
			// Loops until player 2 has had their go
			while (!secondHasPlayed) {
				waitKey(100);
				clock_t begin_monte = clock();
				int* computerMove = p2->getMoveMonteCarlo(board, 1);
				clock_t end_monte = clock();
				time_taken_monte = double(end_monte - begin_monte) / CLOCKS_PER_SEC;
				time_monte += time_taken_monte;
				//cout << "Computer move = " << computerMove[0] << ", " << computerMove[1] << endl;

				// If the move flips at least 1 disc, make the move and pass the turn to player 2
				if (board->placeDisc(computerMove[0], computerMove[1], 1)) {
					secondHasPlayed = true;

					// Resets the number of consecutive turns without possible moves to 0
					numberOfNonPlayableConsecutiveTurns = 0;
				}
				else {
					cout << "Invalid move, please pick another! " << endl;
				}
			}
		}
		else {
			//cout << "No available move for white - turn ended " << endl;
			// If no available move, pass turn to player 1 and increase number of consecutive turns without a possible move by 1
			numberOfNonPlayableConsecutiveTurns += 1;
			if (numberOfNonPlayableConsecutiveTurns == 2) {
				//cout << "NO AVAILABLE MOVES FOR EITHER PLAYER, ENDING GAME" << endl;
				break;
			}
		}
		// Draws the board after each move
		pictureOfBoard = board->drawBoard();
		pictureOfBoard = board->drawAvailablePositions(-1, pictureOfBoard);
		imshow("Board with discs", pictureOfBoard);
		waitKey(5);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
		pictureOfBoard = board->drawBoard();
		imshow("Board with discs", pictureOfBoard);

		waitKey(50);

	// Works out who won
	int winner = board->printWinner();
	
	double averageMonte = double(monteCarloMoves) / double(totalNumberOfMoves);
	double averageMinimax = double(miniMaxMoves) / double(totalNumberOfMoves);
	cout << "monte moves = " << averageMonte << endl;
	cout << "minimax moves = " << averageMinimax << endl << endl;
	cout << "Total number of moves = " << totalNumberOfMoves << endl;

	cout << "Total time for minimax = " << time_minimax << endl;
	cout << "Total time for monte = " << time_monte << endl;
	//cout << "Game finished with player " << winner << " as the winner" << endl;
	waitKey(0);
	return winner;

}

void compete() {

	int populationSize = 25;
	int* numberOfWins = new int[populationSize];
	MiniMax** populationMembers = new MiniMax*[populationSize];
	int boardSize = 8;
	int depth = 4;

	populationMembers[0] = new MiniMax(boardSize, depth, 6024, 7137, 550, 4713, 9480, 4245, 8588, 6616);				//	0
	populationMembers[1] = new MiniMax(boardSize, depth, 15930, 11971, 4881, 5076, 5253, 10926, 8653, 12196);				//	1
	populationMembers[2] = new MiniMax(boardSize, depth, 7712, 15950, 6603, 10583, 1420, 11681, 13703, 4537);				//	2
	populationMembers[3] = new MiniMax(boardSize, depth, 14067, 14147, 9403, 12401, 8128, 3120, 8776, 6326);			//	3
	populationMembers[4] = new MiniMax(boardSize, depth, 8689, 2042, 3967, 8783, 4379, 2309, 14686, 17498);				//	4
	populationMembers[5] = new MiniMax(boardSize, depth, 11726, 4572, 4857, 15301, 11076, 13648, 10830, 4068);				//	5

	populationMembers[6] = new MiniMax(boardSize, depth, 6482, 8546, 8366, 8965, 10985, 10214, 6551, 573);				//	6
	populationMembers[7] = new MiniMax(boardSize, depth, 6123, 13063, 2896, 2793, 12748, 646, 7614, 6481);				//	7
	populationMembers[8] = new MiniMax(boardSize, depth, 1784, 16071, 3742, 5049, 12472, 5507, 15779, 12497);				//	8
	populationMembers[9] = new MiniMax(boardSize, depth, 12611, 8581, 8862, 7084, 1602, 6813, 4069, 8636);				//	9
	populationMembers[10] = new MiniMax(boardSize, depth, 8045, 8829, 5569, 15552, 16024, 6521, 5020, 17770);				//	10

	populationMembers[11] = new MiniMax(boardSize, depth, 4036, 1883, 10912, 7653, 11696, 4510, 2050, 1952);				//	11
	populationMembers[12] = new MiniMax(boardSize, depth, 11338, 9314, 8480, 11456, 16948, 4785, 19877, 11193);			//	12
	populationMembers[13] = new MiniMax(boardSize, depth, 13539, 8665, 7962, 7000, 3037, 5985, 13909, 2124);			//	13
	populationMembers[14] = new MiniMax(boardSize, depth, 11420, 1149, 7185, 16678, 12169, 6028, 12489, 9325);				//	14
	populationMembers[15] = new MiniMax(boardSize, depth, 19936, 5157, 13300, 16057, 2956, 6359, 13525, 11093);				//	15

	populationMembers[16] = new MiniMax(boardSize, depth, 4355, 13969, 5800, 8848, 1099, 1428, 10669, 15331);			//	16
	populationMembers[17] = new MiniMax(boardSize, depth, 9272, 19878, 10804, 6826, 1911, 6003, 16297, 11193);				//	17
	populationMembers[18] = new MiniMax(boardSize, depth, 1804, 19593, 3401, 4917, 17301, 3999, 13750, 16099);				//	18
	populationMembers[19] = new MiniMax(boardSize, depth, 5463, 16698, 7257, 9104, 7258, 13474, 2454, 5501);				//	19
	populationMembers[20] = new MiniMax(boardSize, depth, 4544, 7114, 10499, 11842, 3535, 1498, 6732, 5501);				//	20


	populationMembers[21] = new MiniMax(boardSize, depth, 8270, 1351, 18635, 8289, 1773, 563, 8206, 4829);				//	25
	populationMembers[22] = new MiniMax(boardSize, depth, 15119, 13016, 7989, 12412, 8633, 6615, 9119, 4111);			//  30
	populationMembers[23] = new MiniMax(boardSize, depth, 11377, 9973, 8566, 10274, 11769, 1452, 10068, 12220);			//	35
	populationMembers[24] = new MiniMax(boardSize, depth, 2656, 3802, 7196, 16654, 2364, 792, 14113, 11207);

	for (int x = 0; x < populationSize; x++) {
		numberOfWins[x] = 0;
	}

	// Reference only
	int numberOfDraws = 0;
	int gameNumber = 0;

	for (int x = 0; x < populationSize; x++) {
		for (int y = x; y < populationSize; y++) {
			if (x != y) {
				gameNumber += 1;
				double percentage = 100 * double(gameNumber) / double((populationSize / 2 * (populationSize - 1)));
				printf("%.1f %%\r", percentage);
				fflush(stdout);
				//cout << percentage << "% \r" << endl;


				//cout << "Child " << x << " playing against child " << y << endl;
				int winnerGameOne = playGame(populationMembers[x], populationMembers[y], boardSize);
				if (winnerGameOne == 1) {
					//cout << "Child " << y << " won the first game" << endl;
					numberOfWins[y] += 1;
				}
				else if (winnerGameOne == -1) {
					//cout << "Child " << x << " won the first game" << endl;
					numberOfWins[x] += 1;
				}
				else {
					numberOfDraws += 1;
				}

				int winnerGameTwo = playGame(populationMembers[y], populationMembers[x], boardSize);
				if (winnerGameTwo == 1) {
					//cout << "Child " << x << " won the second game " << endl;
					numberOfWins[x] += 1;
				}
				else if (winnerGameTwo == -1) {
					//cout << "Child " << y << " won the second game " << endl;
					numberOfWins[y] += 1;
				}
				else {
					numberOfDraws += 1;
				}
				//cout << endl;
			}
		}
	}
	for (int x = 0; x < populationSize; x++) {
		cout << "Child " << x << " has " << numberOfWins[x] << " wins " << endl;
	}

}

void monteTest() {

	// Size of board to test on
	int boardSize = 8;

	// The number of AI to use
	int numberOfAI = 4;

	// Number of simulations each AI does
	int* numberOfSims = new int[numberOfAI];

	// Number of wins each AI has
	int* numberOfWins = new int[numberOfAI];

	for (int x = 0; x < numberOfAI; x++) {
		numberOfWins[x] = 0;
	}

	int number_of_rounds = 1;
	cout << "I am using " << number_of_rounds << endl;
	// The population of AI
	MonteCarlo** monteAIs = new MonteCarlo*[numberOfAI];

	numberOfSims[0] = 10;
	numberOfSims[1] = 50;
	numberOfSims[2] = 100;
	numberOfSims[3] = 250;
	//numberOfSims[4] = 500;
	//numberOfSims[5] = 1000;
	//numberOfSims[6] = 1500;
	//numberOfSims[7] = 2000;
	//numberOfSims[8] = 2500;
	//numberOfSims[9] = 3000;
	//numberOfSims[10] = 3500;
	//numberOfSims[11] = 4000;

	for (int x = 0; x < numberOfAI; x++) {
		//numberOfSims[x] = (x+1)*500;
		//cout << "Number of sims of child " << x << " = " << numberOfSims[x] << endl;
		monteAIs[x] = new MonteCarlo(numberOfSims[x]);
	}

	cout << "Using " << numberOfAI << " AI with " << number_of_rounds << " rounds" << endl;

	// 10, 50, 100, 200, 500, 750, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000

	for (int z = 0; z < number_of_rounds; z++) {
		for (int x = 0; x < numberOfAI; x++) {
			for (int y = x; y < numberOfAI; y++) {
				if (x != y) {
					cout << "Child " << x << " (" << numberOfSims[x] << " sims) playing against child " << y << " (" << numberOfSims[y] << " sims)" << endl;
					int winnerGameOne = playGame(monteAIs[x], monteAIs[y], boardSize);
					if (winnerGameOne == 1) {
						numberOfWins[y] += 1;
					}
					else if (winnerGameOne == -1) {
						numberOfWins[x] += 1;
					}
					//cout << "child " << y << " playing against child " << x << endl;
					int winnerGameTwo = playGame(monteAIs[y], monteAIs[x], boardSize);
					if (winnerGameTwo == 1) {
						numberOfWins[x] += 1;
					}
					else if (winnerGameTwo == -1) {
						numberOfWins[y] += 1;
					}
				}
			}
		}
	}
	cout << " -----------------------------" << endl;
	for (int x = 0; x < numberOfAI; x++) {
		cout << "AI number " << x << " with " << numberOfSims[x] << " simulations has " << numberOfWins[x] << " wins" << endl;
	}


}

int main(int argc, char *argv[])
{
	/// Size of board
	/// Search depth
	/// Number of generations
	/// Population size
	/// Mutation rate
	/// Randomness
	//Genetic *genetic = new Genetic(12, 3, 4, 8, 0.05, 3);

	//waitKey(0);

	// The difficulty of each player
	int playerOneDifficulty = 0;
	int playerTwoDifficulty = 0;

	// Returns 1 - Human
	// Returns 2 - Monte Carlo
	// Returns 3 - MiniMax

	int gameMode = chooseGameMode();
	
	if (gameMode == 1) {
		int hexagonBoardSize = chooseGameBoardSizeHexagons();
		playHexagonGame(hexagonBoardSize);
	}
	else {

		// Get player one type
		int playerOne = choosePlayerTypes("Select player 1 type (black)", 0);

		// If it's MonteCarlo
		if (playerOne == 2) {
			playerOneDifficulty = chooseAIDifficulty(2);
			//cout << "Player 1 difficulty = " << playerOneDifficulty << endl;
		}
		// If MiniMax
		else if (playerOne == 3) {
			playerOneDifficulty = chooseAIDifficulty(3);
			//cout << "Player 1 difficulty = " << playerOneDifficulty << endl;
		}

		// Select player 2 type
		int playerTwo = choosePlayerTypes("Select player 2 type (white)", 255);

		// If Monte Carlo
		if (playerTwo == 2) {
			playerTwoDifficulty = chooseAIDifficulty(2);
			//cout << "Player 2 difficulty = " << playerTwoDifficulty << endl;
		}
		// If MiniMax
		else if (playerTwo == 3) {
			playerTwoDifficulty = chooseAIDifficulty(3);
			//cout << "Player 2 difficulty = " << playerTwoDifficulty << endl;
		}

		// Get the size of the board
		int boardSize = chooseGameBoardSizeSquares();



		// If Human player 1
		if (playerOne == 1) {
			// Create player object
			Player *p1 = new Player();

			// If Human player 2
			if (playerTwo == 1) {
				Player *p2 = new Player();
				playGame(p1, p2, boardSize);
			}
			// If Monte Carlo player 2
			else if (playerTwo == 2) {
				MonteCarlo *p2 = new MonteCarlo(playerTwoDifficulty);
				playGame(p1, p2, boardSize);
			}
			// If MiniMax player 3
			else if (playerTwo == 3) {
				MiniMax *p2 = new MiniMax(boardSize, playerTwoDifficulty, 5241, 9975, 1958, 2497, 921, 522, 478, 309);
				playGame(p1, p2, boardSize);
			}
		}
		// If Monte Carlo player 1
		else if (playerOne == 2) {
			// Make Monte Carlo object
			MonteCarlo *p1 = new MonteCarlo(playerOneDifficulty);

			if (playerTwo == 1) {
				Player *p2 = new Player();
				playGame(p1, p2, boardSize);
			}
			// If Monte Carlo player 2
			else if (playerTwo == 2) {
				MonteCarlo *p2 = new MonteCarlo(playerTwoDifficulty);
				playGame(p1, p2, boardSize);
			}
			// If MiniMax player 3
			else if (playerTwo == 3) {
				MiniMax *p2 = new MiniMax(boardSize, playerTwoDifficulty, 5241, 9975, 1958, 2497, 921, 522, 478, 309);
				playGame(p1, p2, boardSize);
			}
		}
		// If MiniMax player 1
		else if (playerOne == 3) {
			// Make MiniMax object
			MiniMax *p1 = new MiniMax(boardSize, playerOneDifficulty, 5241, 9975, 1958, 2497, 921, 522, 478, 309);

			// If Human player 1
			if (playerTwo == 1) {
				Player *p2 = new Player();
				playGame(p1, p2, boardSize);
			}
			// If Monte Carlo player 2
			else if (playerTwo == 2) {
				MonteCarlo *p2 = new MonteCarlo(playerTwoDifficulty);
				playGame(p1, p2, boardSize);
			}
			// If Minimax player 2
			else if (playerTwo == 3) {
				MiniMax *p2 = new MiniMax(boardSize, playerTwoDifficulty, 5241, 9975, 1958, 2497, 921, 522, 478, 309);
				playGame(p1, p2, boardSize);
			}
		}
	}

	
	// corner = Weighting of Stable corners
	// adjacent
	// edge = Weighting of Stable edges
	// internalStable = Weighting of Internal Stable discs
	// immedMobilityMultiplier = A constant for Edge Stability
	// potetMobilityMultiplier = A multiplier for Edge Stability
	// multiplierXSquare = A constant for Mobility
	// multiplierDiscCount = A multiplier for Mobility

	//playSquareGame(6);

	// Size of board
	// Search depth
	// Number of generations
	// Population size
	// Mutation rate
	// Randomness
	//Genetic *x = new Genetic(8, 3, 4, 8, 0.2, 3);
	//compete();


	// 5241, 9975, 1958, 2497, 921, 522, 478, 309											DEFAULT
	//int boardSize = 8;				//		cor     adj   edge  intS  imm  pot  X	  disc
	//MiniMax *p1 = new MiniMax(boardSize, 5, 5241, 9975, 1958, 2497, 921, 522, 478, 309);
	//MiniMax *p2 = new MiniMax(boardSize, 5, 15241, 19975, 1958, 2497, 921, 522, 478, 309);
	//playGame(p1, p2, boardSize);
	//playGame(p2, p1, boardSize);
	cout << "Done" << endl;

	return 0;
}		