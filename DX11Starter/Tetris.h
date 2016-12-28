#pragma once
#include "GameEntity.h"
#include "TetrisBlock.h"
#include <stdlib.h>
#include <time.h>

class Tetris
{
public:
	Tetris(Mesh &shape, Material &r, Material &b, Material &g, Material &p);
	~Tetris();

	void StartGame(int h, int w);
	void DrawBoard(int height, int width);
	void UpdateGame();
	TetrisBlock* GenerateBlock();
	std::vector<GameEntity> GetBoard();
	std::vector<GameEntity> GetBlocks();
	void SetCurrentBlock(TetrisBlock *block);
	void SetFutureBlock();
	void MoveBlock();
	void CheckWallCollide();
	void CheckFloorCollide();
	
	Mesh* shapeBlock;
	Material* red;
	Material* blue;
	Material* green;
	Material* purple;

	TetrisBlock* futureBlock = nullptr;
	TetrisBlock* currentBlock = nullptr;

	bool gameStart;
	int height;
	int width;
	float tTime;
	float timeOfDescent;

	bool currentState;
	bool previousState;
	
	std::vector<GameEntity> board;
	std::vector<TetrisBlock*> tBlocks;

private:

};

