#ifndef Node_H
#define Node_H

#include "Node.h"
#include "Board.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv; // OpenCV API is in the C++ "cv" namespace

class Node
{
public:
	// Create a node
	Node(Node *parentNode, int *action, int currentPlayer);

	// Deallocate a node, freeing up lots of memory
	~Node();

	// --------------------------------------NODE SETTERS------------------------------------ //
	// The methods that alter node variable values
	void setChildren(Node ** childArray, int numberChildren);
	void setPlayerWhoWon(int winner);
	void setCurrentPlayer(int player);
	void setExpandedTrue();
	void setNumberOfChildren(int numberChildren);
	void increaseWinCount();
	void increaseVisitCount();
	// --------------------------------------NODE GETTERS------------------------------------ //
	// Methods that return variables associated with the node
	Node * getParent();
	Node * getChild(int childNumber);
	int * getMove();
	int getCurrentPlayer();
	int getPlayerWhoWon();
	double getNumberOfVisits();
	double getNumberOfWins();
	Node ** getChildren();
	int getNumberOfChildren();
	double calculateUpperConfidenceBound();
	// --------------------------------------NODE VARIABLES---------------------------------- //
	// All of the node variables
	Node *parent;
	Node** children;
	bool hasBeenExpanded();	
	double numberOfWins;
	double numberOfVisits;
	int* choiceOfMove;
	bool beenExpanded;
	int currentPlayer;
	int playerWhoWon;
	int numberOfChildren;
	// -------------------------------------------------------------------------------------- //
};
#endif