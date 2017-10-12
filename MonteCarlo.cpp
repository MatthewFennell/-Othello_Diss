#include "MonteCarlo.h"
#include "Board.h"
#include "Node.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include <time.h>	

using namespace cv;
using namespace std;

// Create a computer player object
MonteCarlo::MonteCarlo(int simulationLimit)
{
	simLimit = simulationLimit;
	// Nothing to initialise for a player object yet
}

// Deallocates a player object
MonteCarlo::~MonteCarlo()
{
	// all deallocating is done automatically
}

// Recursively travel through the tree bottom up, increasing win and visit values
void MonteCarlo::backPropagate(Node *finishingNode, int winner) {

	// Keep looping if the parent isn't NULL
	if (finishingNode->getParent() != NULL) {
		// If the parent nodes player is equal to the winning player, then this was a successful node
		// Increases both win and visit
		if (finishingNode->getParent()->getCurrentPlayer() == winner) {
			finishingNode->increaseWinCount();
			finishingNode->increaseVisitCount();
		}
		// Otherwise increase just the visit
		else {
			finishingNode->increaseVisitCount();
		}
		// Since we didn't reach the root node, recursively backpropagate on the parent of this node
		backPropagate(finishingNode->getParent(), winner);
	}
	// If the parent is NULL, then this must be the root, so increase the visit count and finish backpropagating
	else {
		finishingNode->increaseVisitCount();
	}
}

// Takes a Board input and returns a separate object that has the same board values as in the inputted board
Board* MonteCarlo::cloneBoard(Board boardToClone) {
	// Retrives the board parameters
	int blockSize = boardToClone.getBlockSize();
	int boardSize = boardToClone.getBoardSize();
	int offset = boardToClone.getOffset();

	// Retrives the board value information
	int** temporaryBoardValues = boardToClone.getBoardValues();

	// Makes a new board object with the previous parameters
	Board *temporaryBoard = new Board(blockSize, boardSize, offset);

	// Sets the new board values to the same as the old ones
	temporaryBoard->setBoardValues(temporaryBoardValues);

	// Return a clone of the input board
	return temporaryBoard;
}

// Takes the current board state and the current player
// Returns a move based off the Monte Carlo process
int* MonteCarlo::getMoveMonteCarlo(Board *board, int playerNumber) {
	// Initialise the root node
	// It has no action or parent associated with it
	Node *root = new Node(NULL, NULL, playerNumber);

	// Clone the current board to store the current values of the board tiles
	// Then the board can be reset to the original values after each simulation
	Board *temp = cloneBoard(*board);

	// This is the board that will be altered during every simulation
	Board *boardToPlayWith = cloneBoard(*board);

	// Stores the original board values so that the board can be reset after each simulation
	int** tempValues = temp->getBoardValues();

	// Run the Monte Carlo simulations
	for (int x = 0; x < simLimit; x += 1) {

		// At the beginning of each simulation, set the board back to the original state at this stage in the game
		boardToPlayWith->setBoardValues(tempValues);

		// Print out how close the computer is to finishing the simulation

		if (x % (simLimit / 100) == 0) {
			double percentage = double(x) / simLimit * 100;
			printf("%.1f %%\r", percentage);
			fflush(stdout);
			//cout << percentage << "% \r" << endl;
		}
		

		// Begin the Monte Carlo Tree Search
		selection(root, boardToPlayWith);
	}

	// Calculates the estimated win rate of the computer from this position
	double winRate = selectMostUrgentNode(root)->getNumberOfWins() / selectMostUrgentNode(root)->getNumberOfVisits();
	
	// Select the next move by taking the most urgent child of the root
	int* action = selectMostUrgentNode(root)->getMove();
	//cout << "% win rate from here = " << 100 * winRate << endl;



	test = new int[2];

	test[0] = action[0];
	test[1] = action[1];

	// Displays what move the computer is going to play
	//cout << "Action = " << action[0] << "," << action[1] << endl;

	// Wait a few seconds to see where the computer will play and what the consequences will be
	//waitKey(3);

	// Begin the recursive deleting of the tree
	delete root;


	// Return the move determined by the computer
	return test;
}

// Takes the node to play from and the current board
// Randomly plays simulations until the end of the game from this node
Node* MonteCarlo::playRandomSimulations(Node *randomFromHere, Board *board) {

	// Sets the random seed so that each game is different
	srand(time(0));

	// The current player
	int currentPlayer = randomFromHere->getCurrentPlayer();

	// Expands the node
	expandNode(randomFromHere, board, currentPlayer);

	// Gets the number of children
	int numberOfChildren = board->numberOfAvailableMoves(currentPlayer);

	// If there are children, then a random move must be possible
	if (numberOfChildren > 0) {

		// Chooses a random child
		int randNum = rand() % (numberOfChildren);

		// Gets the move of that random child
		int* actionChosen = randomFromHere->getChild(randNum)->getMove();

		// Plays that disc
		board->placeDisc(actionChosen[0], actionChosen[1], currentPlayer);

		// Recursively play random simulations on one of the randomly chosen children
		playRandomSimulations(randomFromHere->getChild(randNum), board);
	}
	else {
		// The previous player had no turns available. Try from other players perspective
		currentPlayer *= -1;

		// Expand node
		expandNode(randomFromHere, board, currentPlayer);

		// Get number of children
		int numberOfChildren = board->numberOfAvailableMoves(currentPlayer);

		// Again, if this player now has children then there must be moves available
		if (numberOfChildren > 0) {

			// The current player has now changed, so need to edit that
			randomFromHere->setCurrentPlayer(currentPlayer);

			// Chooses a random child
			int randNum = rand() % (numberOfChildren);

			// Gets the move of that random child
			int* actionChosen = randomFromHere->getChild(randNum)->getMove();

			// Place disc
			board->placeDisc(actionChosen[0], actionChosen[1], currentPlayer);

			// Recursively play random simulations on one of the randomly chosen children
			playRandomSimulations(randomFromHere->getChild(randNum), board);
		}
		else {
			// No remaining moves left, end the game and calculate the winner
			randomFromHere->setPlayerWhoWon(board->calculateWinner());
			return randomFromHere;
		}
	}
}

// Expands a given node with respect to certain player, generating all possible child nodes for that player
void MonteCarlo::expandNode(Node *nodeToExpand, Board *board, int currentPlayer)
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
		Node** myChildren = new Node*[numberOfChildren];

		// Creates an array of arrays representing all of the potential moves for the nodes player
		int** availableMoves = board->availableMoveLocations(currentPlayer, numberOfChildren);

		for (int x = 0; x < numberOfChildren; x += 1) {
			// Assigns each child one of the available moves.
			myChildren[x] = new Node(nodeToExpand, availableMoves[x], currentPlayer*-1);
		}
		// Sets the expanded nodes children
		nodeToExpand->setChildren(myChildren, numberOfChildren);

		// Deletes the array to stop the memory leaking
		delete[] availableMoves;
	}
}

// Check if the root has been expanded. If it hasn't, expand it and play games randomly until the end.
// If it has, keep selecting the node with the largest confidence bound until unexplored, and play randomly from there.
void MonteCarlo::selection(Node *start, Board *board) {

	// Gets how many children the current node has
	int numberOfChildren = board->numberOfAvailableMoves(start->getCurrentPlayer());

	// If it has been expanded and it is not the end of a game
	if (start->hasBeenExpanded() && (numberOfChildren > 0)) {

		// Selects the most urgent of the nodes children
		Node *mostUrgentNode = selectMostUrgentNode(start);

		// Gets the move of the most urgent child
		int* urgentNodesMove = mostUrgentNode->getMove();

		// Places the disc in that position on the board being simulated with
		board->placeDisc(urgentNodesMove[0], urgentNodesMove[1], mostUrgentNode->getParent()->getCurrentPlayer());

		// Recursively continue simulating
		selection(mostUrgentNode, board);
	}
	// If the node hasn't been expanded or it's the end of the game
	else {
		// Play random simulations and get the final node
		Node *finalPlaced = playRandomSimulations(start, board);

		// Find out who won the game
		int winner = finalPlaced->getPlayerWhoWon();

		// Back propagate, updating the win / visit values for each node that was selected
		backPropagate(finalPlaced, winner);
	}
}

// Returns the node with the highest upper confidence bound from all of the children of the given parent
Node* MonteCarlo::selectMostUrgentNode(Node *currentParent) {

	// Grabs the children nodes of the parent node in question
	Node** children = currentParent->getChildren();

	// Finds out how many children the parent node has
	int numberOfChildren = currentParent->getNumberOfChildren();

	// Represents the highest upper bound that will be found
	double upperBound = 0.0;

	// Initalise the current childs bound to 0.0
	double currentChildUpperBound = 0.0;

	// To know which child had the largest upper bound
	int greatestChildIndex = 0;

	// Loop through the children
	for (int x = 0; x < numberOfChildren; x += 1) {
		// Find out the current childs upper bound
		currentChildUpperBound = children[x]->calculateUpperConfidenceBound();

		// If the current childs upper bound is larger than the largest upper bound found so far, set it as the new highest upper bound
		if (currentChildUpperBound > upperBound) {
			upperBound = currentChildUpperBound;
			greatestChildIndex = x;
		}
	}
	return children[greatestChildIndex];
}