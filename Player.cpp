#include "Player.h"
#include "Board.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

// Create a player object
Player::Player()
{

}

Player::~Player()
{
	// all deallocating is done automatically
}


// This function is called when the left mouse button is pressed
// It affects 'pt', so that : pt.x = co-ordinate of x-axis pixel 
//							  pt.y = co-ordinate of y-axis pixel
void Player::mouseHandler(int event, int x, int y, int flags, void * param)
{
	if (event == CV_EVENT_LBUTTONDOWN) {
		Point* ptPtr = (Point*)param;
		ptPtr->x = x;
		ptPtr->y = y;
	}
}

// @imageOfBoard : the current display of the board
// Returns the pixel location selected - needs to be converted into row/column
int* Player::getMove(Mat imageOfBoard, Board board)
{
	bool validMove = false;

	while (!validMove) {
		// Set to (-1,-1) every time getMove() is called
		Point2i pt(-1, -1);

		// Shows the most recent board (taken as the input parameter)
		imshow("Board with discs", imageOfBoard);

		// Represents the pixel selection (x,y)
		row_column = new int[2];


		// Required for callback function
		setMouseCallback("Board with discs", mouseHandler, (void*)&pt);

		// While nothing has been clicked yet, keep refreshing the image
		while (pt.x == -1) {
			waitKey(50);
		}

		// Set the row and column to the pixels selected by the mouse
		row_column[0] = pt.x;
		row_column[1] = pt.y;

		if (pt.x < 720) {
			validMove = true;
		}
		else {
			cout << "Invalid move, please pick another! " << endl;
		}
	}
	return row_column;
}


