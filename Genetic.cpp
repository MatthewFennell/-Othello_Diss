#include "Board.h"
#include "MiniMax.h"
#include "Genetic.h"
#include <iostream>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "Player.h"
#include <algorithm>
#include <iterator> 

using namespace cv;
using namespace std;


Genetic::Genetic(int sizeOfBoard, int searchDepth, int numberOfGens, int popSize, double mutationRate, int randomness) {

	numberOfGenerations = numberOfGens;

	populationSize = popSize;

	mutationProbability = mutationRate;

	variance = randomness;

	boardSize = sizeOfBoard;

	maxSearchDepth = searchDepth;

	// Randomly set the seed
	srand(time(NULL));

	// Array containing AI's in the population
	populationMembers = new MiniMax*[populationSize];

	nextGenerationMembers = new MiniMax*[populationSize];

	// Respective number of wins of each AI
	numberOfWins = new int[populationSize];

	// Generate a random population
	for (int x = 0; x < populationSize; x++) {

		// Random number between 0 and 10,000
		int cornerValue = rand() % 10001;
		
		// Random number between 0 and 20,000
		int adjacentValue = rand() % 20001;

		// Random number between 0 and 4000
		int edgeValue = rand() % 4001;

		// Random number between 0 and 5000
		int internalValue = rand() % 5001;

		// Random number between 0 and 2000	
		int immediateMobilityValue = rand() % 2001;

		// Random number between 0 and 1000
		int potentialMobilityValue = rand() % 1001;

		// Random number between 0 and 1000
		int XSquareValue = rand() % 1001;

		// Random number between 0 and 600
		int discCountValue = rand() % 601;

		// Create the AIs with random chromosones
		populationMembers[x] = new MiniMax(boardSize, maxSearchDepth, cornerValue, adjacentValue, edgeValue, internalValue, immediateMobilityValue, potentialMobilityValue, XSquareValue, discCountValue);
	}

	for (int x = 0; x < numberOfGenerations; x++) {
		cout << "Generation = " << x << endl;
		fightToTheDeath();
		selection();
		crossOver();
		transferGenerations();
	}
	
	cout << "FINAL HEURISTIC RESULTS" << endl;
	cout << "Number of generations = " << numberOfGenerations << ", population size = " << populationSize << ", search depth = " << searchDepth << endl << endl;
	populationMembers[0]->printHeuristics();

	//int corner = 0;
	//int adjacent = 0;
	//int edge = 0;
	//int internall = 0;
	//int immediate = 0;
	//int potential = 0;
	//int xSquare = 0;
	//int disc = 0;

	//for (int x = 0; x < populationSize; x++) {
	//	corner += populationMembers[x]->getCornerValue();
	//	adjacent += populationMembers[x]->getAdjacentValue();
	//	edge += populationMembers[x]->getEdgeValue();
	//	internall += populationMembers[x]->getInternalValue();
	//	immediate += populationMembers[x]->getImmediateMobilityMultiplier();
	//	potential += populationMembers[x]->getPotentialMobilityMultiplier();
	//	xSquare += populationMembers[x]->getXSquareMultiplier();
	//	disc += populationMembers[x]->getDiscCountMultiplier();
	//}

	//corner = corner / populationSize;
	//adjacent = adjacent / populationSize;
	//edge = edge / populationSize;
	//internall = internall / populationSize;
	//immediate = immediate / populationSize;
	//potential = potential / populationSize;
	//xSquare = xSquare / populationSize;
	//disc = disc / populationSize;

	//cout << "Corner = " << corner << endl;
	//cout << "Adjacent = " << adjacent << endl;
	//cout << "Edge = " << edge << endl;
	//cout << "Internal = " << internall << endl;
	//cout << "Immediate = " << immediate << endl;
	//cout << "Potential = " << potential << endl;
	//cout << "xSquare = " << xSquare << endl;
	//cout << "Disc = " << disc << endl;

}

// corner = Weighting of Stable corners
// edge = Weighting of Stable edges
// internalStable = Weighting of Internal Stable discs
// immedMobilityMultiplier = A constant for Edge Stability
// potetMobilityMultiplier = A multiplier for Edge Stability
// multiplierXSquare = A constant for Mobility
// multiplierDiscCount = A multiplier for Mobility



// Deallocates a player object
Genetic::~Genetic()
{
	// all deallocating is done automatically
}

// Calculates number of wins per AI in a round robin
void Genetic::fightToTheDeath() {

	// Set all of the win numbers to 0
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
				double percentage = 100*double(gameNumber) / double((populationSize/2 * (populationSize - 1)));
				printf("%.1f %%\r", percentage);
				fflush(stdout);
				//cout << percentage << "% \r" << endl;


				//cout << "Child " << x << " playing against child " << y << endl;
				int winnerGameOne = playSquareGameComputerComputerMiniMax(populationMembers[x], populationMembers[y], boardSize);
				if (winnerGameOne == 1) {
					numberOfWins[y] += 1;
				}
				else if (winnerGameOne == -1) {
					numberOfWins[x] += 1;
				}
				else {
					numberOfDraws += 1;
				}

				int winnerGameTwo = playSquareGameComputerComputerMiniMax(populationMembers[y], populationMembers[x], boardSize);
				if (winnerGameTwo == 1) {
					numberOfWins[x] += 1;
				}
				else if (winnerGameTwo == -1) {
					numberOfWins[y] += 1;
				}
				else {
					numberOfDraws += 1;
				}
			}
		}
	}
	//cout << "There were " << numberOfDraws << " draws " << endl;
	//cout << "----------------------Number of wins -------------------" << endl;
	for (int x = 0; x < populationSize; x++) {
		//cout << "Child " << x << " has " << numberOfWins[x] << " wins " << endl;
	}
}


// Find the n/2 AIs with the most wins
// Add them to the next generation
void Genetic::selection() {

	//cout << "----------------------Selecting children -------------------" << endl;

	for (int y = 0; y < populationSize / 2; y++) {

		int mostWins = -1;
		int largestIndex = -1;

		// Finds the AI with the most wins
		// Adds it to the generation
		// Multiplies its win by -1 so it isn't added twice
		// O(n^2) but only a small sample
		for (int x = 0; x < populationSize; x++) {
			if (numberOfWins[x] > mostWins) {
				mostWins = numberOfWins[x];
				largestIndex = x;
			}
		}

		//cout << "Selected child " << largestIndex << endl;


		int corner = populationMembers[largestIndex]->getCornerValue();
		int adjacent = populationMembers[largestIndex]->getAdjacentValue();
		int edge = populationMembers[largestIndex]->getEdgeValue();
		int intern = populationMembers[largestIndex]->getInternalValue();
		int immediateMobility = populationMembers[largestIndex]->getImmediateMobilityMultiplier();
		int potentialMobility = populationMembers[largestIndex]->getPotentialMobilityMultiplier();
		int xSquare = populationMembers[largestIndex]->getXSquareMultiplier();
		int discCount = populationMembers[largestIndex]->getDiscCountMultiplier();
		nextGenerationMembers[y] = new MiniMax(boardSize, maxSearchDepth, corner, adjacent, edge, intern, immediateMobility, potentialMobility, xSquare, discCount);


		//nextGenerationMembers[y] = populationMembers[largestIndex];
		//cout << "Child number " << y << " made " << endl;
		numberOfWins[largestIndex] *= -1;
	}

	// Delete the old generation
	for (int x = 0; x < populationSize; x++) {
	//	delete populationMembers[x];
	}
}

void Genetic::crossOver() {
	//cout << "Making children " << endl;
	for (int x = 0; x < populationSize / 2; x++) {
		int parentOne = rand() %  populationSize / 2;
		int parentTwo = rand() % populationSize / 2;		

		// Ensures that the parents are different
		while (parentOne == parentTwo) {
			parentTwo = rand() % populationSize / 2;
		}

		//cout << "Parent one = " << parentOne << endl;
		//cout << "Parent two = " << parentTwo << endl;
		
		// Inherit a combination of the parents chromosones	
		double parentOneRatio = ((double)rand() / (RAND_MAX));
		
		//cout << "Parent one ratio = " << parentOneRatio << endl;

		int cornerValue = int(parentOneRatio * double(nextGenerationMembers[parentOne]->getCornerValue()) + (1.0 - parentOneRatio) * double(nextGenerationMembers[parentTwo]->getCornerValue()));

		int adjacentValue = int(parentOneRatio * double(nextGenerationMembers[parentOne]->getAdjacentValue()) + (1.0 - parentOneRatio) * double(nextGenerationMembers[parentTwo]->getAdjacentValue()));

		int edgeValue = int(parentOneRatio * double(nextGenerationMembers[parentOne]->getEdgeValue()) + (1.0 - parentOneRatio) * double(nextGenerationMembers[parentTwo]->getEdgeValue()));

		int internalValue = int(parentOneRatio * double(nextGenerationMembers[parentOne]->getInternalValue()) + (1.0 - parentOneRatio) * double(nextGenerationMembers[parentTwo]->getInternalValue()));

		int immediateMobility = int(parentOneRatio * double(nextGenerationMembers[parentOne]->getImmediateMobilityMultiplier()) + (1.0 - parentOneRatio) * double(nextGenerationMembers[parentTwo]->getImmediateMobilityMultiplier()));

		int potentialMobility = int(parentOneRatio * double(nextGenerationMembers[parentOne]->getPotentialMobilityMultiplier()) + (1.0 - parentOneRatio) * double(nextGenerationMembers[parentTwo]->getPotentialMobilityMultiplier()));

		int xSquareValue = int(parentOneRatio * double(nextGenerationMembers[parentOne]->getXSquareMultiplier()) + (1.0 - parentOneRatio) * double(nextGenerationMembers[parentTwo]->getXSquareMultiplier()));

		int discCountValue = int(parentOneRatio * double(nextGenerationMembers[parentOne]->getDiscCountMultiplier()) + (1.0 - parentOneRatio) * double(nextGenerationMembers[parentTwo]->getDiscCountMultiplier()));

		//cout << "New child made with values : " << endl;

		nextGenerationMembers[populationSize / 2 + x] = new MiniMax(boardSize, maxSearchDepth, cornerValue, adjacentValue, edgeValue, internalValue, immediateMobility, potentialMobility, xSquareValue, discCountValue);
		//nextGenerationMembers[populationSize / 2 + x]->printHeuristics();
	}
}

void Genetic::transferGenerations() {

	for (int x = 0; x < populationSize; x++) {
		populationMembers[x] = nextGenerationMembers[x];
		//delete nextGenerationMembers[x];
	}

	//delete[] nextGenerationMembers;
}

// Plays against the computer, with the human player going first
int Genetic::playSquareGameComputerComputerMiniMax(MiniMax *p1, MiniMax *p2, int boardSize) {

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

	// Player *p3 = new Player();

	bool placingVoids = false;			// CAN BE CHANGED

	int previousLocation = 0;
	double newLocation = 0;

	//while (placingVoids) {

	//	int openingWidth = 800;
	//	int openingHeight = 500;

	//	// Font, size and thickness settings
	//	int fontFace = FONT_HERSHEY_COMPLEX;
	//	double fontScale = 1.0;
	//	int thickness = 2;

	//	// Location of where to put the test (done across 2 lines)
	//	Point textOrgLineOne(openingWidth / 6, openingHeight / 14);
	//	Point textOrgLineTwo(openingWidth / 4, openingWidth - 2 * offset / 5);

	//	string textLineOne = "Finish selecting unplayables";
	//	string textLineTwo = "Generate random seed";



	//	putText(pictureOfBoard, textLineOne, textOrgLineOne, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);
	//	putText(pictureOfBoard, textLineTwo, textOrgLineTwo, fontFace, fontScale, Scalar(255, 255, 255), thickness, 8);

	//	for (int x = 0; x < 10; x++) {
	//		//cout << "new location = " << newLocation << endl;
	//		if (x == newLocation) {
	//			rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 0, 150), CV_FILLED, 8, 0);
	//		}
	//		else {
	//			rectangle(pictureOfBoard, Point(offset / 12, offset / 2 + x * 70), Point(offset / 12 + 30, offset / 2 + 30 + x * 70), Scalar(0, 255, 255), CV_FILLED, 8, 0);
	//		}
	//	}

	//	int* unplayabalePixelSelection = p3->getMove(pictureOfBoard, *board);



	//	for (double x = 0; x < 10; x++) {
	//		if (unplayabalePixelSelection[0] > offset / 12 && unplayabalePixelSelection[0] < offset / 12 + 30 && unplayabalePixelSelection[1] > offset / 2 + x * 70 && unplayabalePixelSelection[1] < offset / 2 + 30 + x * 70) {
	//			//cout << "x = " << x << endl;
	//			newLocation = x;
	//		}
	//	}


	//	if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 13 && unplayabalePixelSelection[1] < 41) {
	//		pictureOfBoard = board->drawBoard();
	//		placingVoids = false;
	//	}

	//	if (unplayabalePixelSelection[0] > 130 && unplayabalePixelSelection[0] < 640 && unplayabalePixelSelection[1] > 700 && unplayabalePixelSelection[1] < 800) {
	//		srand(time(NULL));
	//		for (int i = 0; i < boardSize; i++) {
	//			for (int j = 0; j < boardSize; j++) {

	//				// Sets the disc back to 0
	//				board->setDiscEmpty(i, j);
	//				//cout << "new location / 5 = " << double(newLocation / 5.0) << endl;
	//				// Gives the square a random chance of becoming unplayable
	//				if (rand() % 100 + 1 < double((newLocation + 1) * 2)) {
	//					board->setDiscUnplayable(i, j);
	//				}
	//			}
	//		}

	//		// Set the center co-ordinates back to the start
	//		board->setInitialSquares();
	//		pictureOfBoard = board->drawBoard();
	//	}

	//	if (placingVoids) {
	//		if (unplayabalePixelSelection[0] > offset) {
	//			int* unplayableMoves = board->scalePixelSelection(unplayabalePixelSelection);
	//			board->setDiscUnplayable(unplayableMoves[0], unplayableMoves[1]);
	//			pictureOfBoard = board->drawBoard();
	//		}
	//	}

	//}
	//delete p3;
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
		waitKey(1);

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
				//waitKey(1);
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
		waitKey(1);
		// Player 2 turn ended
		/* ---------------------------------------------------------------------------------------------------------------------------------*/
	}
	// Shows a picture of the board at the end of the game
	pictureOfBoard = board->drawBoard();
	imshow("Board with discs", pictureOfBoard);

	waitKey(1);

	// Works out who won
	int winner = board->printWinner();

	if (winner == 1) {
		//cout << "white won" << endl;
	}
	else if (winner == -1) {
		//cout << "black won" << endl;
	}
	else {
		//cout << "No winner " << endl;
	}

	//cout << "Game finished " << endl;

	return winner;

}
