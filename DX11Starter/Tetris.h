#pragma once
#include "GameEntity.h"
#include "TetrisBlock.h"
#include <stdlib.h>

class Tetris
{
public:
	Tetris(Mesh &shape, Material &r, Material &b, Material &g, Material &p);
	~Tetris();

	void StartGame(int h, int w);
	void DrawBoard(int height, int width);
	TetrisBlock* GenerateBlock();
	std::vector<GameEntity> GetBoard();
	std::vector<GameEntity> GetBlocks();
	
	Mesh* shapeBlock;
	Material* red;
	Material* blue;
	Material* green;
	Material* purple;

	TetrisBlock* futureBlock;
	TetrisBlock* currentBlock;

	bool gameStart;
	int height;
	int width;
	
	std::vector<GameEntity> board;
	std::vector<TetrisBlock*> tBlocks;

private:

};

