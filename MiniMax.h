#ifndef MiniMax_H
#define MiniMax_H

#include "Board.h"
#include "MiniMaxNode.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv; // OpenCV API is in the C++ "cv" namespace

class MiniMax
{
public:
	// Create a player
	MiniMax(int sizeOfBoard, int maximumSearchDepth,  int corner, int adjacent, int edge, int internalStable, int constantESAC, int multiplierESAC, int constantCMAC, int multiplierCMAC);

	// Size of the board
	int boardSize;

	// Deallocates a player
	~MiniMax();

	int maxDepth;

	int maximisingPlayerNumber;

	// Clones the board, returning a board with the same values to be used
	Board * cloneBoard(Board *boardToClone);

	// Takes the current board state and player number
	// Returns a given action based on the Monte Carlo Tree Search method
	int* getMoveMiniMax(Board * board, int playerNumber);

	int alphaBeta(MiniMaxNode *origin, int depth, int alpha, int beta, bool maximisingPlayer);

	void expandSelf(MiniMaxNode * nodeToExpand, int currentPlayer, int depth);
	
	// Expands a node, generating all of the children possible from that given node
	void expandNode(MiniMaxNode * nodeToExpand, Board * board, int currentPlayer);

	int getCornerValue();

	int getAdjacentValue();

	int getEdgeValue();

	int getInternalValue();

	int getImmediateMobilityMultiplier();
	int getPotentialMobilityMultiplier();

	int getXSquareMultiplier();

	int getDiscCountMultiplier();

	void setPlayerNumber(int number);

	void printHeuristics();

	// -------------------------------------------------------------------------------------- //
	// Genetic features
	int stableCornerWeighting;
	int stableAdjacentCornerWeighting;
	int stableEdgeWeighting;
	int stableInternalWeighting;

	int immediateMobilityMultiplier;
	int potentialMobilityMultiplier;

	int xSquareMultiplier;
	int discCountMultiplier;
	// -------------------------------------------------------------------------------------- //
};

#endif
