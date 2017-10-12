#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "Board.h"
#include "Node.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv; // OpenCV API is in the C++ "cv" namespace

class MonteCarlo
{
public:
	// Create a player
	MonteCarlo(int simulationLimit);

	// Max number of simulations
	int simLimit;

	// Deallocates a player
	~MonteCarlo();

	// Travels through the tree from bottom upwards
	// Increases win count for nodes that won
	// Increases visit count for all nodes used
	void backPropagate(Node * finalPlaced, int winner);

	// Clones the board, returning a board with the same values to be used
	Board * cloneBoard(Board boardToClone);

	// Takes the current board state and player number
	// Returns a given action based on the Monte Carlo Tree Search method
	int* getMoveMonteCarlo(Board * board, int playerNumber);

	// Takes the start node, and plays random simulations from that node until the end of the game
	Node* playRandomSimulations(Node * randomFromHere, Board * board);

	// Expands a node, generating all of the children possible from that given node
	void expandNode(Node * nodeToExpand, Board * board, int currentPlayer);

	// The beginning of the Monte Carlo process
	// Takes the initial node and keeps selecting the most urgent child until that child node hasn't been expanded
	// Then plays random simulations
	void selection(Node * start, Board *board);

	// Selects the most promising node
	// This is based on various mathematical formulae
	Node * selectMostUrgentNode(Node * currentParent);


	int* test;
};
#endif
