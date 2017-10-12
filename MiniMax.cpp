#include "MiniMax.h"
#include "Board.h"
#include "MiniMaxNode.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include <time.h>	

using namespace cv;
using namespace std;

// Create a computer player object
// sizeOfBoard = Size of the Board
// maximumSearchDepth = Depth of the Game Tree
// playerNumber = Which player is this AI playing as
// corner = Weighting of Stable corners
// edge = Weighting of Stable edges
// internalStable = Weighting of Internal Stable discs
// immedMobilityMultiplier = A constant for Edge Stability
// potetMobilityMultiplier = A multiplier for Edge Stability
// multiplierXSquare = A constant for Mobility
// multiplierDiscCount = A multiplier for Mobility
MiniMax::MiniMax(int sizeOfBoard, int maximumSearchDepth, int corner, int adjacent, int edge, int internalStable, int immedMobilityMultiplier, int potetMobilityMultiplier, int multiplierXSquare, int multiplierDiscCount)
{
	boardSize = sizeOfBoard;
	// Nothing to initialise for a player object yet
	maxDepth = maximumSearchDepth;

	maximisingPlayerNumber = 0;

	stableCornerWeighting = corner;
	stableAdjacentCornerWeighting = adjacent;
	stableEdgeWeighting = edge;
	stableInternalWeighting = internalStable;

	immediateMobilityMultiplier = immedMobilityMultiplier;
	potentialMobilityMultiplier = potetMobilityMultiplier;

	xSquareMultiplier = multiplierXSquare;
	discCountMultiplier = multiplierDiscCount ;

	//cout << "A baby has been made with chromosones :" << endl;
	//cout << "Corner value = " << corner << endl;
	//cout << "Adjacent = " << adjacent << endl;
	//cout << "Edge = " << edge << endl;
	//cout << "Internal stable = " << internalStable << endl;
	//cout << "Immediatie mobility multiplier = " << immediateMobilityMultiplier << endl;
	//cout << "Potential mobility multiplier = " << potentialMobilityMultiplier << endl;
	//cout << "X Square multiplier = " << xSquareMultiplier << endl;
	//cout << "Disc count multiplier = " << discCountMultiplier << endl << endl;
}

// Deallocates a player object
MiniMax::~MiniMax()
{
	// all deallocating is done automatically
}

// Takes a Board input and returns a separate object that has the same board values as in the inputted board
Board* MiniMax::cloneBoard(Board *boardToClone) {

	// Retrives the board value information
	int** temporaryBoardValues = boardToClone->getBoardValues();

	// Makes a new board object with the previous parameters
	Board *temporaryBoard = new Board(NULL, boardSize, NULL);

	//temporaryBoard->printBoardValues();

	// Sets the new board values to the same as the old ones
	temporaryBoard->setBoardValues(temporaryBoardValues);

	// Return a clone of the input board
	return temporaryBoard;
}

// Takes the current board state and the current player
// Returns a move based off the Monte Carlo process
int* MiniMax::getMoveMiniMax(Board *board, int playerNumber) {

	int childToSelect = -1;

	// Initialise the root node
	// It has no action or parent associated with it
	MiniMaxNode *root = new MiniMaxNode(NULL, board->getBoardValues(), playerNumber, boardSize, NULL, maximisingPlayerNumber, stableCornerWeighting, stableAdjacentCornerWeighting, stableEdgeWeighting, stableInternalWeighting, immediateMobilityMultiplier, potentialMobilityMultiplier, xSquareMultiplier, discCountMultiplier);

	// Generates the tree to the max depth
	expandSelf(root, playerNumber, 0);	

	if (root->getNumberOfChildren() == 1) {
		return root->getChild(0)->getMove();
	}
	
	int alphaBetaResult = alphaBeta(root, maxDepth, -99999, 99999, 1);

	//cout << "Result = " << alphaBetaResult << endl;

	// If a node had no children, it was never having it's value correctly set
	// Moves that are end moves need to set their own value
	for (int x = 0; x < root->getNumberOfChildren(); x += 1) {
		if (root->getChild(x)->getNumberOfChildren() == 0) {
			root->getChild(x)->setValue(root->getChild(x)->evaluatePosition());
		}
	}
	
	for (int x = 0; x < root->getNumberOfChildren(); x += 1) {
		if (root->getChild(x)->getEvalNumber() == alphaBetaResult) {			
			childToSelect = x;
			break;
		}
	}

	//	FIXES ODD BUG, NOT SURE WHY
	// ROOT CHILDREN DO NOT HAVE SAME EVAL RESULT AS THE ROOT?!
	int bestResult = -1000;

	if (childToSelect == -1) {
		for (int x = 0; x < root->getNumberOfChildren(); x += 1) {
			if (root->getChild(x)->getEvalNumber() > bestResult) {
				childToSelect = x;
				bestResult = root->getChild(x)->getEvalNumber();
			}
		}
	}

	int* test = new int[2];

	test[0] = root->getChild(childToSelect)->getMove()[0];
	test[1] = root->getChild(childToSelect)->getMove()[1];

	delete root;
	// error when playing 8x8 as black
	// place moves in (1,2), (1,0), (2,1), (0,1), (3,4), (0,3), (5,2), (2,5)
	return test;
}

int MiniMax::alphaBeta(MiniMaxNode *origin, int depth, int alpha, int beta, bool maximisingPlayer) 
{
	Board *temporaryBoard = new Board(NULL, boardSize, NULL);
	temporaryBoard->setBoardValues(origin->getBoardValues());
	int availableMoves = temporaryBoard->numberOfAvailableMoves(1);
	int availableMovesTwo = temporaryBoard->numberOfAvailableMoves(-1);	
	temporaryBoard->deleteBoard();
	delete temporaryBoard;

	if (depth == 0 || (availableMoves == 0 && availableMovesTwo == 0)) {
		int evaluation =  origin->evaluatePosition();
		//cout << "eval = " << evaluation <<" at a depth of " << depth << endl;		
		//origin->printBoardValues();
		return evaluation;
	}	
	if (maximisingPlayer) {
		int v = -9999999;
		for (int x = 0; x < origin->getNumberOfChildren(); x += 1) {
			v = max(v, alphaBeta(origin->getChild(x), depth - 1, alpha, beta, 0));
			alpha = max(alpha, v);
			origin->setValue(v);
			if (beta <= alpha) {
				break;
			}
		}
		return v;
	}		
	else {
		int v = 9999999;
		for (int x = 0; x < origin->getNumberOfChildren(); x += 1) {
			v = min(v, alphaBeta(origin->getChild(x), depth - 1, alpha, beta, 1));
			beta = min(beta, v);
			origin->setValue(v);
			if (beta <= alpha) {
				break;
			}
		}
		return v;	
	}
}

// Expands a given node with respect to certain player, generating all possible child nodes for that player
void MiniMax::expandSelf(MiniMaxNode *nodeToExpand, int currentPlayer, int depth)
{
	if (depth < maxDepth) {
		
		Board *temporaryBoard = new Board(NULL, boardSize, NULL);
		temporaryBoard->setBoardValues(nodeToExpand->getBoardValues());
		expandNode(nodeToExpand, temporaryBoard, currentPlayer);	

		if (nodeToExpand->getNumberOfChildren() == 0) {
			expandNode(nodeToExpand, temporaryBoard, currentPlayer*-1);
		}

		temporaryBoard->deleteBoard();
		delete temporaryBoard;

		int numberOfChildren = nodeToExpand->getNumberOfChildren();
		if (numberOfChildren) {
			for (int x = 0; x < numberOfChildren; x += 1) {
				//cout << "Board at child number " << x << endl;
				//nodeToExpand->getChild(x)->printBoardValues();
				expandSelf(nodeToExpand->getChild(x), currentPlayer*-1, depth + 1);				
			}
		}
	}
}

// Expands a given node with respect to certain player, generating all possible child nodes for that player
void MiniMax::expandNode(MiniMaxNode *nodeToExpand, Board *board, int currentPlayer)
{
	// Sets this node to expanded = true
	nodeToExpand->setExpandedTrue();

	// Calculates how many children the node will have
	int numberOfChildren = board->numberOfAvailableMoves(currentPlayer);

	// Sets the number of children that this node has
	nodeToExpand->setNumberOfChildren(numberOfChildren);

	// If the node has at least 1 child
	if (numberOfChildren) {

		// Creates an array for the children
		MiniMaxNode** myChildren = new MiniMaxNode*[numberOfChildren];

		// Creates an array of arrays representing all of the potential moves for the nodes player
		int** availableMoves = board->availableMoveLocations(currentPlayer, numberOfChildren);

		for (int x = 0; x < numberOfChildren; x += 1) {

			Board *temporaryBoard = cloneBoard(board);
			temporaryBoard->placeDisc(availableMoves[x][0], availableMoves[x][1], currentPlayer);

			// Assigns each child one of the available moves.
			myChildren[x] = new MiniMaxNode(nodeToExpand, temporaryBoard->getBoardValues(), currentPlayer*-1, boardSize, availableMoves[x], maximisingPlayerNumber, stableCornerWeighting, stableAdjacentCornerWeighting, stableEdgeWeighting, stableInternalWeighting, immediateMobilityMultiplier, potentialMobilityMultiplier, xSquareMultiplier, discCountMultiplier);
			temporaryBoard->deleteBoard();
			delete temporaryBoard;;
		}
		// Sets the expanded nodes children
		nodeToExpand->setChildren(myChildren, numberOfChildren);

		// Deletes the array to stop the memory leaking
		delete[] availableMoves;
	}
}

int MiniMax::getCornerValue() {
	return stableCornerWeighting;
}

int MiniMax::getAdjacentValue() {
	return stableAdjacentCornerWeighting;
}

int MiniMax::getEdgeValue() {
	return stableEdgeWeighting;
}

int MiniMax::getInternalValue() {
	return stableInternalWeighting;
}

int MiniMax::getImmediateMobilityMultiplier() {
	return immediateMobilityMultiplier;
}

int MiniMax::getPotentialMobilityMultiplier() {
	return potentialMobilityMultiplier;
}

int MiniMax::getXSquareMultiplier() {
	return xSquareMultiplier;
}

int MiniMax::getDiscCountMultiplier() {
	return discCountMultiplier;
}

void MiniMax::setPlayerNumber(int number){
	maximisingPlayerNumber = number;
}
	
void MiniMax::printHeuristics() {
	cout << "Cor = " << stableCornerWeighting << endl;
	cout << "Adj = " << stableAdjacentCornerWeighting << endl;
	cout << "Edg = " << stableEdgeWeighting << endl;
	cout << "Int = " << stableInternalWeighting << endl;
	cout << "Imm = " << immediateMobilityMultiplier << endl;
	cout << "Pot = " << potentialMobilityMultiplier << endl;
	cout << "Xsq = " << xSquareMultiplier << endl;
	cout << "Dis = " << discCountMultiplier << endl;
	cout << "---------------------------------" << endl;
}