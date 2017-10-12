#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv; // OpenCV API is in the C++ "cv" namespace

class Player
{
public:
	// Create a player
	Player();

	// Dealloacates a player
	~Player();

	// Confusing - Changes the values of 'pt' in player when the mouse is pressed
	static void mouseHandler(int event, int x, int y, int flags, void* param);

	// The array returned from getMove (contains the row and column)
	// First value = row, 2nd value = column
	int* row_column;

	// Takes an image of the board to display, returns the location pressed as the move
	int* getMove(Mat imageOfBoard, Board board);
};
#endif