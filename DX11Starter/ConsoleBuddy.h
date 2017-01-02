#pragma once
#include "Tetris.h"

class ConsoleBuddy
{
public:
	ConsoleBuddy(Tetris &TetrisGame);
	~ConsoleBuddy();

	void UpdateCB(float deltaTime);

	int playerStatus;
private:
	std::string sent;

	bool previousState;
	bool currentState;

	void Talk(std::string sentance, float deltaTime);
	void Talk(std::string sentance);
	void ClearLine();

	bool botTalk;
	unsigned int sentIndex;

	int tut;
	int tetris;
	int free;

	float textTime;
	float textSpeed;

	Tetris* tetrisGame;
};

