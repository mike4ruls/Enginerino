#pragma once
#include "Tetris.h"
#include "Renderer.h"
#include "Camera.h"

class ConsoleBuddy
{
public:
	ConsoleBuddy(Tetris &TetrisGame, Renderer &end, Camera &cm);
	~ConsoleBuddy();

	void UpdateCB(float deltaTime);
	void Script(float deltaTime);

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
	float botTime;
	float botTimeTalk;

	Tetris* tetrisGame;
	Renderer* rend;
	Camera* cam;
};

