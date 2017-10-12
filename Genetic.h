#ifndef GENETIC_H
#define GENETIC_H

#include "MiniMax.h"
#include "Board.h"
#include "MiniMax.h"
#include <iostream>

using namespace std;
using namespace cv; // OpenCV API is in the C++ "cv" namespace

class Genetic
{
public:
	// -------------------------------------------------------------------------------------- //

	Genetic(int sizeOfBoard, int searchDepth, int numberOfGens, int populationSize, double mutationRate, int randomness);

	~Genetic();

	void fightToTheDeath();

	void selection();

	void crossOver();

	void transferGenerations();

	int playSquareGameComputerComputerMiniMax(MiniMax * p1, MiniMax * p2, int boardSize);

	int numberOfGenerations;
	int populationSize;
	double mutationProbability;
	int variance;
	int boardSize;
	int maxSearchDepth;

	MiniMax** populationMembers;
	MiniMax** nextGenerationMembers;

	int* numberOfWins;

	// -------------------------------------------------------------------------------------- //
};
#endif