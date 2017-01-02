#pragma once
#include "GameEntity.h"
#include "TetrisBlock.h"
#include <stdlib.h>
#include <time.h>

class Tetris
{
public:
	Tetris(Mesh &shape, GameEntity &gm, GameEntity &ovr, Material &r, Material &b, Material &g, Material &p, Material &lB, Material &y, Material &gr, int h, int w);
	~Tetris();

	void StartGame(int h, int w);
	void DrawBoard(int height, int width);
	void UpdateGame(float deltaTime, float totalTime);
	void ResetGame();
	void EndGame();
	std::vector<GameEntity>* GetBoard();
	std::vector<GameEntity> GetTBlocks();
	std::vector<GameEntity> GetPBlocks();
	TetrisBlock* GenerateBlock();
	void SetCurrentBlock(TetrisBlock *block);
	void SetFutureBlock();
	void MoveBlock();
	void CheckWallCollide();
	void CheckBlockCollide();
	bool CheckWallBlockCollide();
	void CheckFloorCollide();
	void CheckGameOver();
	void PlaceBlock();
	void CheckForLine();
	void Reposition(int yHieght);
	void AddToScore();
	
	Mesh* shapeBlock;
	Material* red;
	Material* blue;
	Material* green;
	Material* purple;
	Material* lightBlue;
	Material* yellow;
	Material* grey;

	TetrisBlock* futureBlock = nullptr;
	TetrisBlock* currentBlock = nullptr;

	int score;
	bool gameStart;
	bool gameOver;
	int height;
	int width;
	float tTime;
	float timeOfDescent;
	int scoreCol;
	int scoreRow;

	bool pChange;
	bool tChange;

	bool currentState;
	bool previousState;
	
	std::vector<GameEntity> board;
	std::vector<GameEntity> tBlocks;
	std::vector<GameEntity> scoreBlocks;
	std::vector<GameEntity> otherBlocks;

private:

};

