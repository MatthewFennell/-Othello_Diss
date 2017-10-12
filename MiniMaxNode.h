#ifndef MiniMaxNode_H
#define MiniMaxNode_H

#include "MiniMaxNode.h"
#include "Board.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv; // OpenCV API is in the C++ "cv" namespace

class MiniMaxNode
{
public:
	// Create a MinimaxNode
	MiniMaxNode(MiniMaxNode *parentMiniMaxNode, int **boardState, int currentPlayer, int sizeOfBoard, int* selectedMove, int maxPlayer, int corner, int adjacent, int edge, int internalStable, int constantESAC, int multiplierESAC, int constantCMAC, int multiplierCMAC);

	// Deallocate a MinimaxNode, freeing up lots of memory
	~MiniMaxNode();

	void deleteBoard();

	// --------------------------------------MinimaxNode SETTERS------------------------------------ //
	// The methods that alter MinimaxNode variable values
	void setChildren(MiniMaxNode** childArray, int numberChildren);
	void setCurrentPlayer(int player);
	void setExpandedTrue();
	void setNumberOfChildren(int numberChildren);
	// --------------------------------------MinimaxNode GETTERS------------------------------------ //
	// Methods that return variables associated with the MinimaxNode
	MiniMaxNode * getParent();
	MiniMaxNode * getChild(int childNumber);
	int * getMove();
	int getCurrentPlayer();
	MiniMaxNode ** getChildren();
	int getNumberOfChildren();
	void printBoardValues();
	int ** getBoardValues();
	void setValue(int value);
	int getEvalNumber();
	bool flipRight(int row, int column, int player);
	bool flipLeft(int row, int column, int player);
	bool flipUp(int row, int column, int player);
	bool flipDown(int row, int column, int player);
	bool flipRightDown(int row, int column, int player);
	bool flipRightUp(int row, int column, int player);
	bool flipLeftUp(int row, int column, int player);
	bool flipLeftDown(int row, int column, int player);
	bool canPlaceHere(int row, int column, int player);
	int evaluatePosition();
	int XSquares();
	int getMoveNumber();
	int ESAC(int moveNumber);
	int CMAC(int moveNumber);
	int discCount();
	int immediateMobility();
	void deleteStables();
	void findStables();
	bool horizontalStability(int row, int column);
	bool verticalStability(int row, int column);
	bool rightDownStability(int row, int column);
	bool rightUpStability(int row, int column);
	bool isStable(int row, int column);
	int potentialMobilityOld();
	int potentialMobility();
	void checkAbove(int row, int column, int playerNumber);
	void checkBelow(int row, int column, int playerNumber);
	void checkRight(int row, int column, int playerNumber);
	void checkLeft(int row, int column, int playerNumber);
	void checkLeftUp(int row, int column, int playerNumber);
	void checkLeftDown(int row, int column, int playerNumber);
	void checkRightUp(int row, int column, int playerNumber);
	void checkRightDown(int row, int column, int playerNumber);
	int hasWon();
	int calculateWinner();
	bool availableMove(int playerNumber);
	// --------------------------------------MinimaxNode VARIABLES---------------------------------- //
	// All of the MinimaxNode variables
	MiniMaxNode *parent;
	MiniMaxNode** children;
	bool hasBeenExpanded();
	bool beenExpanded;
	int currentPlayer;
	int numberOfChildren;
	int** othelloBoard;
	int boardSize;
	int* choiceOfMove;
	int evaluationNumber;
	int maximisingPlayerNumber;
	int** stableDiscs;
	int potentialMobilityCurrentPlayer;
	int potentialMobilityOtherPlayer;
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