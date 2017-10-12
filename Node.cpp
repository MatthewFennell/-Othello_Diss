#include "Node.h"
#include "Board.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include <math.h> 

using namespace cv;
using namespace std;

// Create a Node object
Node::Node(Node *parentNode, int *action, int player)
{
	// Initially the node has not been expanded
	beenExpanded = false;

	// The node has 0 wins
	numberOfWins = 0;

	// Need to initialise number of visits to 1 to avoid some divisions by zero
	numberOfVisits = 1;

	// Stores the move that was taken to get to this node
	choiceOfMove = action;

	// Stores the respective parent of this node
	parent = parentNode;

	// Stores the player number of the player who made the move to visit this node
	currentPlayer = player;

	// Only relevant if its a leaf node
	playerWhoWon = NULL;	

	// Number of children
	numberOfChildren = 0;
}

// Very important manual memory management
Node::~Node()
{	
	// For each node, iterate through the children, and delete the child
	// This will recursively delete all of the children
	for (int x = 0; x < numberOfChildren; x += 1) {
		Node *child = getChild(x);
		delete child;
	}

	// Once a node has deleted all of it's children, delete it's choice of move (an array that clogs up the memory)
	delete choiceOfMove;

	// Delete the container for the children
	delete children;

}

// Sets the array of child nodes
void Node::setChildren(Node** childArray, int numberChildren) {
	children = childArray;
}

// Returns the parent node
Node* Node::getParent() {
	return parent;
}

// Increases the number of times this Node has won by 1
void Node::increaseWinCount() {
	numberOfWins += 1.0;
}

// Increases the number of times this Node has been visited by 1
void Node::increaseVisitCount() {
	numberOfVisits += 1.0;
}

// Shows that this node has been expanded
void Node::setExpandedTrue() {
	beenExpanded = true;
}

// Get if the node has been expanded or not
bool Node::hasBeenExpanded() {
	return beenExpanded;
}

// Get a given child index
Node* Node::getChild(int childNumber) {
	return children[childNumber];
}

// Get the current player
int Node::getCurrentPlayer() {
	return currentPlayer;
}

// Get the move of the current node
int* Node::getMove() {
	return choiceOfMove;
}

// Set the player who won the game
// Only relevant for the leaf nodes
void Node::setPlayerWhoWon(int winner) {
	playerWhoWon = winner;
}

// Get the winning node
// Only relevant for the leaf nodes
int Node::getPlayerWhoWon() {
	return playerWhoWon;
}

// Set the current player of this node
// Only occurs when a player has no available moves and the turn is swapped
void Node::setCurrentPlayer(int player) {
	currentPlayer = player;
}

// Get how many times this node was visited
double Node::getNumberOfVisits() {
	return numberOfVisits;
}

// Get how many times this node won
double Node::getNumberOfWins() {
	return numberOfWins;
}

// Returns the upper confidence bound
double Node::calculateUpperConfidenceBound() {

	// Can be varied to balance exploitation vs exploration
	// Think larger means less varied searching
	double constant = sqrt(2);

	// Calculates how many times the parent node has been visited
	double parentVisits = getParent()->getNumberOfVisits();

	// How much the good options are exploited
	double exploitation = sqrt(2*log(parentVisits) / numberOfVisits);

	// Calculates the exploitation value. This emphashises the search of smaller sections.
	double estimatedReward = double(numberOfWins) / double(numberOfVisits);

	// Calculate the overall upper bound
	double upperBound = estimatedReward + constant*exploitation;

	return upperBound;
}

// Return the array of children belonging to this node
Node** Node::getChildren() {
	return children;
}

// Set how many children this node has
void Node::setNumberOfChildren(int numberChildren) {
	numberOfChildren = numberChildren;
}

// Get how many children this node has
int Node::getNumberOfChildren() {
	return numberOfChildren;
}