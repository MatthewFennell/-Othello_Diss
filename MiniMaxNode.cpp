#include "MiniMaxNode.h"
#include "Board.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include <math.h> 

using namespace cv;
using namespace std;

// Create a MiniMaxNode object
// corner = Weighting of Stable corners
// edge = Weighting of Stable edges
// internalStable = Weighting of Internal Stable discs
// immedMobilityMultiplier = A constant for Edge Stability
// potetMobilityMultiplier = A multiplier for Edge Stability
// multiplierXSquare = A constant for Mobility
// multiplierDiscCount = A multiplier for Mobility
MiniMaxNode::MiniMaxNode(MiniMaxNode *parentNode, int **boardState, int player, int sizeOfBoard, int* selectedMove, int maxPlayer, int corner, int adjacent, int edge, int internalStable, int immedMobilityMultiplier, int potetMobilityMultiplier, int multiplierXSquare, int multiplierDiscCount)
{
	// Initially the MiniMaxNode has not been expanded
	beenExpanded = false;

	maximisingPlayerNumber = maxPlayer;

	// How big the board is
	boardSize = sizeOfBoard;

	// Initialise the disc arrays
	othelloBoard = new int*[boardSize];
	for (int i = 0; i < boardSize; i++) {
		othelloBoard[i] = new int[boardSize];
	}

	// Sets the board values
	for (int x = 0; x < boardSize; x += 1) {
		for (int y = 0; y < boardSize; y += 1) {
			othelloBoard[x][y] = boardState[x][y];
		}
	}

	choiceOfMove = selectedMove;

	// Stores the respective parent of this MiniMaxNode
	parent = parentNode;

	// Stores the player number of the player who made the move to visit this MiniMaxNode
	currentPlayer = player;

	// The number of children
	numberOfChildren = 0;

	// The respective evaluation function of this node's board
	evaluationNumber = 0;

	// The potential mobilty for the current player
	potentialMobilityCurrentPlayer = 0;

	// The potential mobility for the other player
	potentialMobilityOtherPlayer = 0;

	stableCornerWeighting = corner;
	stableAdjacentCornerWeighting = adjacent;
	stableEdgeWeighting = edge;
	stableInternalWeighting = internalStable;

	immediateMobilityMultiplier = immedMobilityMultiplier;
	potentialMobilityMultiplier = potetMobilityMultiplier;

	xSquareMultiplier = multiplierXSquare;
	discCountMultiplier = multiplierDiscCount;
}

// Very important manual memory management
MiniMaxNode::~MiniMaxNode()
{
	// For each MiniMaxNode, iterate through the children, and delete the child
	// This will recursively delete all of the children
	for (int x = 0; x < numberOfChildren; x += 1) {
		MiniMaxNode *child = getChild(x);
		delete child;
	}

	// Once a MiniMaxNode has deleted all of it's children, delete it's delete the board states
	for (int i = 0; i < boardSize; i++) {
		delete othelloBoard[i];
	}
	delete othelloBoard;

	delete[] choiceOfMove;

	//printBoardValues();

	// Delete the container for the children
	delete children;
}

void MiniMaxNode::deleteBoard() {
	for (int i = 0; i < boardSize; i++) {
		delete othelloBoard[i];
	}
	delete othelloBoard;
}

// The evaluation function
int MiniMaxNode::evaluatePosition() {

	// Check if the game has ended
	// If won, return a very large positive value (good)
	// If lost, return a very large negative value (bad)
	// Draw do nothing
	int whoWon = hasWon();

	if (whoWon == maximisingPlayerNumber) {
		//cout << "Has won" << endl;
		return 99999999;
	}
	else if (whoWon == maximisingPlayerNumber*-1) {
		//cout << "Has lost" << endl;
		return -9999999;
	}
	else {

	}
	

	int evaluationValue = 0;

	// How far into the game it is (number of moves taken)
	int moveNumber = getMoveNumber();

	// Finds which discs are stable
	// 1 = Stable, 0 = Non stable
	findStables();

	// Check if corners are stable
	if (stableDiscs[0][0] == 1) {
		if (othelloBoard[0][0] == maximisingPlayerNumber) {
			//cout << "The square at 0,0 is stable (+700) for the current player" << endl;
			evaluationValue += stableCornerWeighting;
		}
		else {
			//cout << "The square at 0,0 is stable (-700) for the OTHER player" << endl;
			evaluationValue -= stableCornerWeighting;
		}
	}
	if (stableDiscs[0][boardSize - 1] == 1) {
		if (othelloBoard[0][boardSize - 1] == maximisingPlayerNumber) {
			//cout << "The square at " << boardSize - 1 << ",0 is stable (+700) for the current player" << endl;
			evaluationValue += stableCornerWeighting;
		}
		else {
			//cout << "The square at " << boardSize - 1 << ",0 is stable (-700) for the OTHER player" << endl;
			//evaluationValue += ESAC() * 700;
			evaluationValue -= stableCornerWeighting;
		}
	}
	if (stableDiscs[boardSize - 1][0] == 1) {
		if (othelloBoard[boardSize - 1][0] == maximisingPlayerNumber) {
			//cout << "The square at 0, " << boardSize - 1 << " is stable (+ 700) for the current player" << endl;
			evaluationValue += stableCornerWeighting;
		}
		else {
			//cout << "The square at 0, " << boardSize - 1 << " is stable (-700) for the OTHER player" << endl;
			evaluationValue -= stableCornerWeighting;
		}
	}
	if (stableDiscs[boardSize - 1][boardSize - 1] == 1) {
		if (othelloBoard[boardSize - 1][boardSize - 1] == maximisingPlayerNumber) {
			//cout << "The square at " << boardSize - 1 << "," << boardSize - 1 << " is stable (+700) for the current player" << endl;
			evaluationValue += stableCornerWeighting;
		}
		else {
			//cout << "The square at " << boardSize - 1 << "," << boardSize - 1 << " is stable (-700) for the OTHER player" << endl;
			evaluationValue -= stableCornerWeighting;
		}
	}

	// right 1 of top left corner
	if (stableDiscs[1][0] == 1) {
		if (othelloBoard[1][0] == maximisingPlayerNumber) {
			//cout << "The square at 0,1 is stable (+1000) for the current player" << endl;
			evaluationValue += stableAdjacentCornerWeighting;
		}
		else {
			//cout << "The square at 0,1 is stable (-1000) for the OTHER player" << endl;
			evaluationValue -= stableAdjacentCornerWeighting;
		}
	}

	// down 1 place of top left corner
	if (stableDiscs[0][1] == 1) {
		if (othelloBoard[0][1] == maximisingPlayerNumber) {
			//cout << "The square at 1,0 is stable (+1000) for the current player" << endl;
			evaluationValue += stableAdjacentCornerWeighting;
		}
		else {
			//cout << "The square at 1,0 is stable (-1000) for the OTHER player" << endl;
			evaluationValue -= stableAdjacentCornerWeighting;
		}
	}

	// 1 above bottom left corner
	if (stableDiscs[0][boardSize - 2] == 1) {
		if (othelloBoard[0][boardSize - 2] == maximisingPlayerNumber) {
			//cout << "The square at " << boardSize - 2 << ", 0 is stable (+1000) for the current player" << endl;
			evaluationValue += stableAdjacentCornerWeighting;
		}
		else {
			//cout << "The square at " << boardSize - 2 << ", 0 is stable (-1000) for the OTHER player" << endl;
			evaluationValue -= stableAdjacentCornerWeighting;
		}
	}
	// 1 right of bottom left corner
	if (stableDiscs[1][boardSize - 1] == 1) {
		if (othelloBoard[1][boardSize - 1] == maximisingPlayerNumber) {
			//cout << "The square at " << boardSize - 1 << ",1 is stable (+1000) for the current player" << endl;
			evaluationValue += stableAdjacentCornerWeighting;
		}
		else {
			//cout << "The square at " << boardSize - 1 << ",1 is stable (-1000) for the other player" << endl;
			evaluationValue -= stableAdjacentCornerWeighting;
		}
	}

	// 1 left of top right corner
	if (stableDiscs[boardSize - 2][0] == 1) {
		if (othelloBoard[boardSize - 2][0] == maximisingPlayerNumber) {
			//cout << "The square at 0, " << boardSize - 2 << " is stable (+1000) for the current player" << endl;
			evaluationValue += stableAdjacentCornerWeighting;
		}
		else {
			//cout << "The square at 0, " << boardSize - 2 << " is stable (-1000) for the OTHER player" << endl;
			evaluationValue -= stableAdjacentCornerWeighting;
		}
	}

	// 1 below top right corner
	if (stableDiscs[boardSize - 1][1] == 1) {
		if (othelloBoard[boardSize - 1][1] == maximisingPlayerNumber) {
			//cout << "The square at 1, " << boardSize - 1 << " is stable (+1000) for the current player" << endl;
			evaluationValue += stableAdjacentCornerWeighting;
		}
		else {
			//cout << "The square at 1, " << boardSize - 1 << " is stable (-1000) for the OTHER player" << endl;
			evaluationValue -= stableAdjacentCornerWeighting;
		}
	}

	// 1 left of bottom right corner
	if (stableDiscs[boardSize - 2][boardSize - 1] == 1) {
		if (othelloBoard[boardSize - 2][boardSize - 1] == maximisingPlayerNumber) {
			//cout << "The square at " << boardSize - 1 << "," << boardSize - 2 << " is stable (+1000) for the current player" << endl;
			evaluationValue += stableAdjacentCornerWeighting;
		}
		else {
			//cout << "The square at " << boardSize - 1 << "," << boardSize - 2 << " is stable (-1000) for the OTHER player" << endl;
			evaluationValue -= stableAdjacentCornerWeighting;
		}
	}

	// 1 up of bottom left corner
	if (stableDiscs[boardSize - 1][boardSize - 2] == 1) {
		if (othelloBoard[boardSize - 1][boardSize - 2] == maximisingPlayerNumber) {
			//cout << "The square at " << boardSize - 2 << "," << boardSize - 1 << " is stable (+1000) for the current player" << endl;
			evaluationValue += stableAdjacentCornerWeighting;
		}
		else {
			//cout << "The square at " << boardSize - 2 << "," << boardSize - 1 << " is stable (-1000) for the OTHER player" << endl;
			evaluationValue -= stableAdjacentCornerWeighting;
		}
	}

	for (int x = 2; x < boardSize - 2; x += 1) {
		if (stableDiscs[x][0] == 1) {
			if (othelloBoard[x][0] == maximisingPlayerNumber) {
				//cout << "The square at 0, " << x << " is stable (+500) for the current player" << endl;
				evaluationValue += stableEdgeWeighting;
			}
			else {
				//cout << "The square at 0, " << x << " is stable (-500) for the OTHER player" << endl;
				evaluationValue -= stableEdgeWeighting;
			}
		}

		// Horizontal bottom row
		if (stableDiscs[x][boardSize - 1] == 1) {
			if (othelloBoard[x][boardSize - 1] == maximisingPlayerNumber) {
				//cout << "The square at " << boardSize - 1 << "," << x << " is stable (+500) for the current player" << endl;
				evaluationValue += stableEdgeWeighting;
			}
			else {
				//cout << "The square at " << boardSize - 1 << "," << x << " is stable (-500) for the OTHER player" << endl;
				evaluationValue -= stableEdgeWeighting;
			}
		}
	}

	// Vertical left column
	for (int y = 2; y < boardSize - 2; y += 1) {
		if (stableDiscs[0][y] == 1) {
			if (othelloBoard[0][y] == maximisingPlayerNumber) {
				//cout << "The square at " << y << ", 0 is stable (+500) for the current player" << endl;
				evaluationValue += stableEdgeWeighting;
			}
			else {
				//cout << "The square at " << y << ", 0 is stable (-500) for the OTHER player" << endl;
				evaluationValue -= stableEdgeWeighting;
			}
		}

		// Vertical right column
		if (stableDiscs[boardSize - 1][y] == 1) {
			if (othelloBoard[boardSize - 1][y] == maximisingPlayerNumber) {
				//cout << "The square at " << y << "," << boardSize - 1 << " is stable (+500) for the current player" << endl;
				evaluationValue += stableEdgeWeighting;
			}
			else {
				//cout << "The square at " << y << "," << boardSize - 1 << " is stable (-500) for the OTHER player" << endl;
				evaluationValue -= stableEdgeWeighting;
			}
		}
	}

	for (int x = 1; x < boardSize - 1; x += 1) {
		for (int y = 1; y < boardSize - 1; y += 1) {
			if (stableDiscs[y][x]) {
				if (othelloBoard[y][x] == maximisingPlayerNumber) {
					//cout << "The square at " << x << "," << y << " is stable (+10) for the current player" << endl;
					evaluationValue += stableInternalWeighting;
				}
				else {
					//cout << "The square at " << x << "," << y << " is stable (-10) for the OTHER player" << endl;
					evaluationValue -= stableInternalWeighting;
				}
			}
		}
	}

	int immedMobility = immediateMobility();
	evaluationValue += immedMobility;

	potentialMobility();

	evaluationValue += potentialMobilityMultiplier*potentialMobilityCurrentPlayer;
	evaluationValue -= potentialMobilityMultiplier*potentialMobilityOtherPlayer;

	// Calculate X-Square values
	//evaluationValue += xSquareMultiplier * XSquares();

	for (int i = 0; i < boardSize; i++) {
		delete stableDiscs[i];
	}
	delete stableDiscs;

	return evaluationValue;
}

int MiniMaxNode::XSquares() {

	// Penalty for holding X-Squares (Will be a negative value)
	int bonus = 0;
	
	// If a square is unstable and an X-Square controlled by a player
	if (stableDiscs[1][1] == 0) {
		if (othelloBoard[1][1] == maximisingPlayerNumber) {
			//cout << "x square top left for current player" << endl;
			bonus -= 1;
		}
		else if (othelloBoard[1][1] == maximisingPlayerNumber * -1) {
			//cout << "x square top left for other player" << endl;
			bonus += 1;
		}
	}

	if (stableDiscs[1][boardSize - 2] == 0) {
		if (othelloBoard[1][boardSize - 2] == maximisingPlayerNumber) {
			//cout << "x square top right for current player" << endl;
			bonus -= 1;
		}
		else if (othelloBoard[1][boardSize - 2] == maximisingPlayerNumber * -1) {
			//cout << "x square top right for other player" << endl;
			bonus += 1;
		}
	}

	if (stableDiscs[boardSize - 2][1] == 0) {
		if (othelloBoard[boardSize - 2][1] == maximisingPlayerNumber) {
			//cout << "x square bottom left for current player" << endl;
			bonus -= 1;
		}
		else if (othelloBoard[boardSize - 2][1] == maximisingPlayerNumber * -1) {
			//cout << "x square bottom left for other player" << endl;
			bonus += 1;
		}
	}

	if (stableDiscs[boardSize - 2][boardSize - 2] == 0) {
		if (othelloBoard[boardSize - 2][boardSize - 2] == maximisingPlayerNumber) {
			//cout << "x square bottom right for current player" << endl;
			bonus -= 1;
		}
		else if (othelloBoard[boardSize - 2][boardSize - 2] == maximisingPlayerNumber * -1) {
			//cout << "x square bottom right for other player" << endl;
			bonus += 1;
		}
	}

	//cout << "Bonus = " << bonus << endl;
	return bonus;
}

// ESAC Move number from IAGO
int MiniMaxNode::getMoveNumber() {
	int moveNumber = 0;

	for (int x = 0; x < boardSize; x += 1) {
		for (int y = 0; y < boardSize; y += 1) {
			if (othelloBoard[y][x] == 1 || othelloBoard[y][x] == -1) {
				moveNumber += 1;
			}
		}
	}
	moveNumber -= 4;
	return moveNumber;
}

int MiniMaxNode::ESAC(int moveNumber) {

	// Based off IAGO
	// 36/64 scales it to a smaller board
	if (boardSize == 6) {
		return 0;
		//return int(36.0 / 64.0 * double(ESACconstant) + 36.0 / 64.0 * double(ESACmultiplier) * double(moveNumber));
	}
	else {
		return 0;
		//return int(double(ESACconstant) + double(ESACmultiplier) * double(moveNumber));
	}

}

int MiniMaxNode::CMAC(int moveNumber) {

	if (boardSize == 6) {
		// 14 = 36 / 64 * 25 (FROM IAGO)
		if (moveNumber < 14) {
			return 0;
			//return int(36.0 / 64.0 * double(CMACconstant) + double(CMACmultiplier) * double(moveNumber));
		}
		else {
			return 0;
			//return int(36.0 / 64.0 * double(CMACconstant) + double(moveNumber));
		}
	}
	else {
		if (moveNumber < 25) {
			return 0;
			//return int(double(CMACconstant) + double(CMACmultiplier) * double(moveNumber));
		}
		else {
			return 0;
			//return int(double((CMACconstant*1.50)) + double(moveNumber));
		}
	}
}

// Counts how many discs are of the colour of the maximising player
int MiniMaxNode::discCount() {
	int total = 0;
	for (int x = 0; x < boardSize; x += 1) {
		for (int y = 0; y < boardSize; y += 1) {
			if (othelloBoard[x][y] == maximisingPlayerNumber) {
				total += 1;
			}
		}
	}
	return total;
}

// Sets the array of child MiniMaxNodes
void MiniMaxNode::setChildren(MiniMaxNode** childArray, int numberChildren) {
	children = childArray;
}

// Returns the parent MiniMaxNode
MiniMaxNode* MiniMaxNode::getParent() {
	return parent;
}

// Shows that this MiniMaxNode has been expanded
void MiniMaxNode::setExpandedTrue() {
	beenExpanded = true;
}

// Get if the MiniMaxNode has been expanded or not
bool MiniMaxNode::hasBeenExpanded() {
	return beenExpanded;
}

// Get a given child index
MiniMaxNode* MiniMaxNode::getChild(int childNumber) {
	return children[childNumber];
}

// Get the current player
int MiniMaxNode::getCurrentPlayer() {
	return currentPlayer;
}

// Get the move of the current MiniMaxNode
int* MiniMaxNode::getMove() {
	return choiceOfMove;
}

// Set the current player of this MiniMaxNode
// Only occurs when a player has no available moves and the turn is swapped
void MiniMaxNode::setCurrentPlayer(int player) {
	currentPlayer = player;
}

// Return the array of children belonging to this MiniMaxNode
MiniMaxNode** MiniMaxNode::getChildren() {
	return children;
}

// Set how many children this MiniMaxNode has
void MiniMaxNode::setNumberOfChildren(int numberChildren) {
	numberOfChildren = numberChildren;
}

// Get how many children this MiniMaxNode has
int MiniMaxNode::getNumberOfChildren() {
	return numberOfChildren;
}

// Prints out one row per line in the terminal of the board values
void MiniMaxNode::printBoardValues() {
	for (int x = 0; x < boardSize; x += 1) {
		for (int y = 0; y < boardSize; y += 1) {
			//cout << othelloBoard[y][x] << ", ";
		}
		//cout << endl;
	}
	//cout << endl << endl;
}

int** MiniMaxNode::getBoardValues() {
	return othelloBoard;
}

void MiniMaxNode::setValue(int value) {
	evaluationNumber = value;
}

int MiniMaxNode::getEvalNumber() {
	return evaluationNumber;
}

// All explained in flip Right (very similar for all)
bool MiniMaxNode::flipRight(int row, int column, int player) {

	if (othelloBoard[column][row] == 0) {

		bool availableMove = false;
		bool foundDisc = false;

		while (availableMove == false && column < boardSize - 1) {
			column += 1;

			// If there's a gap, stop
			if (othelloBoard[column][row] == 0 || othelloBoard[column][row] == 2) {
				break;
			}

			// If it is the same colour as your own disc and an opposing disc has not been found, stop
			if (othelloBoard[column][row] == player && foundDisc == false) {
				break;
			}

			// If it is the same colour as your own disc and an opposing disc has been found, return true
			if (othelloBoard[column][row] == player && foundDisc == true) {
				availableMove = true;
				break;
			}

			// Check to see if an opposing disc has been found
			if (othelloBoard[column][row] == player*-1) {
				foundDisc = true;
			}
		}
		return availableMove;
	}
	else {
		return false;
	}
}

bool MiniMaxNode::flipLeft(int row, int column, int player) {

	if (othelloBoard[column][row] == 0) {

		bool availableMove = false;
		bool foundDisc = false;

		while (availableMove == false && column > 0) {
			column -= 1;

			if (othelloBoard[column][row] == 0 || othelloBoard[column][row] == 2) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == false) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == true) {
				availableMove = true;
				break;
			}


			if (othelloBoard[column][row] == player*-1) {
				////cout << "Found opposing disc in square " << column << "," << row << endl;
				foundDisc = true;
			}
		}
		return availableMove;
	}
	else {
		return false;
	}
}

bool MiniMaxNode::flipUp(int row, int column, int player) {

	if (othelloBoard[column][row] == 0) {

		bool availableMove = false;
		bool foundDisc = false;

		while (availableMove == false && row > 0) {
			row -= 1;

			if (othelloBoard[column][row] == 0 || othelloBoard[column][row] == 2) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == false) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == true) {
				availableMove = true;
				break;
			}

			if (othelloBoard[column][row] == player*-1) {
				////cout << "Found opposing disc in square " << column << "," << row << endl;
				foundDisc = true;
			}
		}
		return availableMove;
	}
	else {
		return false;
	}
}

bool MiniMaxNode::flipDown(int row, int column, int player) {
	if (othelloBoard[column][row] == 0) {

		bool availableMove = false;
		bool foundDisc = false;

		while (availableMove == false && row < boardSize - 1) {
			row += 1;

			if (othelloBoard[column][row] == 0 || othelloBoard[column][row] == 2) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == false) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == true) {
				availableMove = true;
				break;
			}

			if (othelloBoard[column][row] == player*-1) {
				////cout << "Found opposing disc in square " << column << "," << row << endl;
				foundDisc = true;
			}
		}
		return availableMove;
	}
	else {
		return false;
	}
}

bool MiniMaxNode::flipRightDown(int row, int column, int player) {

	if (othelloBoard[column][row] == 0) {

		bool availableMove = false;
		bool foundDisc = false;

		while (availableMove == false && column < boardSize - 1 && row < boardSize - 1) {
			row += 1;
			column += 1;

			if (othelloBoard[column][row] == 0 || othelloBoard[column][row] == 2) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == false) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == true) {
				availableMove = true;
				break;
			}

			if (othelloBoard[column][row] == player*-1) {
				////cout << "Found opposing disc in square " << column << "," << row << endl;
				foundDisc = true;
			}
		}
		return availableMove;
	}
	else {
		return false;
	}
}

bool MiniMaxNode::flipRightUp(int row, int column, int player) {

	if (othelloBoard[column][row] == 0) {

		bool availableMove = false;
		bool foundDisc = false;

		while (availableMove == false && column < boardSize - 1 && row > 0) {
			row -= 1;
			column += 1;

			if (othelloBoard[column][row] == 0 || othelloBoard[column][row] == 2) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == false) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == true) {
				availableMove = true;
				break;
			}

			if (othelloBoard[column][row] == player*-1) {
				////cout << "Found opposing disc in square " << column << "," << row << endl;
				foundDisc = true;
			}
		}
		return availableMove;
	}
	else {
		return false;
	}
}

bool MiniMaxNode::flipLeftUp(int row, int column, int player) {

	if (othelloBoard[column][row] == 0) {

		bool availableMove = false;
		bool foundDisc = false;

		while (availableMove == false && column > 0 && row > 0) {
			row -= 1;
			column -= 1;

			if (othelloBoard[column][row] == 0 || othelloBoard[column][row] == 2) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == false) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == true) {
				availableMove = true;
				break;
			}

			if (othelloBoard[column][row] == player*-1) {
				////cout << "Found opposing disc in square " << column << "," << row << endl;
				foundDisc = true;
			}
		}
		return availableMove;
	}
	else {
		return false;
	}
}

bool MiniMaxNode::flipLeftDown(int row, int column, int player) {

	if (othelloBoard[column][row] == 0) {

		bool availableMove = false;
		bool foundDisc = false;

		while (availableMove == false && column > 0 && row < boardSize - 1) {
			row += 1;
			column -= 1;

			if (othelloBoard[column][row] == 0 || othelloBoard[column][row] == 2) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == false) {
				break;
			}

			if (othelloBoard[column][row] == player && foundDisc == true) {
				availableMove = true;
				break;
			}

			if (othelloBoard[column][row] == player*-1) {
				////cout << "Found opposing disc in square " << column << "," << row << endl;
				foundDisc = true;
			}
		}
		return availableMove;
	}
	else {
		return false;
	}
}

// Checks if (row,column) is a valid place to place a disc
bool MiniMaxNode::canPlaceHere(int row, int column, int player) {
	int numberOfMoves = 0;

	if (flipRight(row, column, player)) {
		//cout << "right at row column " << row << "," << column << endl;
		return true;
	}
	if (flipLeft(row, column, player)) {
		//cout << "left at row column " << row << "," << column << endl;
		return true;
	}
	if (flipUp(row, column, player)) {
		//cout << "up at row column " << row << "," << column << endl;
		return true;
	}
	if (flipDown(row, column, player)) {
		//cout << "down at row column " << row << "," << column << endl;
		return true;
	}
	if (flipRightDown(row, column, player)) {
		//cout << "right down at row column " << row << "," << column << endl;
		return true;
	}
	if (flipRightUp(row, column, player)) {
		//cout << "right up at row column " << row << "," << column << endl;
		return true;
	}
	if (flipLeftDown(row, column, player)) {
		//cout << "left down at row column " << row << "," << column << endl;
		return true;
	}
	if (flipLeftUp(row, column, player)) {
		//cout << "left up at row column " << row << "," << column << endl;
		return true;
	}
	return false;
}

// Calculates the number of available moves
int MiniMaxNode::immediateMobility() {
	int numberOfMovesCurrentPlayer = 0;
	int numberOfMovesOtherPlayer = 0;
	for (int x = 0; x < boardSize; x += 1) {
		for (int y = 0; y < boardSize; y += 1) {
			if (canPlaceHere(x, y, maximisingPlayerNumber)) {
				//cout << "Immediate Mobility + 1 from location " << x << ", " << y << endl;
				numberOfMovesCurrentPlayer += 1;
			}
			if (canPlaceHere(x, y, maximisingPlayerNumber*-1)) {
				numberOfMovesOtherPlayer += 1;
			}
		}
	}
	//cout << "Number of moves available to current player = " << numberOfMovesCurrentPlayer << endl;
	//cout << "Number of moves available to other player = " << numberOfMovesOtherPlayer << endl;

	return numberOfMovesCurrentPlayer - numberOfMovesOtherPlayer;
}

void MiniMaxNode::deleteStables() {
	for (int i = 0; i < boardSize; i++) {
		delete stableDiscs[i];
	}
	delete stableDiscs;
}

void MiniMaxNode::findStables() {

	// Initialise the stable discs (0 = not stable, 1 = stable)
	stableDiscs = new int*[boardSize];
	for (int i = 0; i < boardSize; i++) {
		stableDiscs[i] = new int[boardSize];
	}

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			stableDiscs[i][j] = 0;
		}
	}

	// Checks the four corners for stability first
	if (othelloBoard[0][0] == 1 || othelloBoard[0][0] == -1) {
		isStable(0, 0);
	}
	if (othelloBoard[boardSize - 1][0] == 1 || othelloBoard[boardSize - 1][0] == -1) {
		isStable(boardSize - 1, 0);
	}
	if (othelloBoard[0][boardSize - 1] == 1 || othelloBoard[0][boardSize - 1] == -1) {
		isStable(0, boardSize - 1);
	}
	if (othelloBoard[boardSize - 1][boardSize - 1] == 1 || othelloBoard[boardSize - 1][boardSize - 1] == -1) {
		isStable(boardSize - 1, boardSize - 1);
	}

	// Loops from top to bottom and bottom to top of the side columns (up and down on both edges)	
	// If that location isn't stable, checks to see if it is stable
	for (int x = 1; x < boardSize - 1; x += 1) {

		if (othelloBoard[0][x] == 1 || othelloBoard[0][x] == -1) {
			isStable(0, x);
		}

		if (othelloBoard[0][boardSize - 1 - x] == 1 || othelloBoard[0][boardSize - 1 - x] == -1) {
			isStable(0, boardSize - 1 - x);
		}

		if (othelloBoard[boardSize - 1][x] == 1 || othelloBoard[boardSize - 1][x] == -1) {
			isStable(boardSize - 1, x);
		}

		if (othelloBoard[boardSize - 1][boardSize - 1 - x] == 1 || othelloBoard[boardSize - 1][boardSize - 1 - x] == -1) {
			isStable(boardSize - 1, boardSize - 1 - x);
		}
	}

	// These ones count from left to right and right to left
	for (int y = 1; y < boardSize - 1; y += 1) {

		if (othelloBoard[y][0] == 1 || othelloBoard[y][0] == -1) {
			isStable(y, 0);
		}

		if (othelloBoard[boardSize - 1 - y][0] == 1 || othelloBoard[boardSize - 1 - y][0] == -1) {
			isStable(boardSize - 1 - y, 0);
		}

		if (othelloBoard[y][boardSize - 1] == 1 || othelloBoard[y][boardSize - 1] == -1) {
			isStable(y, boardSize - 1);
		}

		if (othelloBoard[boardSize - 1 - y][boardSize - 1] == 1 || othelloBoard[boardSize - 1 - y][boardSize - 1] == -1) {
			isStable(boardSize - 1 - y, boardSize - 1);
		}
	}

	for (int x = 1; x < boardSize - 1; x += 1) {
		for (int y = 1; y < boardSize - 1; y += 1) {
			if (othelloBoard[y][x] == 1 || othelloBoard[y][x] == -1) {
				isStable(y, x);
			}
		}
	}
}

bool MiniMaxNode::horizontalStability(int row, int column) {

	bool horizontalStability = true;

	// Checks to see if it at either the left or right hand edges
	if (column != 0 && column != boardSize - 1) {

		// If the square to the right or left is a blank
		if (othelloBoard[column + 1][row] == 2 || othelloBoard[column - 1][row] == 2) {
			return true;
		}

		for (int currentColumn = 0; currentColumn < boardSize; currentColumn += 1) {
			if (othelloBoard[currentColumn][row] == 0) {
				horizontalStability = false;
				break;
			}
		}
	}

	if (horizontalStability) {
		return true;
	}
	else {
		// If a horizontally adjacent disc is the same colour and stable
		if (othelloBoard[column][row] == othelloBoard[column + 1][row] && stableDiscs[column + 1][row] == 1) {
			return true;
		}
		if (othelloBoard[column][row] == othelloBoard[column - 1][row] && stableDiscs[column - 1][row] == 1) {
			return true;
		}
		return false;
	}
}

bool MiniMaxNode::verticalStability(int row, int column) {

	bool verticalStability = true;

	// Checks to see if it at either the top or bottom hand edges
	if (row != 0 && row != boardSize - 1) {

		// If the square above or below is a blank
		if (othelloBoard[column][row + 1] == 2 || othelloBoard[column][row - 1] == 2) {
			return true;
		}

		for (int currentRow = 0; currentRow < boardSize; currentRow += 1) {
			if (othelloBoard[column][currentRow] == 0) {
				verticalStability = false;
				break;
			}
		}
	}

	if (verticalStability) {
		return true;
	}
	else {
		if (othelloBoard[column][row] == othelloBoard[column][row - 1] && stableDiscs[column][row - 1] == 1) {
			return true;
		}
		if (othelloBoard[column][row] == othelloBoard[column][row + 1] && stableDiscs[column][row + 1] == 1) {
			return true;
		}
		return false;
	}
}

bool MiniMaxNode::rightDownStability(int row, int column) {

	bool rightDownStability = true;

	int rowStart = row;
	int columnStart = column;

	// If at the top, bottom, left or right, it must be stable in this direction
	if (row != 0 && row != boardSize - 1 && column != 0 && column != boardSize - 1) {

		// If the square up the to left or down to the right is unplayable, it must be stable
		if (othelloBoard[column + 1][row + 1] == 2 || othelloBoard[column - 1][row - 1] == 2) {
			return true;
		}

		while (rowStart < boardSize - 1 && columnStart < boardSize - 1) {
			rowStart += 1;
			columnStart += 1;

			if (othelloBoard[columnStart][rowStart] == 0) {
				rightDownStability = false;
				break;
			}
		}

		rowStart = row;
		columnStart = column;

		while (rowStart > 0 && columnStart > 0) {
			rowStart -= 1;
			columnStart -= 1;

			if (othelloBoard[columnStart][rowStart] == 0) {
				rightDownStability = false;
				break;
			}
		}
	}

	if (rightDownStability) {
		return true;
	}
	else {
		if (othelloBoard[column][row] == othelloBoard[column + 1][row + 1] && stableDiscs[column + 1][row + 1] == 1) {
			return true;
		}
		if (othelloBoard[column][row] == othelloBoard[column - 1][row - 1] && stableDiscs[column - 1][row - 1] == 1) {
			return true;
		}
		return false;
	}
}

bool MiniMaxNode::rightUpStability(int row, int column) {

	bool rightUpStability = true;

	int rowStart = row;
	int columnStart = column;


	if (row != 0 && row != boardSize - 1 && column != 0 && column != boardSize - 1) {

		if (othelloBoard[column + 1][row - 1] == 2 || othelloBoard[column - 1][row + 1] == 2) {
			return true;
		}


		while (rowStart > 0 && columnStart < boardSize - 1) {
			rowStart -= 1;
			columnStart += 1;

			if (othelloBoard[columnStart][rowStart] == 0) {
				rightUpStability = false;
				break;
			}
		}

		rowStart = row;
		columnStart = column;

		while (rowStart < boardSize - 1 && columnStart > 0) {
			rowStart += 1;
			columnStart -= 1;

			if (othelloBoard[columnStart][rowStart] == 0) {
				rightUpStability = false;
				break;
			}
		}
	}

	if (rightUpStability) {
		return true;
	}
	else {
		if (othelloBoard[column][row] == othelloBoard[column + 1][row - 1] && stableDiscs[column + 1][row - 1] == 1) {
			return true;
		}
		if (othelloBoard[column][row] == othelloBoard[column - 1][row + 1] && stableDiscs[column - 1][row + 1] == 1) {
			return true;
		}
		return false;
	}

}

bool MiniMaxNode::isStable(int column, int row) {
	// The AND only checks for stable discs of the current player
	if (stableDiscs[column][row] != 1) {

		bool horizontallyStable = horizontalStability(row, column);
		bool verticallyStable = verticalStability(row, column);

		bool rightDownStable = rightDownStability(row, column);
		bool rightUpStable = rightUpStability(row, column);

		if (horizontallyStable && verticallyStable && rightDownStable && rightUpStable) {
			//cout << row << ", " << column << " is fully stable " << endl;
			stableDiscs[column][row] = 1;
		}
	}

	return 0;
}

int MiniMaxNode::potentialMobilityOld() {

	int total = 0;
	for (int x = 0; x < boardSize; x += 1) {
		for (int y = 0; y < boardSize; y += 1) {
			if (othelloBoard[y][x] == maximisingPlayerNumber) {
				total += 15 * y;
				total += 4 * x;
			}
		}
	}
	return total;
}

int MiniMaxNode::potentialMobility() {

	int otherPlayer = maximisingPlayerNumber *-1;

	////cout << "Starting top left corner" << endl;
	if (othelloBoard[0][0] == 0) {
		checkBelow(0, 0, otherPlayer);
		checkRight(0, 0, otherPlayer);
		checkRightDown(0, 0, otherPlayer);
	}

	////cout << "Starting bottom left corner " << endl;
	//// Bottom left corner
	if (othelloBoard[0][boardSize - 1] == 0) {
		checkAbove(0, boardSize - 1, otherPlayer);
		checkRight(0, boardSize - 1, otherPlayer);
		checkRightUp(0, boardSize - 1, otherPlayer);
	}

	////cout << "Starting top right corner " << endl;
	//// Top right corner
	if (othelloBoard[boardSize - 1][0] == 0) {
		checkLeft(boardSize - 1, 0, otherPlayer);
		checkBelow(boardSize - 1, 0, otherPlayer);
		checkLeftDown(boardSize - 1, 0, otherPlayer);
	}

	////cout << "Starting bottom right corner" << endl;
	//// Bottom right corner
	if (othelloBoard[boardSize - 1][boardSize - 1] == 0) {
		checkLeft(boardSize - 1, boardSize - 1, otherPlayer);
		checkAbove(boardSize - 1, boardSize - 1, otherPlayer);
		checkLeftUp(boardSize - 1, boardSize - 1, otherPlayer);
	}

	// right 1 place of top left corner
	if (othelloBoard[1][0] == 0) {
		checkRight(1, 0, otherPlayer);
		checkRightDown(1, 0, otherPlayer);
		checkBelow(1, 0, otherPlayer);
	}

	// down 1 place of top left corner
	if (othelloBoard[0][1] == 0) {
		checkRight(0, 1, otherPlayer);
		checkRightDown(0, 1, otherPlayer);
		checkBelow(0, 1, otherPlayer);
	}

	// 1 above bottom left corner
	if (othelloBoard[0][boardSize - 2] == 0) {
		checkAbove(0, boardSize - 2, otherPlayer);
		checkRightUp(0, boardSize - 2, otherPlayer);
		checkRight(0, boardSize - 2, otherPlayer);
	}
	// 1 right of bottom left corner
	if (othelloBoard[1][boardSize - 1] == 0) {
		checkRight(1, boardSize - 1, otherPlayer);
		checkRightUp(1, boardSize - 1, otherPlayer);
		checkAbove(1, boardSize - 1, otherPlayer);
	}

	// 1 left of top right corner
	if (othelloBoard[boardSize - 2][0] == 0) {
		checkLeft(boardSize - 2, 0, otherPlayer);
		checkLeftDown(boardSize - 2, 0, otherPlayer);
		checkBelow(boardSize - 2, 0, otherPlayer);
	}

	// 1 below top right corner
	if (othelloBoard[boardSize - 1][1] == 0) {
		checkBelow(boardSize - 1, 1, otherPlayer);
		checkLeftDown(boardSize - 1, 1, otherPlayer);
		checkLeft(boardSize - 1, 1, otherPlayer);
	}

	// 1 left of bottom right corner
	if (othelloBoard[boardSize - 2][boardSize - 1] == 0) {
		checkLeft(boardSize - 2, boardSize - 1, otherPlayer);
		checkLeftUp(boardSize - 2, boardSize - 1, otherPlayer);
		checkAbove(boardSize - 2, boardSize - 1, otherPlayer);
	}

	// 1 up of bottom left corner
	if (othelloBoard[boardSize - 1][boardSize - 2] == 0) {
		checkAbove(boardSize - 1, boardSize - 2, otherPlayer);
		checkLeftUp(boardSize - 1, boardSize - 2, otherPlayer);
		checkLeft(boardSize - 1, boardSize - 2, otherPlayer);
	}

	// Horizontal top row
	for (int x = 2; x < boardSize - 2; x += 1) {
		if (othelloBoard[x][0] == 0) {
			checkLeft(x, 0, otherPlayer);
			checkLeftDown(x, 0, otherPlayer);
			checkBelow(x, 0, otherPlayer);
			checkRightDown(x, 0, otherPlayer);
			checkRight(x, 0, otherPlayer);
		}

		// Horizontal bottom row
		if (othelloBoard[x][boardSize - 1] == 0) {
			checkLeft(x, boardSize - 1, otherPlayer);
			checkLeftUp(x, boardSize - 1, otherPlayer);
			checkAbove(x, boardSize - 1, otherPlayer);
			checkRightUp(x, boardSize - 1, otherPlayer);
			checkRight(x, boardSize - 1, otherPlayer);
		}
	}

	// Vertical left column
	for (int y = 2; y < boardSize - 2; y += 1) {
		if (othelloBoard[0][y] == 0) {
			checkAbove(0, y, otherPlayer);
			checkRightUp(0, y, otherPlayer);
			checkRight(0, y, otherPlayer);
			checkRightDown(0, y, otherPlayer);
			checkBelow(0, y, otherPlayer);
		}

		// Vertical right column
		if (othelloBoard[boardSize - 1][y] == 0) {
			checkAbove(boardSize - 1, y, otherPlayer);
			checkLeftUp(boardSize - 1, y, otherPlayer);
			checkLeft(boardSize - 1, y, otherPlayer);
			checkLeftDown(boardSize - 1, y, otherPlayer);
			checkBelow(boardSize - 1, y, otherPlayer);
		}
	}

	// Top left x-square
	if (othelloBoard[1][1] == 0) {
		checkRight(1, 1, otherPlayer);
		checkRightDown(1, 1, otherPlayer);
		checkBelow(1, 1, otherPlayer);
	}

	// Top right x-square
	if (othelloBoard[boardSize - 2][1] == 0) {
		checkLeft(boardSize - 2, 1, otherPlayer);
		checkLeftDown(boardSize - 2, 1, otherPlayer);
		checkBelow(boardSize - 2, 1, otherPlayer);
	}

	if (othelloBoard[1][boardSize - 2] == 0) {
		checkAbove(1, boardSize - 2, otherPlayer);
		checkRightUp(1, boardSize - 2, otherPlayer);
		checkRight(1, boardSize - 2, otherPlayer);
	}

	if (othelloBoard[boardSize - 2][boardSize - 2] == 0) {
		checkAbove(boardSize - 2, boardSize - 2, otherPlayer);
		checkLeftUp(boardSize - 2, boardSize - 2, otherPlayer);
		checkLeft(boardSize - 2, boardSize - 2, otherPlayer);
	}

	// 2nd row down excluding x squares
	for (int x = 2; x < boardSize - 2; x += 1) {
		if (othelloBoard[x][1] == 0) {
			checkLeft(x, 1, otherPlayer);
			checkLeftDown(x, 1, otherPlayer);
			checkBelow(x, 1, otherPlayer);
			checkRightDown(x, 1, otherPlayer);
			checkRight(x, 1, otherPlayer);
		}

		// Horizontal bottom row
		if (othelloBoard[x][boardSize - 2] == 0) {
			checkLeft(x, boardSize - 2, otherPlayer);
			checkLeftUp(x, boardSize - 2, otherPlayer);
			checkAbove(x, boardSize - 2, otherPlayer);
			checkRightUp(x, boardSize - 2, otherPlayer);
			checkRight(x, boardSize - 2, otherPlayer);
		}
	}

	for (int y = 2; y < boardSize - 2; y += 1) {
		if (othelloBoard[0][y] == 0) {
			checkAbove(1, y, otherPlayer);
			checkRightUp(1, y, otherPlayer);
			checkRight(1, y, otherPlayer);
			checkRightDown(1, y, otherPlayer);
			checkBelow(1, y, otherPlayer);
		}

		// Vertical right column
		if (othelloBoard[boardSize - 1][y] == 0) {
			checkAbove(boardSize - 2, y, otherPlayer);
			checkLeftUp(boardSize - 2, y, otherPlayer);
			checkLeft(boardSize - 2, y, otherPlayer);
			checkLeftDown(boardSize - 2, y, otherPlayer);
			checkBelow(boardSize - 2, y, otherPlayer);
		}
	}

	for (int x = 2; x < boardSize - 2; x += 1) {
		for (int y = 2; y < boardSize - 2; y += 1) {
			if (othelloBoard[y][x] == 0) {
				checkAbove(y, x, otherPlayer);
				checkRightUp(y, x, otherPlayer);
				checkRight(y, x, otherPlayer);
				checkRightDown(y, x, otherPlayer);
				checkBelow(y, x, otherPlayer);
				checkLeftDown(y, x, otherPlayer);
				checkLeft(y, x, otherPlayer);
				checkLeftUp(y, x, otherPlayer);
			}
		}
	}

	//cout << "potential mobility = " << potentialMobilityCurrentPlayer << endl;
	return potentialMobilityCurrentPlayer;
}

void MiniMaxNode::checkAbove(int column, int row, int playerNumber) {
	////cout << "Checking above - was given row = " << row << " and column = " << column << endl;
	////cout << "Value above = " << othelloBoard[column][row - 1] << endl;

	if (othelloBoard[column][row - 1] == playerNumber) {
		////cout << "Mobility above when looking in row = " << row << " and column = " << column << endl;
		if (stableDiscs[column][row - 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for current player + 1 for above when looking in " << row << "," << column << endl;
				potentialMobilityCurrentPlayer += 1;
			}
		}
		else {
			//cout << "No potential mobility for above when looking in " << row << "," << column << " because it's stable " << endl;
		}

	}
	else if (othelloBoard[column][row - 1] == playerNumber *-1) {
		if (stableDiscs[column][row - 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for OTHER player - 1 for above when looking in " << row << "," << column << endl;
				potentialMobilityOtherPlayer += 1;
			}
		}
	}
}

void MiniMaxNode::checkBelow(int column, int row, int playerNumber) {
	////cout << "Checking below - was given row = " << row << " and column = " << column << endl;
	////cout << "Value below = " << othelloBoard[column][row + 1] << endl;

	if (othelloBoard[column][row + 1] == playerNumber) {
		if (stableDiscs[column][row + 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for current player + 1 for below when looking in " << row << "," << column << endl;
				potentialMobilityCurrentPlayer += 1;
			}
		}
		else {
			//cout << "No potential mobility for below when looking in " << row << "," << column << " because it's stable " << endl;
		}
	}
	else if (othelloBoard[column][row + 1] == playerNumber *-1) {
		if (stableDiscs[column][row + 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for OTHER player - 1 for above when looking in " << row << "," << column << endl;
				potentialMobilityOtherPlayer += 1;
			}
		}
	}
}

void MiniMaxNode::checkRight(int column, int row, int playerNumber) {
	////cout << "Checking right - was given row = " << row << " and column = " << column << endl;
	////cout << "Value right = " << othelloBoard[column+1][row] << endl;

	if (othelloBoard[column + 1][row] == playerNumber) {
		if (stableDiscs[column + 1][row] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for current player + 1 for right when looking in " << row << "," << column << endl;
				potentialMobilityCurrentPlayer += 1;
			}
		}
		else {
			//cout << "No potential mobility for right when looking in " << row << "," << column << " because it's stable " << endl;
		}
	}
	else if (othelloBoard[column + 1][row] == playerNumber *-1) {
		if (stableDiscs[column + 1][row] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for OTHER player - 1 for above when looking in " << row << "," << column << endl;
				potentialMobilityOtherPlayer += 1;
			}
		}
	}
}

void MiniMaxNode::checkLeft(int column, int row, int playerNumber) {
	////cout << "Checking left - was given row = " << row << " and column = " << column << endl;
	////cout << "Value left = " << othelloBoard[column-1][row] << endl;

	if (othelloBoard[column - 1][row] == playerNumber) {
		if (stableDiscs[column - 1][row] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for current player + 1 for left when looking in " << row << "," << column << endl;
				potentialMobilityCurrentPlayer += 1;
			}
		}
		else {
			//cout << "No potential mobility for left when looking in " << row << "," << column << " because it's stable " << endl;
		}
	}
	else if (othelloBoard[column - 1][row] == playerNumber *-1) {
		if (stableDiscs[column - 1][row] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for OTHER player - 1 for above when looking in " << row << "," << column << endl;
				potentialMobilityOtherPlayer += 1;
			}
		}
	}
}

void MiniMaxNode::checkLeftUp(int column, int row, int playerNumber) {
	////cout << "Checking left up - was given row = " << row << " and column = " << column << endl;
	////cout << "Value up = " << othelloBoard[column-1][row - 1] << endl;

	if (othelloBoard[column - 1][row - 1] == playerNumber) {
		if (stableDiscs[column - 1][row - 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for current player + 1 for left up when looking in " << row << "," << column << endl;
				potentialMobilityCurrentPlayer += 1;
			}
		}
		else {
			//cout << "No potential mobility for left up when looking in " << row << "," << column << " because it's stable " << endl;
		}
	}
	else if (othelloBoard[column - 1][row - 1] == playerNumber *-1) {
		if (stableDiscs[column - 1][row - 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for OTHER player - 1 for above when looking in " << row << "," << column << endl;
				potentialMobilityOtherPlayer += 1;
			}
		}
	}
}

void MiniMaxNode::checkLeftDown(int column, int row, int playerNumber) {
	////cout << "Checking left down - was given row = " << row << " and column = " << column << endl;
	////cout << "Value left down = " << othelloBoard[column-1][row + 1] << endl;

	if (othelloBoard[column - 1][row + 1] == playerNumber) {
		if (stableDiscs[column - 1][row + 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for current player + 1 for left down when looking in " << row << "," << column << endl;
				potentialMobilityCurrentPlayer += 1;
			}
		}
		else {
			//cout << "No potential mobility for left down when looking in " << row << "," << column << " because it's stable " << endl;
		}
	}
	else if (othelloBoard[column - 1][row + 1] == playerNumber *-1) {
		if (stableDiscs[column - 1][row + 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for OTHER player - 1 for above when looking in " << row << "," << column << endl;
				potentialMobilityOtherPlayer += 1;
			}
		}
	}
}

void MiniMaxNode::checkRightUp(int column, int row, int playerNumber) {
	////cout << "Checking right up - was given row = " << row << " and column = " << column << endl;
	////cout << "Value right up = " << othelloBoard[column+1][row - 1] << endl;

	if (othelloBoard[column + 1][row - 1] == playerNumber) {
		if (stableDiscs[column + 1][row - 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for current player + 1 for right up when looking in " << row << "," << column << endl;
				potentialMobilityCurrentPlayer += 1;
			}
		}
		else {
			//cout << "No potential mobility for right up when looking in " << row << "," << column << " because it's stable " << endl;
		}
	}
	else if (othelloBoard[column + 1][row - 1] == playerNumber *-1) {
		if (stableDiscs[column + 1][row - 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for OTHER player - 1 for above when looking in " << row << "," << column << endl;
				potentialMobilityOtherPlayer += 1;
			}
		}
	}
}

void MiniMaxNode::checkRightDown(int column, int row, int playerNumber) {
	////cout << "Checking right down - was given row = " << row << " and column = " << column << endl;
	////cout << "Value right down = " << othelloBoard[column+1][row +1] << endl;

	if (othelloBoard[column + 1][row + 1] == playerNumber) {
		if (stableDiscs[column + 1][row + 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for current player + 1 for right down when looking in " << row << "," << column << endl;
				potentialMobilityCurrentPlayer += 1;
			}
		}
		else {
			//cout << "No potential mobility for right down when looking in " << row << "," << column << " because it's stable " << endl;
		}
	}
	else if (othelloBoard[column + 1][row + 1] == playerNumber *-1) {
		if (stableDiscs[column + 1][row + 1] != 1) {

			if (playerNumber != maximisingPlayerNumber) {
				//cout << "Potential Mobility for OTHER player - 1 for above when looking in " << row << "," << column << endl;
				potentialMobilityOtherPlayer += 1;
			}
		}
	}
}

int MiniMaxNode::hasWon() {

	if (!availableMove(1) && !availableMove(-1)) {
		int winner = calculateWinner();
		if (winner == maximisingPlayerNumber) {
			return maximisingPlayerNumber;
		}
		else {
			return maximisingPlayerNumber*-1;
		}
	}
	else {
		return 0;
	}
}

// Return 1 if white wins. Returns -1 if black wins. Returns 0 if draw.
int MiniMaxNode::calculateWinner() {

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

	if (playerOneScore > playerMinusOneScore) {
		return 1;
	}
	else if (playerOneScore < playerMinusOneScore) {
		return -1;
	}
	else {
		return 0;
	}
}

// Calculates how many available moves there are for the given player number
bool MiniMaxNode::availableMove(int playerNumber) {
	
	//bool a = false;

	for (int row = 0; row < boardSize; row++) {
		for (int column = 0; column < boardSize; column++) {

			//a = false;

			if (othelloBoard[column][row] == 0) {

				if (flipRight(row, column, playerNumber)) {
					//cout << "Flip right in " << row << ", " << column << endl;
					//a = true;
					return true;
				}

				if (flipLeft(row, column, playerNumber)) {
					//cout << "Flip left in " << row << "," << column << endl;
					//a = true;
					return true;
				}

				if (flipUp(row, column, playerNumber)) {
					//a = true;
					return true;
				}

				if (flipDown(row, column, playerNumber)) {
					//a = true;
					return true;
				}

				if (flipRightDown(row, column, playerNumber)) {
					//a = true;
					return true;
				}

				if (flipRightUp(row, column, playerNumber)) {
					//a = true;
					return true;
				}

				if (flipLeftDown(row, column, playerNumber)) {
					//a = true;
					return true;
				}

				if (flipLeftUp(row, column, playerNumber)) {
					//a = true;
					return true;
				}
			}
		}
	}
	return false;
}

